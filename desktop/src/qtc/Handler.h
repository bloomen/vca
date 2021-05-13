#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

#include "CommandQueue.h"
#include "Model.h"
#include "Node.h"
#include "ThreadPool.h"

namespace qtc
{

// All methods must be called on the main thread only, unless indicated
// otherwise.
class Handler
{
public:
    Handler(Model& model,
            ThreadPool& threadPool,
            CommandQueue& mainQueue,
            QString endpoint);

    virtual ~Handler() = default;

    Q_DISABLE_COPY(Handler)

    const QString&
    getEndpoint() const;

    // Connect handlers with each other.
    virtual void
    connect()
    {
    }

    const std::map<QString, std::unique_ptr<NodeBase>>&
    getNodes() const;

    NodeBase&
    getNode(const QString& key) const;

    const std::function<void(const QJsonObject&)>&
    getCommand(const QString& name) const;

protected:
    // Registers a new node at this handler.
    template <typename ValueType>
    Node<ValueType>&
    addNode(QString key,
            ValueType defaultValue,
            const bool actionable,
            const bool persistent)
    {
        auto node = new Node<ValueType>{mModel,
                                        *this,
                                        std::move(key),
                                        std::move(defaultValue),
                                        actionable,
                                        persistent};
        mNodes[node->getKey()] = std::unique_ptr<NodeBase>{node};
        return *node;
    }

    // Registers a new command at this handler
    void
    addCommand(QString name, std::function<void(const QJsonObject&)> command);

    // Executes some work asynchronously on another thread
    void
    execAsync(std::function<void()> functor);

    // Pushes a command onto the main queue. Can be called from any thread.
    void
    pushToMain(std::function<void()> functor);

    // Returns the handler at a specific endpoint.
    Handler&
    getHandler(const QString& endpoint) const;

private:
    Model& mModel;
    ThreadPool& mThreadPool;
    CommandQueue& mMainQueue;
    QString mEndpoint;
    std::map<QString, std::unique_ptr<NodeBase>> mNodes; // key -> node
    std::map<QString, std::function<void(const QJsonObject&)>>
        mCommands; // name -> command
};

} // namespace qtc
