#ifndef CAMERASDK_H
#define CAMERASDK_H

#include <QObject>
#include <QList>
#include <QTimer>

#include "brqttcamera.h"

class CameraSDK : public QObject
{
    Q_OBJECT
public:
    explicit CameraSDK(QObject *parent = nullptr);
    ~CameraSDK();

    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(bool initialized READ initialized NOTIFY initializedChanged)
    Q_PROPERTY(BrqttCamera * camera READ camera NOTIFY cameraChanged)

    QString version() const;
    bool initialized() const;

    BrqttCamera * camera();

public slots:
    void initializeSDK();
    void setVersion(QString version);
    void setInitialized(bool initialized);

signals:
    void failedToInitialize();
    void versionChanged(QString version);
    void initializedChanged(bool initialized);
    void cameraChanged();
    void onError(QString message);
    void onWarning(QString message);

private:
    bool m_initialized;
    QString m_version;
    BrqttCamera * m_camera;

    void setCamera(BrqttCamera * camera);

private slots:
    void retrieveCameras();
};

#endif // CAMERASDK_H
