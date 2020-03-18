#include "brqttcamera.h"

#include "CameraRemote_SDK.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QUrl>
#include <QVideoSurfaceFormat>
#include <QVideoFrame>
#include <QLibraryInfo>

namespace SDK = SCRSDK;

BrqttCamera::BrqttCamera(QObject *parent) : QObject(parent),
    m_deviceHandle(0),
    m_model("Unknown"),
    m_connected(false),
    m_aperture(0),
    m_liveViewState(Unsupported),
    m_liveView(NULL),
    m_liveViewQuality(Low),
    m_imageSize(0, 0)
{
}

BrqttCamera::BrqttCamera(SCRSDK::ICrCameraObjectInfo const *camera_info, QObject *parent) : QObject(parent),
    m_deviceHandle(0),
    m_connected(false),
    m_aperture(0),
    m_liveViewState(Unsupported),
    m_liveView(NULL),
    m_liveViewQuality(Low),
    m_imageSize(0, 0)
{
    m_info = SDK::CreateCameraObjectInfo(
                camera_info->GetName(),
                camera_info->GetModel(),
                camera_info->GetUsbPid(),
                camera_info->GetIdType(),
                camera_info->GetIdSize(),
                camera_info->GetId(),
                camera_info->GetConnectionTypeName(),
                camera_info->GetAdaptorName(),
                camera_info->GetPairingNecessity()
                );

    // Extract the model from the real camera info
    setModel(QString::fromStdWString(m_info->GetModel()));

    connect(this, SIGNAL(onError(QString)), this, SLOT(debugErrorMessage(QString)));
    connect(this, SIGNAL(connectedChanged(bool)), this, SLOT(configureDeviceAndFetchProperties(bool)));
    connect(this, SIGNAL(onPropertyChanged()), this, SLOT(getProperties()));
    connect(this, SIGNAL(onLiveViewPropertyChanged()), this, SLOT(getLiveViewProperties()));
}

BrqttCamera::~BrqttCamera()
{
    delete m_liveView;
    m_info->Release();
}

QString BrqttCamera::model() const
{
    return m_model;
}

bool BrqttCamera::connectToDevice()
{
    if (m_info != NULL && !connected()) {
        qDebug() << "Trying to connect to device" << m_model;
        auto connect_status = SDK::Connect(m_info, this, &m_deviceHandle);

        if (CR_FAILED(connect_status)) {
            setConnected(false);

            qDebug() << "Failed to connect" << connect_status;

            emit failedToConnect();
            return false;
        }

        setSavePath();

        return true;
    }

    return false;
}

void BrqttCamera::setSavePath()
{
    std::wstring path = QDir::toNativeSeparators(QDir::currentPath()).toStdWString();

    qDebug() << "Setting save path to:" << path;

    CrChar prefix[] = L"BRQTTT";
    auto save_status = SDK::SetSaveInfo(m_deviceHandle, &path.front(), prefix, 1);

    if (CR_FAILED(save_status)) {
        emit onError(tr("Could not setup the path to save files"));
    }
}

void BrqttCamera::OnConnected(SCRSDK::DeviceConnectionVersioin version)
{
    Q_UNUSED(version)
    setConnected(true);
}

void BrqttCamera::OnDisconnected(CrInt32u error)
{
    qDebug() << "Camera disconnected";

    if (error == SDK::CrError_Connect_TimeOut) {
        qDebug() << "Device timed out";
    }

    setConnected(false);
    m_deviceHandle = 0;
}

void BrqttCamera::OnPropertyChanged()
{
    // According to the SDK documentation this callback should returns as quick as possible
    // because it is executed on the camera communication thread. Instead of retrieving the
    // properties here, this just emits a signal connected to a slot that will retrieve them.
    emit onPropertyChanged();
}

void BrqttCamera::OnLvPropertyChanged()
{
    emit onLiveViewPropertyChanged();
}

void BrqttCamera::OnCompleteDownload(CrChar *filename)
{
    emit downloadedPhoto(QString::fromStdWString(filename));
}

void BrqttCamera::OnWarning(CrInt32u warning)
{
    ///TODO: implement this
    qDebug() << "Warning: 0x" << std::hex << warning << std::dec << '\n';
}

void BrqttCamera::OnError(CrInt32u error)
{
    ///TODO: implement this
    qDebug() << "Error: 0x" << std::hex << error << std::dec << '\n';
}

QAbstractVideoSurface *BrqttCamera::videoSurface() const
{
    return m_liveView;
}

