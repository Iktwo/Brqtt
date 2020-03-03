#ifndef BRQTTCAMERA_H
#define BRQTTCAMERA_H

#include <QObject>

class BrqttCamera : public QObject
{
    Q_OBJECT

public:
    explicit BrqttCamera(QObject *parent = nullptr);

    Q_PROPERTY(QString model READ model WRITE setModel NOTIFY modelChanged)

    QString model() const;

public slots:
    void setModel(QString model);

signals:
    void modelChanged(QString model);

private:
    std::atomic<bool> m_connected;
    QString m_model;
};

#endif // BRQTTCAMERA_H
