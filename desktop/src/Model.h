#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

namespace app
{

class Model : public QObject
{
    Q_OBJECT
public:
    Model() = default;
    virtual ~Model() = default;

    Q_DISABLE_COPY(Model)

    virtual void
    update(const QString& endpoint,
           const QVariant& value,
           bool input = true) = 0;
    virtual QVariant
    get(const QString& endpoint) const = 0;
    virtual QVariant
    defaultValue(const QString& endpoint) const = 0;

    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void
        valueChanged(const QVariant& value, bool input) = 0;
    };

    virtual void
    addListener(const QString& endpoint, Listener& listener) = 0;
    virtual void
    removeListener(const QString& endpoint, Listener& listener) = 0;
};

} // namespace app
