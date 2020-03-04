#ifndef BRQTTCAMERA_H
#define BRQTTCAMERA_H

#include <QObject>

#include "CameraRemote_SDK.h"
#include "IDeviceCallback.h"

class BrqttCamera : public QObject, public SCRSDK::IDeviceCallback
{
    Q_OBJECT

public:
    explicit BrqttCamera(QObject *parent = nullptr);
    BrqttCamera(SCRSDK::ICrCameraObjectInfo const *camera_info, QObject *parent = nullptr);
    ~BrqttCamera();

    Q_PROPERTY(QString model READ model NOTIFY modelChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)

    QString model() const;
    bool connected() const;
    void setSavePath();

    // IDeviceCallback
    void OnConnected(SCRSDK::DeviceConnectionVersioin version) override;
    void OnDisconnected(CrInt32u error) override;
    void OnPropertyChanged() override;
    void OnLvPropertyChanged() override;
    void OnCompleteDownload(CrChar* filename) override;
    void OnWarning(CrInt32u warning) override;
    void OnError(CrInt32u error) override;

public slots:
    bool connect();

signals:
    void modelChanged(QString model);
    void connectedChanged(bool connected);
    void failedToConnect();
    void onError(QString errorMessage);

private:
    void setModel(QString model);
    void setConnected(bool connected);

    SCRSDK::ICrCameraObjectInfo* m_info;
    std::int64_t m_device_handle;
    std::atomic<bool> m_connected;
    QString m_model;
};

#endif // BRQTTCAMERA_H
