#include "Output.h"

#include "Handler.h"

namespace qtc
{

Output::~Output()
{
    if (mModel && mListener)
    {
        mModel->getHandler(mEndpoint).getNode(mNode).removeListenerUnsafe(
            mListener);
    }
}

void
Output::componentComplete()
{
    QQuickItem::componentComplete();
    Q_ASSERT(mModel);
    mListener = mModel->getHandler(mEndpoint).getNode(mNode).addListenerUnsafe(
        [this](const NodeBase& node, qtc::UpdateType) {
            mValue = makeQmlValue(node.getValueUnsafe());
            emit valueChanged(mValue);
        });
}

Model*
Output::getModel() const
{
    return mModel;
}

void
Output::setModel(Model* model)
{
    Q_ASSERT(model);
    mModel = model;
}

const QString&
Output::getEndpoint() const
{
    return mEndpoint;
}

void
Output::setEndpoint(const QString& endpoint)
{
    mEndpoint = endpoint;
}

const QString&
Output::getNode() const
{
    return mNode;
}

void
Output::setNode(const QString& node)
{
    mNode = node;
}

const QVariant&
Output::getValue() const
{
    return mValue;
}

} // namespace qtc
