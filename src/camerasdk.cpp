#include "camerasdk.h"

#include <QDebug>
#include <iostream>
#include <string>
#include <QQmlListProperty>

#include "CrTypes.h"
#include "CameraRemote_SDK.h"

namespace SDK = SCRSDK;

CameraSDK::CameraSDK(QObject *parent) : QObject(parent), m_initialized(false), m_version("")
{
}

QString CameraSDK::version() const
{
    return m_version;
}

bool CameraSDK::initialized() const
{
    return m_initialized;
}

QQmlListProperty<BrqttCamera> CameraSDK::cameras()
{
    return QQmlListProperty<BrqttCamera>(this,
                                         nullptr,
                                         &CameraSDK::appendCamera,
                                         &CameraSDK::cameraCount,
                                         &CameraSDK::camera,
                                         &CameraSDK::clearCameras);
}

void CameraSDK::appendCamera(BrqttCamera * camera) {
    m_cameras.append(camera);
    emit camerasChanged();
}

void CameraSDK::appendCamera(QQmlListProperty<BrqttCamera>* list, BrqttCamera* camera)
{
    CameraSDK* cameraSDK = qobject_cast<CameraSDK*>(list->object);
    if (cameraSDK && camera) {
        cameraSDK->appendCamera(camera);
    }
}

int CameraSDK::cameraCount(QQmlListProperty<BrqttCamera>* list)
{
    CameraSDK* cameraSDK = qobject_cast<CameraSDK*>(list->object);
    if (cameraSDK) {
        return cameraSDK->m_cameras.count();
    }
    return 0;
}

BrqttCamera *CameraSDK::camera(QQmlListProperty<BrqttCamera>* list, int index)
{
    CameraSDK* cameraSDK = qobject_cast<CameraSDK*>(list->object);
    if (cameraSDK) {
        return cameraSDK->m_cameras.at(index);
    }
    return nullptr;
}

void CameraSDK::clearCameras(QQmlListProperty<BrqttCamera>* list)
{
    CameraSDK* cameraSDK = qobject_cast<CameraSDK*>(list->object);
    if (cameraSDK) {
        cameraSDK->m_cameras.clear();
        emit cameraSDK->camerasChanged();
    }
}

void CameraSDK::initializeSDK()
{
    CrInt32u version = SDK::GetSDKVersion();
    int major = (version & 0xFF000000) >> 24;
    int minor = (version & 0x00FF0000) >> 16;
    int patch = (version & 0x0000FF00) >> 8;
    int service = (version & 0x000000FF);

    setVersion(QStringLiteral("%1.%2.%3.%4").arg(major).arg(minor).arg(patch).arg(service));

    auto init_success = SDK::Init();

    if (!init_success) {
        failedToInitialize();
        SDK::Release();
    } else {
        setInitialized(true);
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
    SDK::ICrEnumCameraObjectInfo* camera_list = nullptr;
    auto enum_status = SDK::EnumCameraObjects(&camera_list);

    if (CR_FAILED(enum_status) || camera_list == nullptr) {
        return;
    }

    auto ncams = camera_list->GetCount();

    for (uint i = 0; i < ncams; ++i) {
        auto camera_info = camera_list->GetCameraObjectInfo(i);

        QString cameraModel = QString::fromStdWString(camera_info->GetModel());
        appendCamera(new BrqttCamera(cameraModel, this));
    }
}


