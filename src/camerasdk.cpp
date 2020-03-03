#include "camerasdk.h"

#include <QDebug>
#include <iostream>
#include <string>

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
    /// TODO: Retrieve cameras
}