BrqttCamera::AspectRatio BrqttCamera::aspectRatio() const
{
    return m_aspectRatio;
}

BrqttCamera::LiveViewState BrqttCamera::liveViewState() const
{
    return m_liveViewState;
}

double BrqttCamera::aperture() const
{
    return m_aperture;
}

int BrqttCamera::shutterSpeed() const
{
    return m_shutterSpeed;
}

bool BrqttCamera::connected() const
{
    return m_connected;
}

void BrqttCamera::setModel(QString model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged(m_model);
}

void BrqttCamera::setConnected(bool connected)
{
    if (m_connected == connected)
        return;

    m_connected = connected;
    emit connectedChanged(m_connected);
}

void BrqttCamera::logError(int errorCode)
{
    switch (errorCode) {
    case SDK::CrError_Genric:
        emit onError(tr("Failed to get properties generic error"));
        break;

    case SDK::CrError_Api:
        emit onError(tr("Failed to get properties API error"));
        break;

    case SDK::CrError_File:
        emit onError(tr("Failed to get properties file error"));
        break;

    case SDK::CrError_Init:
        emit onError(tr("Failed to get properties init error"));
        break;

    case SDK::CrError_Memory:
        emit onError(tr("Failed to get properties memory error"));
        break;

    case SDK::CrError_Adaptor:
        emit onError(tr("Failed to get properties adaptor error"));
        break;

    case SDK::CrError_Connect:
        emit onError(tr("Failed to get properties connect error"));
        break;

    case SDK::CrError_Polling:
        emit onError(tr("Failed to get properties polling error"));
        break;

    case SDK::CrError_Application:
        emit onError(tr("Failed to get properties application error"));
        break;

    case SDK::CrError_Device:
        emit onError(tr("Failed to get properties device error"));
        break;

    case SDK::CrError_Genric_Abort:
        emit onError(tr("Failed to get properties abort error"));
        break;
    default:
        emit onError(tr("Failed to get properties %1").arg(errorCode));
        break;
    }
}

void BrqttCamera::getProperties()
{
    qDebug() << "Trying to get properties";
    std::int32_t nprop = 0;
    SDK::CrDeviceProperty* prop_list = nullptr;
    auto status = SDK::GetDeviceProperties(m_deviceHandle, &prop_list, &nprop);

    if (CR_FAILED(status)) {
        logError(status);
        return;
    }

    if (prop_list && nprop > 0) {
        for (std::int32_t i = 0; i < nprop; ++i) {
            auto prop = prop_list[i];

            if (!prop.IsGetEnableCurrentValue()) {
                if (prop.GetCode() == SDK::CrDevicePropertyCode::CrDeviceProperty_LiveViewStatus) {
                    qDebug() << "Can not read live view status";
                    setLiveViewState(LiveViewState::Unsupported);
                }
                continue;
            }

            switch (prop.GetCode()) {

            case SDK::CrDevicePropertyCode::CrDeviceProperty_FNumber:
                setAperture(static_cast<std::uint16_t>(prop.GetCurrentValue()) / 100.0);
                break;

            case SDK::CrDevicePropertyCode::CrDeviceProperty_LiveViewStatus:
                break;

            case SDK::CrDevicePropertyCode::CrDeviceProperty_ShutterSpeed:
                ///TODO: update shutter speed
                break;

            case SDK::CrDevicePropertyCode::CrDeviceProperty_IsoSensitivity:
                ///TODO: update iso
                break;

            case SDK::CrDevicePropertyCode::CrDeviceProperty_WhiteBalance:
                ///TODO: update white balance
                break;

            case SDK::CrDevicePropertyCode::CrDeviceProperty_BatteryLevel:
                ///TODO: update battery level
                break;

            case SDK::CrDevicePropertyCode::CrDeviceProperty_AspectRatio:
                setAspectRatio(AspectRatio(prop.GetCurrentValue()));
                ///TODO: update aspect ratio
                break;

            case SDK::CrDevicePropertyCode::CrDeviceProperty_DRO:
                ///TODO: update DRO
                break;

            case SDK::CrDevicePropertyCode::CrDeviceProperty_NearFar:
                ///TODO: update near far
                break;

            case SDK::CrDevicePropertyCode::CrDeviceProperty_LiveView_Image_Quality:
                // The SDK documentation says that the image size depends on the live view image quality and the aspect ratio
                // Since the image quality has changed here, this resets the image size
                m_imageSize = QSize(0, 0);
                m_liveViewQuality = LiveViewQuality(prop.GetCurrentValue());
                break;
            default:
                break;
            }
        }

        SDK::ReleaseDeviceProperties(m_deviceHandle, prop_list);
    } else {
        qDebug() << "No props";
    }
}

