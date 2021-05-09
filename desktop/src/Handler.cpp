#include "Handler.h"

namespace app
{

Handler::Handler(QVariant default_value)
    : m_default_value(default_value)
    , m_value(default_value)
{
}

void
Handler::setModel(Model& model)
{
    m_model = &model;
}

void
Handler::setValue(const QVariant& value)
{
    m_value = value;
}

const QVariant&
Handler::defaultValue() const
{
    return m_default_value;
}

const QVariant&
Handler::get() const
{
    return m_value;
}

void
Handler::publishChange(const QVariant& value)
{
    Q_ASSERT(m_model);
    m_model->update(endpoint(), value, false);
}

QVariant
Handler::defaultValue(const QString& endpoint) const
{
    Q_ASSERT(m_model);
    return m_model->defaultValue(endpoint);
}

QVariant
Handler::get(const QString& endpoint) const
{
    Q_ASSERT(m_model);
    return m_model->get(endpoint);
}

void
Handler::set(const QString& endpoint, const QVariant& variant)
{
    Q_ASSERT(m_model);
    m_model->update(endpoint, variant, false);
}

void
Handler::addListener(const QString& endpoint, Listener& listener)
{
    Q_ASSERT(m_model);
    m_model->addListener(endpoint, listener);
}

void
Handler::removeListener(const QString& endpoint, Listener& listener)
{
    Q_ASSERT(m_model);
    m_model->removeListener(endpoint, listener);
}

} // namespace app
