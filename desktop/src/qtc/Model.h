#pragma once

#include <deque>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include <QtCore/QJsonObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include "Action.h"
#include "NodeBase.h"

namespace qtc
{

class Handler;

QVariant
makeQmlValue(const QVariant& value);

class Model : public QObject
{
    Q_OBJECT
public:
    Model(const QString& organization, const QString& application);
    ~Model();

    Q_DISABLE_COPY(Model)

    void
    addHandler(std::unique_ptr<Handler> handler);

    void
    connectHandlers();

    Q_INVOKABLE void
    setValue(const QString& endpoint,
             const QString& key,
             const QVariant& value,
             UpdateType type = UpdateType::USER);
    Q_INVOKABLE void
    assignDefault(const QString& endpoint,
                  const QString& key,
                  UpdateType type = UpdateType::USER);
    Q_INVOKABLE QVariant
    getValue(const QString& endpoint, const QString& key) const;
    Q_INVOKABLE QVariant
    getDefault(const QString& endpoint, const QString& key) const;
    Q_INVOKABLE void
    execCommand(const QString& endpoint,
                const QString& name,
                const QJsonObject& params = {});

    Q_INVOKABLE void
    initialize();

    Q_INVOKABLE void
    startAction();
    Q_INVOKABLE void
    endAction();

    Q_INVOKABLE void
    undo();
    Q_INVOKABLE void
    redo();

    Handler&
    getHandler(const QString& endpoint) const;

    void
    trackValueChange(NodeBase& node, const QVariant& new_value);

    class IgnoreChanges
    {
    public:
        explicit IgnoreChanges(Model& model);
        ~IgnoreChanges();
        Q_DISABLE_COPY(IgnoreChanges)
    private:
        Model& mModel;
    };

private:
    bool mIgnoreChanges = true;
    bool mInitialized = false;
    std::map<QString, std::unique_ptr<Handler>>
        mHandlers; // endpoint -> handler
    std::unique_ptr<Action> mActiveAction;
    std::deque<Action> mUndoStack;
    std::deque<Action> mRedoStack;
    QSettings mSettings;
};

} // namespace qtc