void BrqttCamera::getLiveViewProperties()
{
    CrInt32 num = 0;
    SDK::CrLiveViewProperty* property = NULL;
    auto err = SDK::GetLiveViewProperties(m_deviceHandle, &property, &num);
    if (CR_FAILED(err)) {
        emit onError(tr("Failed to get live view properties %1").arg(err));
        return;
    }

    if (!property->IsGetEnableCurrentValue()) {
        emit onError(tr("Cant read live view"));
        return;
    }

    for (std::int32_t i = 0; i < num; ++i) {
        auto prop = property[i];
    }

    SDK::ReleaseLiveViewProperties(m_deviceHandle, property);
}

void BrqttCamera::configureDeviceAndFetchProperties(bool connected)
{
    if (connected) {
        SDK::SetDeviceSetting(m_deviceHandle, SDK::SettingKey::Setting_Key_EnableLiveView, 1);
        setSavePath();
        getProperties();
    } else {
        setAperture(0);
    }
}

void BrqttCamera::setFocusFarNear(SDK::CrNearFarVal value)
{
    SDK::CrDeviceProperty * property = new SDK::CrDeviceProperty();
    property->SetCode(SDK::CrDevicePropertyCode::CrDeviceProperty_PriorityKeySettings);
    property->SetCurrentValue(SDK::CrPriorityKeySettings::CrPriorityKey_PCRemote);
    property->SetValueType(SDK::CrDataType::CrDataType_UInt32Array);
    auto err_priority = SDK::SetDeviceProperty(m_deviceHandle, property);

    delete property;

    if (CR_FAILED(err_priority)) {
        qDebug() << "Priority Key setting FAILED\n";
        return;
    }

    property = new SDK::CrDeviceProperty();

    property->SetCode(SDK::CrDevicePropertyCode::CrDeviceProperty_FocusMode);
    property->SetCurrentValue(0x0001);
    property->SetValueType(SDK::CrDataType::CrDataType_UInt16Array);

    auto err_focusMode = SDK::SetDeviceProperty(m_deviceHandle, property);

    delete property;
    if (CR_FAILED(err_focusMode)) {
        qDebug() << "Focus mode setting FAILED\n";
        return;
    }

    property = new SDK::CrDeviceProperty();

    property->SetCode(SDK::CrDevicePropertyCode::CrDeviceProperty_NearFar);
    property->SetCurrentValue(value);
    property->SetValueType(SDK::CrDataType::CrDataType_UInt16Array);

    SDK::SetDeviceProperty(m_deviceHandle, property);
}

void BrqttCamera::debugErrorMessage(QString error)
{
    qDebug() << "-------------------ERROR--------------------";
    qDebug() << error;
    qDebug() << "--------------------ERROR-------------------";
}

void BrqttCamera::getLiveView()
{
    if (QLibraryInfo::isDebugBuild()) {
        return;
    }

    if (connected()) {
        CrInt32 num = 0;
        SDK::CrLiveViewProperty* property = NULL;
        auto err = SDK::GetLiveViewProperties(m_deviceHandle, &property, &num);
        if (CR_FAILED(err)) {
            emit onError(tr("Failed to get live view properties %1").arg(err));
            return;
        }

        if (!property->IsGetEnableCurrentValue()) {
            emit onError(tr("Cant read live view"));
            return;
        }

        SDK::CrImageInfo inf;
        err = SDK::GetLiveViewImageInfo(m_deviceHandle, &inf);

        if (CR_FAILED(err)) {
            emit onError(tr("Failed to get live view image info %1").arg(err));
            return;
        }

        if (inf.GetBufferSize() > 0) {
            auto* image_data = new SDK::CrImageDataBlock();
            image_data->SetFrameNo(0);
            image_data->SetSize(inf.GetBufferSize());
            image_data->SetData(new CrInt8u[image_data->GetSize()]);

            err = SDK::GetLiveViewImage(m_deviceHandle, image_data);
            if (CR_FAILED(err)) {
                emit onError(tr("Failed to get live view image %1").arg(err));
                delete image_data;
            } else {
                if (0 < image_data->GetSize()) {
                    ///TODO: figure out how to construct a QImage without writting and reading from a file

                    // Get the real size from a file
                    std::wstring path = QDir::currentPath().toStdWString();
                    path.append(L"\\LiveView000000.JPG");

                    std::ofstream file(path, std::ios::out | std::ios::binary);
                    if (!file.bad()) {
                        file.write((char *)image_data->GetImageData(), image_data->GetImageSize());
                        file.close();
                    }

                    QString filePath = QString::fromStdWString(path);

                    QImage image = QImage();
                    image.load(filePath);
                    QFile* imageFile = new QFile(filePath);


                    imageFile->remove();
                    delete imageFile;

                    m_imageSize = image.size();

                    if (m_liveView != NULL) {
                        if (!m_liveView->isActive()) {
                            m_liveView->start(QVideoSurfaceFormat(m_imageSize, QVideoFrame::pixelFormatFromImageFormat(image.format())));
                        }

                        m_liveView->present(QVideoFrame(image));
                    }

                    if (m_liveView != NULL) {
                        if (m_liveView->isActive()) {
                            // If live view is active then reschedule the timer
                            QTimer* timer = new QTimer();

                            connect(timer, &QTimer::timeout, [=]() {
                                getLiveView();
                                timer->deleteLater();
                            } );

                            timer->start(33);
                        }
                    }

                    delete image_data;
                } else {
                    emit onError(tr("Failed to get live view"));
                    delete image_data;
                }
            }
        }
    } else {
        qDebug() << "Camera not connected";
    }
}

