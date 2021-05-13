#include "Input.h"

namespace qtc
{

Model*
Input::getModel() const
{
    return mModel;
}

void
Input::setModel(Model* model)
{
    Q_ASSERT(model);
    mModel = model;
}

const QString&
Input::getEndpoint() const
{
    return mEndpoint;
}

void
Input::setEndpoint(const QString& endpoint)
{
    mEndpoint = endpoint;
}

const QString&
Input::getNode() const
{
    return mNode;
}

void
Input::setNode(const QString& node)
{
    mNode = node;
}

QVariant
Input::getValue() const
{
    if (isComponentComplete())
    {
        Q_ASSERT(mModel);
        return mModel->getValue(mEndpoint, mNode);
    }
    else
    {
        return mValue;
    }
}

void
Input::setValue(const QVariant& value)
{
    if (isComponentComplete())
    {
        Q_ASSERT(mModel);
        mModel->setValue(mEndpoint, mNode, value);
    }
    else
    {
        mValue = value;
    }
}

} // namespace qtc
