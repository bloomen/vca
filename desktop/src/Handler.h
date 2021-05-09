#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

#include "Model.h"

namespace app
{

class Handler : public Model::Listener
{
public:
    explicit Handler(QVariant default_value);

    virtual ~Handler() = default;

    Q_DISABLE_COPY(Handler)

    virtual QString
    endpoint() const = 0;

    // Only called during setup.
    void
    setModel(Model& model);

    // Should only be called by the model.
    void
    setValue(const QVariant& value);

    const QVariant&
    defaultValue() const;

    const QVariant&
    get() const;

protected:
    // Publishes a new value to the model corresponding to this handler.
    void
    publishChange(const QVariant& value);

    // Called when this handler's value was changed. `input` denotes
    // whether the change was triggered by user input.
    void
    valueChanged(const QVariant& /*value*/, bool /*input*/) override
    {
    }

    // Returns the default value of a specific endpoint.
    QVariant
    defaultValue(const QString& endpoint) const;

    // Returns the current value of a specific endpoint.
    QVariant
    get(const QString& endpoint) const;

    // Adds a new listener to the model.
    void
    addListener(const QString& endpoint, Listener& listener);

    // Removes an existing listener from the model.
    void
    removeListener(const QString& endpoint, Listener& listener);

private:
    Model* model_{};
    QVariant default_value_;
    QVariant value_;
};

} // namespace app
