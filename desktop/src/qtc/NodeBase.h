#pragma once

#include <functional>
#include <memory>
#include <set>

#include <QtCore/QString>
#include <QtCore/QVariant>

#include "UpdateType.h"

namespace qtc
{

class Handler;
class Model;

class NodeBase
{
public:
    using ListenerUnsafe = std::function<void(const NodeBase&, UpdateType)>;

    NodeBase(Model& model,
             Handler& handler,
             QString key,
             QVariant defaultValue,
             bool actionable,
             bool persistent);

    virtual ~NodeBase() = default;

    Q_DISABLE_COPY(NodeBase)

    Handler&
    getHandler() const;

    const QString&
    getKey() const;

    QMetaType
    getType() const;

    const QVariant&
    getDefaultUnsafe() const;

    const QVariant&
    getValueUnsafe() const;

    void
    setValueUnsafe(QVariant value,
                   UpdateType updateType = UpdateType::INTERNAL);

    void
    assignDefault(UpdateType updateType = UpdateType::INTERNAL);

    bool
    getActionable() const;

    bool
    getPersistent() const;

    const ListenerUnsafe*
    addListenerUnsafe(ListenerUnsafe listener);

    void
    removeListenerUnsafe(const ListenerUnsafe* listener);

private:
    virtual void valueChanged(UpdateType /*updateType*/)
    {
    }

    Model& mModel;
    QVariant mValue;
    Handler& mHandler;
    QString mKey;
    QMetaType mType;
    QVariant mDefaultValue;
    bool mActionable;
    bool mPersistent;
    std::set<std::unique_ptr<const ListenerUnsafe>> mListenersUnsafe;
};

} // namespace qtc
