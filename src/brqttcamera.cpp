#include "brqttcamera.h"

BrqttCamera::BrqttCamera(QObject *parent) : QObject(parent), m_model("Unknown")
{
}

BrqttCamera::BrqttCamera(QString model, QObject *parent) : QObject(parent), m_model(model)
{
}

QString BrqttCamera::model() const
{
    return m_model;
}

void BrqttCamera::setModel(QString model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit modelChanged(m_model);
}
