#pragma once

#include "NodeBase.h"

Q_DECLARE_METATYPE(QList<bool>)
Q_DECLARE_METATYPE(QList<double>)
Q_DECLARE_METATYPE(QList<qint32>)
Q_DECLARE_METATYPE(QList<quint32>)
Q_DECLARE_METATYPE(QList<qint64>)
Q_DECLARE_METATYPE(QList<quint64>)
Q_DECLARE_METATYPE(QList<QString>)

namespace qtc
{

template <typename ValueType>
class Node : public NodeBase
{
public:
    using Listener = std::function<void(const Node&, UpdateType)>;

    Node(Model& model,
         Handler& handler,
         QString key,
         ValueType defaultValue,
         const bool actionable,
         const bool persistent)
        : NodeBase{model,
                   handler,
                   std::move(key),
                   QVariant::fromValue(std::move(defaultValue)),
                   actionable,
                   persistent}
    {
    }

    Q_DISABLE_COPY(Node)

    ValueType
    getDefault() const
    {
        const auto v = getDefaultUnsafe();
        Q_ASSERT(v.template canConvert<ValueType>());
        return v.template value<ValueType>();
    }

    ValueType
    getValue() const
    {
        const auto v = getValueUnsafe();
        Q_ASSERT(v.template canConvert<ValueType>());
        return v.template value<ValueType>();
    }

    void
    setValue(ValueType value,
             const UpdateType updateType = UpdateType::INTERNAL)
    {
        setValueUnsafe(QVariant::fromValue(std::move(value)), updateType);
    }

    const Listener*
    addListener(Listener listener)
    {
        listener(*this, UpdateType::INTERNAL);
        auto l = new Listener{std::move(listener)};
        mListeners.emplace(l);
        return l;
    }

    void
    removeListener(const Listener* listener)
    {
        std::unique_ptr<const Listener> l{listener};
        mListeners.erase(l);
        l.release();
    }

private:
    void
    valueChanged(const UpdateType updateType) override
    {
        for (const auto& listener : mListeners)
        {
            (*listener)(*this, updateType);
        }
    }

    std::set<std::unique_ptr<const Listener>> mListeners;
};

} // namespace qtc
