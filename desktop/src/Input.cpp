#include "Input.h"
#include "ModelImpl.h"

namespace app
{

void
Input::componentComplete()
{
    QQuickItem::componentComplete();
    if (m_value.isValid())
    {
        setValue(m_value);
    }
}

Model*
Input::model() const
{
    return m_model;
}

void
Input::setModel(Model* model)
{
    Q_ASSERT(model);
    m_model = model;
}

const QString&
Input::endpoint() const
{
    return m_endpoint;
}

void
Input::setEndpoint(const QString& endpoint)
{
    m_endpoint = endpoint;
}

QVariant
Input::value() const
{
    if (isComponentComplete())
    {
        Q_ASSERT(m_model);
        return m_model->get(m_endpoint);
    }
    else
    {
        return m_value;
    }
}

void
Input::setValue(const QVariant& value)
{
    if (isComponentComplete())
    {
        Q_ASSERT(m_model);
        m_model->update(m_endpoint, value);
    }
    else
    {
        m_value = value;
    }
}

} // namespace app
