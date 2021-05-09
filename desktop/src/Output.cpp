#include "Output.h"
#include "ModelImpl.h"

namespace app
{

Output::~Output()
{
    Q_ASSERT(m_model);
    m_model->removeListener(m_endpoint, *this);
}

void
Output::componentComplete()
{
    QQuickItem::componentComplete();
    Q_ASSERT(m_model);
    m_model->addListener(m_endpoint, *this);
}

void
Output::valueChanged(const QVariant& value, bool)
{
    emit updated(value);
}

Model*
Output::model() const
{
    return m_model;
}

void
Output::setModel(Model* model)
{
    Q_ASSERT(model);
    m_model = model;
}

const QString&
Output::endpoint() const
{
    return m_endpoint;
}

void
Output::setEndpoint(const QString& endpoint)
{
    m_endpoint = endpoint;
}

} // namespace app
