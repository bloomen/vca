#include "Output.h"
#include "ModelImpl.h"

namespace app
{

Output::~Output()
{
    Q_ASSERT(model_);
    model_->removeListener(endpoint_, *this);
}

void
Output::componentComplete()
{
    QQuickItem::componentComplete();
    Q_ASSERT(model_);
    model_->addListener(endpoint_, *this);
}

void
Output::valueChanged(const QVariant& value, bool)
{
    emit updated(value);
}

Model*
Output::model() const
{
    return model_;
}

void
Output::setModel(Model* model)
{
    Q_ASSERT(model);
    model_ = model;
}

const QString&
Output::endpoint() const
{
    return endpoint_;
}

void
Output::setEndpoint(const QString& endpoint)
{
    endpoint_ = endpoint;
}

} // namespace app
