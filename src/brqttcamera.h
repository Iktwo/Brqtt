#ifndef BRQTTCAMERA_H
#define BRQTTCAMERA_H

#include <QObject>

#include "CameraRemote_SDK.h"
#include "IDeviceCallback.h"

class BrqttCamera : public QObject, public SCRSDK::IDeviceCallback
{
    Q_OBJECT
    Q_ENUMS(LiveViewState)

public:
    explicit BrqttCamera(QObject *parent = nullptr);
    BrqttCamera(SCRSDK::ICrCameraObjectInfo const *camera_info, QObject *parent = nullptr);
    ~BrqttCamera();

    Q_PROPERTY(QString model READ model NOTIFY modelChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(double aperture READ aperture NOTIFY apertureChanged)
    Q_PROPERTY(int shutterSpeed READ shutterSpeed NOTIFY shutterSpeedChanged)
    Q_PROPERTY(LiveViewState liveViewState READ liveViewState NOTIFY liveViewStateChanged)

    enum LiveViewState { Disabled = 0, Enabled = 1, Unsupported = 2 };

    Q_ENUM(LiveViewState)

    QString model() const;
    bool connected() const;
    double aperture() const;
    int shutterSpeed() const;
    void setSavePath();
    LiveViewState liveViewState() const;

    // IDeviceCallback
    void OnConnected(SCRSDK::DeviceConnectionVersioin version) override;
    void OnDisconnected(CrInt32u error) override;
    void OnPropertyChanged() override;
    void OnLvPropertyChanged() override;
    void OnCompleteDownload(CrChar* filename) override;
    void OnWarning(CrInt32u warning) override;
    void OnError(CrInt32u error) override;

public slots:
    bool connectToDevice();
    void getLiveView();
    void setAperture(double aperture);
    void setShutterSpeed(int shutterSpeed);
    void setLiveViewState(LiveViewState liveViewState);
    void takePhoto();
    void setFocusFar();
    void setFocusNear();

signals:
    void modelChanged(QString model);
    void connectedChanged(bool connected);
    void failedToConnect();
    void onError(QString errorMessage);
    void apertureChanged(double aperture);
    void shutterSpeedChanged(int shutterSpeed);
    void onPropertyChanged();
    void onLiveViewPropertyChanged();
    void liveViewStateChanged(LiveViewState liveViewState);
    void downloadedPhoto(QString filePath);

private:
    void setModel(QString model);
    void setConnected(bool connected);
    void logError(int errorCode);

    SCRSDK::ICrCameraObjectInfo* m_info;
    std::int64_t m_deviceHandle;

    // Q_PROPERTY variables
    QString m_model;
    std::atomic<bool> m_connected;
    double m_aperture;
    int m_shutterSpeed;
    LiveViewState m_liveViewState;

private slots:
    void debugErrorMessage(QString error);
    void getProperties();
    void getLiveViewProperties();
    void configureDeviceAndFetchProperties(bool connected);
    void setFocusFarNear(SCRSDK::CrNearFarVal value);
};

#endif // BRQTTCAMERA_H
