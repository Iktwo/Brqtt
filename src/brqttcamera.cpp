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

namespace SDK = SCRSDK;

BrqttCamera::BrqttCamera(QObject *parent) : QObject(parent),
    m_deviceHandle(0),
    m_model("Unknown"),
    m_connected(false),
    m_aperture(0),
    m_liveViewState(Unsupported)
{
}

BrqttCamera::BrqttCamera(SCRSDK::ICrCameraObjectInfo const *camera_info, QObject *parent) : QObject(parent),
    m_deviceHandle(0),
    m_connected(false),
    m_aperture(0),
    m_liveViewState(Unsupported)
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
    m_info->Release();
}

QString BrqttCamera::model() const
{
    return m_model;
}

bool BrqttCamera::connectToDevice()
{
    if (m_info != NULL) {
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
    qDebug() << "On warning" << warning;
}

void BrqttCamera::OnError(CrInt32u error)
{
    ///TODO: implement this
    qDebug() << "On error" << error;
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
    qDebug() << "getLiveViewProperties";
    qDebug() << "CrLiveViewProperty_LiveViewUndefined" << SDK::CrLiveViewPropertyCode::CrLiveViewProperty_LiveViewUndefined;
    qDebug() << "CrLiveViewProperty_AF_Area_Position" << SDK::CrLiveViewPropertyCode::CrLiveViewProperty_AF_Area_Position;
    qDebug() << "LiveViewMaxVal" << SDK::CrLiveViewPropertyCode::CrLiveViewProperty__LiveViewMaxVal;

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

        qDebug() << "lv Property" << prop.GetCode();

        if (prop.IsGetEnableCurrentValue()) {
            qDebug() << "lv Property value:" << prop.GetValue();
        } else {
            qDebug() << "Cant read lv property";
        }
    }

    SDK::ReleaseLiveViewProperties(m_deviceHandle, property);
}

void BrqttCamera::configureDeviceAndFetchProperties(bool connected)
{
    if (connected) {
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

        if (0 < inf.GetBufferSize()) {
            auto* image_data = new SDK::CrImageDataBlock();
            image_data->SetSize(inf.GetBufferSize());
            image_data->SetData(new CrInt8u[inf.GetBufferSize()]);

            err = SDK::GetLiveViewImage(m_deviceHandle, image_data);
            qDebug() << "Got live view image";
            if (CR_FAILED(err)) {
                emit onError(tr("Failed to get live view image %1").arg(err));
                delete image_data;
            } else {
                if (0 < image_data->GetSize()) {
                    std::wstring path = QDir::currentPath().toStdWString();
                    path.append(L"\\LiveView000000.JPG");

                    std::ofstream file(path, std::ios::out | std::ios::binary);
                    if (!file.bad()) {
                        file.write((char *)image_data->GetImageData(), image_data->GetImageSize());
                        file.close();
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

void BrqttCamera::setFocusFar()
{
    // CrNearFar_PLUS_L - 3 times
    // CrNearFar_PLUS_M -
    // CrNearFar_PLUS_S -
    setFocusFarNear(SDK::CrNearFarVal::CrNearFar_PLUS_L);
}

void BrqttCamera::setFocusNear()
{
    // CrNearFar_PLUS_L - 3 times
    // CrNearFar_PLUS_M -
    // CrNearFar_PLUS_S -
    setFocusFarNear(SDK::CrNearFarVal::CrNearFar_MINUS_M);
}
