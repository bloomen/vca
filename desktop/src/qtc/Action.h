#pragma once

#include <list>

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace qtc
{

class Handler;

class Action
{
public:
    Action() = default;

    Q_DISABLE_COPY(Action)

    Action(Action&&) = default;
    Action&
    operator=(Action&&) = default;

    struct Change
    {
        Handler& mHandler;
        QString mKey;
        QVariant mOldValue;
        QVariant mNewValue;
    };

    std::list<Change> mChanges;
};

} // namespace qtc
