#include "Handler.h"

namespace qtc
{

Handler::Handler(Model& model,
                 ThreadPool& threadPool,
                 CommandQueue& mainQueue,
                 QString endpoint)
    : mModel{model}
    , mThreadPool{threadPool}
    , mMainQueue{mainQueue}
    , mEndpoint{std::move(endpoint)}
{
}

const QString&
Handler::getEndpoint() const
{
    return mEndpoint;
}

const std::map<QString, std::unique_ptr<NodeBase>>&
Handler::getNodes() const
{
    return mNodes;
}

NodeBase&
Handler::getNode(const QString& key) const
{
    Q_ASSERT(mNodes.find(key) != mNodes.end()); // key must exist
    return *mNodes.find(key)->second;
}

const std::function<void(const QJsonObject&)>&
Handler::getCommand(const QString& name) const
{
    Q_ASSERT(mCommands.find(name) != mCommands.end()); // name must exist
    return mCommands.find(name)->second;
}

void
Handler::addCommand(QString name,
                    std::function<void(const QJsonObject&)> command)
{
    Q_ASSERT(mCommands.find(name) ==
             mCommands.end()); // name must not exist yet
    mCommands[std::move(name)] = std::move(command);
}

void
Handler::execAsync(std::function<void()> functor)
{
    mThreadPool.push(std::move(functor));
}

void
Handler::pushToMain(std::function<void()> functor)
{
    mMainQueue.push(std::move(functor));
}

Handler&
Handler::getHandler(const QString& endpoint) const
{
    return mModel.getHandler(endpoint);
}

} // namespace qtc
