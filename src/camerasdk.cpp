#include "camerasdk.h"

#include <QDebug>
#include <iostream>
#include <string>

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
    /// TODO: Initialize SDK
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
