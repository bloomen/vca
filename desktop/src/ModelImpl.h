#pragma once

#include <map>
#include <memory>
#include <set>
#include <vector>

#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QVariant>

#include "Handler.h"
#include "Model.h"

namespace app
{

class ModelImpl : public Model
{
    Q_OBJECT
public:
    ModelImpl() = default;

    void
    setHandlers(std::vector<std::unique_ptr<Handler>> handlers);

    Q_INVOKABLE void
    update(const QString& endpoint,
           const QVariant& value,
           bool input = true) override;
    Q_INVOKABLE QVariant
    get(const QString& endpoint) const override;
    Q_INVOKABLE QVariant
    defaultValue(const QString& endpoint) const override;

    void
    addListener(const QString& endpoint, Listener& listener) override;
    void
    removeListener(const QString& endpoint, Listener& listener) override;

private:
    std::map<QString, std::unique_ptr<Handler>>
        handlers_; // endpoint -> handler
    std::map<QString, std::set<Listener*>>
        listeners_; // endpoint -> set(listener)
};

} // namespace app