void BrqttCamera::setAperture(double aperture)
{
    if (qFuzzyCompare(m_aperture, aperture))
        return;

    m_aperture = aperture;
    emit apertureChanged(m_aperture);
}

void BrqttCamera::setShutterSpeed(int shutterSpeed)
{
    if (m_shutterSpeed == shutterSpeed)
        return;

    m_shutterSpeed = shutterSpeed;
    emit shutterSpeedChanged(m_shutterSpeed);
}

void BrqttCamera::setLiveViewState(BrqttCamera::LiveViewState liveViewState)
{
    qDebug() << "Setting live view to: " << liveViewState;
    if (m_liveViewState == liveViewState)
        return;

    m_liveViewState = liveViewState;
    emit liveViewStateChanged(m_liveViewState);
}


void BrqttCamera::takePhoto()
{
    SDK::SendCommand(m_deviceHandle, SDK::CrCommandId::CrCommandId_Release, SDK::CrCommandParam::CrCommandParam_Down);

    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);

    connect(timer, &QTimer::timeout, [=]() {
        SDK::SendCommand(m_deviceHandle, SDK::CrCommandId::CrCommandId_Release, SDK::CrCommandParam::CrCommandParam_Up);
        timer->deleteLater();
    } );

    timer->start(35);
}

void BrqttCamera::setFocusFar(FocusPrecision precision)
{
    switch (precision) {
    case BrqttCamera::SmallPrecision:
        setFocusFarNear(SDK::CrNearFarVal::CrNearFar_PLUS_S);
        break;
    case BrqttCamera::MediumPrecision:
        setFocusFarNear(SDK::CrNearFarVal::CrNearFar_PLUS_M);
        break;
    case BrqttCamera::LargePrecision:
        setFocusFarNear(SDK::CrNearFarVal::CrNearFar_PLUS_L);
        break;
    }
}

void BrqttCamera::setFocusNear(FocusPrecision precision)
{
    switch (precision) {
    case BrqttCamera::SmallPrecision:
        setFocusFarNear(SDK::CrNearFarVal::CrNearFar_MINUS_S);
        break;
    case BrqttCamera::MediumPrecision:
        setFocusFarNear(SDK::CrNearFarVal::CrNearFar_MINUS_M);
        break;
    case BrqttCamera::LargePrecision:
        setFocusFarNear(SDK::CrNearFarVal::CrNearFar_MINUS_L);
        break;
    }
}

void BrqttCamera::setVideoSurface(QAbstractVideoSurface *liveView)
{
    if (m_liveView == liveView)
        return;

    m_liveView = liveView;
    emit videoSurfaceChanged(m_liveView);

    QTimer* timer = new QTimer();

    connect(timer, &QTimer::timeout, [=]() {
        getLiveView();
        timer->deleteLater();
    } );

    timer->start(2000);
}

void BrqttCamera::setAspectRatio(BrqttCamera::AspectRatio aspectRatio)
{
    if (m_aspectRatio == aspectRatio)
        return;

    // The SDK documentation says that the image size depends on the live view image quality and the aspect ratio
    // Since the image quality has changed here, this resets the image size
    m_imageSize = QSize(0, 0);

    m_aspectRatio = aspectRatio;
    emit aspectRatioChanged(m_aspectRatio);
}
