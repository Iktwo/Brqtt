#include "brqttcamera.h"

#include "CameraRemote_SDK.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>

#include <QDebug>
#include <QDir>

namespace SDK = SCRSDK;

BrqttCamera::BrqttCamera(QObject *parent) : QObject(parent),
    m_device_handle(0),
    m_connected(false),
    m_model("Unknown")
{
}

BrqttCamera::BrqttCamera(SCRSDK::ICrCameraObjectInfo const *camera_info, QObject *parent) : QObject(parent),
    m_device_handle(0),
    m_connected(false)
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

    setModel(QString::fromStdWString(m_info->GetModel()));
}

BrqttCamera::~BrqttCamera()
{
    m_info->Release();
}

QString BrqttCamera::model() const
{
    return m_model;
}

bool BrqttCamera::connect()
{
    auto connect_status = SDK::Connect(m_info, this, &m_device_handle);
    if (CR_FAILED(connect_status)) {
        setConnected(false);
        emit failedToConnect();
        return false;
    }

    setConnected(true);
    setSavePath();
    return true;
}

void BrqttCamera::setSavePath()
{
    std::wstring path = QDir::currentPath().toStdWString();

    CrChar prefix[] = L"BRQTTT";
    auto save_status = SDK::SetSaveInfo(m_device_handle, &path.front(), prefix, 1);
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
    Q_UNUSED(error)
    setConnected(false);
}

void BrqttCamera::OnPropertyChanged()
{
    ///TODO: implement this
}

void BrqttCamera::OnLvPropertyChanged()
{
    ///TODO: implement this
}

void BrqttCamera::OnCompleteDownload(CrChar *filename)
{
    ///TODO: implement this
}

void BrqttCamera::OnWarning(CrInt32u warning)
{
    ///TODO: implement this
}

void BrqttCamera::OnError(CrInt32u error)
{
    ///TODO: implement this
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
