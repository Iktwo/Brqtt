#ifndef CAMERASDK_H
#define CAMERASDK_H

#include <QObject>
#include <QList>
#include <QQmlListProperty>

#include "brqttcamera.h"

class CameraSDK : public QObject
{
    Q_OBJECT
public:
    explicit CameraSDK(QObject *parent = nullptr);
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(bool initialized READ initialized NOTIFY initializedChanged)
    Q_PROPERTY(QQmlListProperty<BrqttCamera> cameras READ cameras NOTIFY camerasChanged)

    QString version() const;
    bool initialized() const;

    QQmlListProperty<BrqttCamera> cameras();

    void appendCamera(BrqttCamera*);
    static void appendCamera(QQmlListProperty<BrqttCamera>*, BrqttCamera*);
    static int cameraCount(QQmlListProperty<BrqttCamera>*);
    static BrqttCamera *camera(QQmlListProperty<BrqttCamera>*, int);
    static void clearCameras(QQmlListProperty<BrqttCamera>*);

public slots:
    void initializeSDK();
    void setVersion(QString version);
    void setInitialized(bool initialized);

signals:
    void failedToInitialize();
    void versionChanged(QString version);
    void initializedChanged(bool initialized);

    void camerasChanged();

private:
    bool m_initialized;
    QString m_version;

    void retrieveCameras();
    QList<BrqttCamera *> m_cameras;
};

#endif // CAMERASDK_H
