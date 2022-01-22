#include "Model.h"

#include "Handler.h"

#include <vca/logging.h>

namespace qtc
{

namespace
{

void
limitActions(std::deque<Action>& actions)
{
    constexpr std::size_t limit = 100;
    while (actions.size() > limit)
    {
        actions.pop_front();
    }
}

} // namespace

QVariant
makeQmlValue(const QVariant& value)
{
    if (value.canConvert<QList<QVariant>>())
    {
        return value.value<QList<QVariant>>();
    }
    else
    {
        return value;
    }
}

Model::Model(const QString& organization, const QString& application)
    : mSettings{organization, application}
{
    VCA_INFO << "Settings: " << mSettings.fileName().toUtf8().data();
}

Model::~Model()
{
}

QString
Model::appName() const
{
    return mSettings.applicationName();
}

void
Model::addHandler(std::unique_ptr<Handler> handler)
{
    Q_ASSERT(mHandlers.find(handler->getEndpoint()) ==
             mHandlers.end()); // endpoint handler cannot exist yet
    mHandlers[handler->getEndpoint()] = std::move(handler);
}

void
Model::connectHandlers()
{
    for (const auto& pair : mHandlers)
    {
        pair.second->connect();
    }
}

void
Model::setValue(const QString& endpoint,
                const QString& key,
                const QVariant& value,
                UpdateType type)
{
    getHandler(endpoint).getNode(key).setValueUnsafe(value, type);
}

void
Model::assignDefault(const QString& endpoint,
                     const QString& key,
                     UpdateType type)
{
    getHandler(endpoint).getNode(key).assignDefault(type);
}

QVariant
Model::getValue(const QString& endpoint, const QString& key) const
{
    return makeQmlValue(getHandler(endpoint).getNode(key).getValueUnsafe());
}

QVariant
Model::getDefault(const QString& endpoint, const QString& key) const
{
    return makeQmlValue(getHandler(endpoint).getNode(key).getDefaultUnsafe());
}

void
Model::execCommand(const QString& endpoint,
                   const QString& name,
                   const QJsonObject& params)
{
    getHandler(endpoint).getCommand(name)(params);
}

void
Model::initialize()
{
    if (!mInitialized)
    {
        for (const auto& handlersPair : mHandlers)
        {
            const auto& handler = handlersPair.second;
            for (const auto& pair : handler->getNodes())
            {
                const auto fullPath = handler->getEndpoint() + "/" + pair.first;
                auto value =
                    mSettings.value(fullPath, pair.second->getDefaultUnsafe());
                if (!value.isValid())
                {
                    qCritical("Found invalid value: %s",
                              fullPath.toUtf8().data());
                    Q_ASSERT(false);
                    value = pair.second->getDefaultUnsafe();
                }
                if (!value.convert(pair.second->getType()))
                {
                    qCritical("Unable to convert settings value: %s",
                              fullPath.toUtf8().data());
                    Q_ASSERT(false);
                    value = pair.second->getDefaultUnsafe();
                }
                setValue(handler->getEndpoint(),
                         pair.first,
                         value,
                         UpdateType::INTERNAL);
            }
        }
        mIgnoreChanges = false;
        mInitialized = true;
    }
}

void
Model::startAction()
{
    if (!mActiveAction)
    {
        mActiveAction.reset(new Action);
    }
}

void
Model::endAction()
{
    if (mActiveAction)
    {
        mUndoStack.push_back(std::move(*mActiveAction.release()));
        limitActions(mUndoStack);
    }
}

void
Model::undo()
{
    endAction();
    if (mUndoStack.empty())
    {
        return;
    }
    auto lastAction = std::move(mUndoStack.back());
    mUndoStack.pop_back();
    IgnoreChanges ignoreChanges{*this};
    for (auto change = lastAction.mChanges.rbegin();
         change != lastAction.mChanges.rend();
         ++change)
    {
        setValue(change->mHandler.getEndpoint(),
                 change->mKey,
                 change->mOldValue,
                 UpdateType::INTERNAL);
    }
    mRedoStack.push_back(std::move(lastAction));
    limitActions(mRedoStack);
}

void
Model::redo()
{
    if (mRedoStack.empty())
    {
        return;
    }
    auto lastAction = std::move(mRedoStack.back());
    mRedoStack.pop_back();
    IgnoreChanges ignore_changes{*this};
    for (auto change = lastAction.mChanges.begin();
         change != lastAction.mChanges.end();
         ++change)
    {
        setValue(change->mHandler.getEndpoint(),
                 change->mKey,
                 change->mNewValue,
                 UpdateType::INTERNAL);
    }
    mUndoStack.push_back(std::move(lastAction));
    limitActions(mUndoStack);
}

Handler&
Model::getHandler(const QString& endpoint) const
{
    Q_ASSERT(mHandlers.find(endpoint) !=
             mHandlers.end()); // endpoint handler must exist
    return *mHandlers.find(endpoint)->second;
}

void
Model::trackValueChange(NodeBase& node, const QVariant& newValue)
{
    Q_ASSERT(newValue.isValid());
    if (!mInitialized)
    {
        return;
    }
    if (node.getActionable() && !mIgnoreChanges)
    {
        const bool groupingChanges = mActiveAction != nullptr;
        if (!groupingChanges)
        {
            startAction();
        }
        mActiveAction->mChanges.push_back({node.getHandler(),
                                           node.getKey(),
                                           node.getValueUnsafe(),
                                           newValue});
        if (!groupingChanges)
        {
            endAction();
        }
    }

    if (node.getPersistent())
    {
        const auto fullPath =
            node.getHandler().getEndpoint() + "/" + node.getKey();
        if (newValue == node.getDefaultUnsafe())
        {
            mSettings.remove(fullPath);
        }
        else
        {
            mSettings.setValue(fullPath, newValue);
        }
    }
}

Model::IgnoreChanges::IgnoreChanges(Model& model)
    : mModel{model}
{
    mModel.mIgnoreChanges = true;
}
Model::IgnoreChanges::~IgnoreChanges()
{
    mModel.mIgnoreChanges = false;
}

} // namespace qtc
