#include "camerasdk.h"

#include <iostream>
#include <string>
#include <QTimer>
#include <QDebug>

#include "CrTypes.h"
#include "CameraRemote_SDK.h"

namespace SDK = SCRSDK;

CameraSDK::CameraSDK(QObject *parent) : QObject(parent),
    m_initialized(false),
    m_version(""),
    m_camera(NULL)
{
    initializeSDK();
}

CameraSDK::~CameraSDK()
{
    SDK::Release();
}

QString CameraSDK::version() const
{
    return m_version;
}

bool CameraSDK::initialized() const
{
    return m_initialized;
}

BrqttCamera * CameraSDK::camera()
{
    return m_camera;
}

void CameraSDK::initializeSDK()
{
    auto init_success = SDK::Init(0);

    if (!init_success) {
        failedToInitialize();
        SDK::Release();
    } else {
        setInitialized(true);

        CrInt32u version = SDK::GetSDKVersion();
        int major = (version & 0xFF000000) >> 24;
        int minor = (version & 0x00FF0000) >> 16;
        int patch = (version & 0x0000FF00) >> 8;
        int service = (version & 0x000000FF);

        setVersion(QStringLiteral("%1.%2.%3.%4").arg(major).arg(minor).arg(patch).arg(service));

        retrieveCameras();
    }
}

void CameraSDK::setVersion(QString version){
    if (m_version == version)
        return;

    m_version = version;
    emit versionChanged(m_version);
}

void CameraSDK::setInitialized(bool initialized)
{
    if (m_initialized == initialized)
        return;

    m_initialized = initialized;
    emit initializedChanged(m_initialized);
}

void CameraSDK::retrieveCameras()
{
    if (m_camera == NULL) {
        SDK::ICrEnumCameraObjectInfo* camera_list = nullptr;
        auto enum_status = SDK::EnumCameraObjects(&camera_list);

        if (CR_FAILED(enum_status) || camera_list == nullptr) {
            return;
        }

        auto ncams = camera_list->GetCount();

        if (ncams > 1) {
            emit onWarning(tr("More than one camera detected. The application will attempt to connect to the first one"));
        }

        if (ncams > 0) {
            auto cameraInfo = camera_list->GetCameraObjectInfo(0);

            if (m_camera != NULL) {
                delete m_camera;
            }

            setCamera(new BrqttCamera(cameraInfo, this));
        } else {
            emit onWarning(tr("No cameras detected"));
        }

        camera_list->Release();
    }
}

void CameraSDK::setCamera(BrqttCamera *camera)
{
    m_camera = camera;
    emit cameraChanged();
}


