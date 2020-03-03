#ifndef CAMERASDK_H
#define CAMERASDK_H

#include <QObject>

#include "brqttcamera.h"

class CameraSDK : public QObject
{
    Q_OBJECT
public:
    explicit CameraSDK(QObject *parent = nullptr);
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(bool initialized READ initialized WRITE setInitialized NOTIFY initializedChanged)

    QString version() const;
    bool initialized() const;

public slots:
    void initializeSDK();
    void setVersion(QString version);
    void setInitialized(bool initialized);

signals:
    void failedToInitialize();
    void versionChanged(QString version);
    void initializedChanged(bool initialized);

private:
    bool m_initialized;
    QString m_version;

    void retrieveCameras();
};

#endif // CAMERASDK_H
