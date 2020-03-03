#include "brqttcamera.h"

BrqttCamera::BrqttCamera(QObject *parent) : QObject(parent)
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
