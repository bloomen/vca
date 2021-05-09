#include "Handler.h"

namespace app
{

Handler::Handler(QVariant default_value)
    : default_value_(default_value)
    , value_(default_value)
{
}

void
Handler::setModel(Model& model)
{
    model_ = &model;
}

void
Handler::setValue(const QVariant& value)
{
    value_ = value;
}

const QVariant&
Handler::defaultValue() const
{
    return default_value_;
}

const QVariant&
Handler::get() const
{
    return value_;
}

void
Handler::publishChange(const QVariant& value)
{
    Q_ASSERT(model_);
    model_->update(endpoint(), value, false);
}

QVariant
Handler::defaultValue(const QString& endpoint) const
{
    Q_ASSERT(model_);
    return model_->defaultValue(endpoint);
}

QVariant
Handler::get(const QString& endpoint) const
{
    Q_ASSERT(model_);
    return model_->get(endpoint);
}

void
Handler::addListener(const QString& endpoint, Listener& listener)
{
    Q_ASSERT(model_);
    model_->addListener(endpoint, listener);
}

void
Handler::removeListener(const QString& endpoint, Listener& listener)
{
    Q_ASSERT(model_);
    model_->removeListener(endpoint, listener);
}

} // namespace app
