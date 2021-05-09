#include "Input.h"
#include "ModelImpl.h"

namespace app
{

void
Input::componentComplete()
{
    QQuickItem::componentComplete();
    if (value_.isValid())
    {
        setValue(value_);
    }
}

Model*
Input::model() const
{
    return model_;
}

void
Input::setModel(Model* model)
{
    Q_ASSERT(model);
    model_ = model;
}

const QString&
Input::endpoint() const
{
    return endpoint_;
}

void
Input::setEndpoint(const QString& endpoint)
{
    endpoint_ = endpoint;
}

QVariant
Input::value() const
{
    if (isComponentComplete())
    {
        Q_ASSERT(model_);
        return model_->get(endpoint_);
    }
    else
    {
        return value_;
    }
}

void
Input::setValue(const QVariant& value)
{
    if (isComponentComplete())
    {
        Q_ASSERT(model_);
        model_->update(endpoint_, value);
    }
    else
    {
        value_ = value;
    }
}

} // namespace app
