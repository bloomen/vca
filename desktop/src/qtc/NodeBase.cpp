#include "NodeBase.h"

#include "Handler.h"
#include "Model.h"

namespace qtc
{

NodeBase::NodeBase(Model& model,
                   Handler& handler,
                   QString key,
                   QVariant defaultValue,
                   const bool actionable,
                   const bool persistent)
    : mModel{model}
    , mValue{defaultValue}
    , mHandler{handler}
    , mKey{std::move(key)}
    , mType{defaultValue.metaType()}
    , mDefaultValue{std::move(defaultValue)}
    , mActionable{actionable}
    , mPersistent{persistent}
{
    Q_ASSERT(mType.isValid());
}

Handler&
NodeBase::getHandler() const
{
    return mHandler;
}

const QString&
NodeBase::getKey() const
{
    return mKey;
}

QMetaType
NodeBase::getType() const
{
    return mType;
}

const QVariant&
NodeBase::getDefaultUnsafe() const
{
    return mDefaultValue;
}

const QVariant&
NodeBase::getValueUnsafe() const
{
    return mValue;
}

void
NodeBase::setValueUnsafe(QVariant value, const UpdateType updateType)
{
    Q_ASSERT(value.metaType() == mType);
    if (mValue != value)
    {
        mModel.trackValueChange(*this, value);
        mValue = std::move(value);
        for (const auto& listener : mListenersUnsafe)
        {
            (*listener)(*this, updateType);
        }
        valueChanged(updateType);
    }
}

void
NodeBase::assignDefault(const UpdateType updateType)
{
    setValueUnsafe(mDefaultValue, updateType);
}

bool
NodeBase::getActionable() const
{
    return mActionable;
}

bool
NodeBase::getPersistent() const
{
    return mPersistent;
}

const NodeBase::ListenerUnsafe*
NodeBase::addListenerUnsafe(ListenerUnsafe listener)
{
    listener(*this, UpdateType::INTERNAL);
    auto l = new ListenerUnsafe{std::move(listener)};
    mListenersUnsafe.emplace(l);
    return l;
}

void
NodeBase::removeListenerUnsafe(const ListenerUnsafe* listener)
{
    std::unique_ptr<const ListenerUnsafe> l{listener};
    mListenersUnsafe.erase(l);
    l.release();
}

} // namespace qtc
