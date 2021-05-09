#pragma once

#include <QtCore/QPointer>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtQuick/QQuickItem>

#include "Model.h"

namespace app
{

class Output : public QQuickItem, public Model::Listener
{
    Q_OBJECT
    Q_PROPERTY(Model* model READ model WRITE setModel)
    Q_PROPERTY(QString endpoint READ endpoint WRITE setEndpoint)
public:
    Output() = default;
    ~Output();

    Q_DISABLE_COPY(Output)

signals:
    void
    updated(const QVariant& value);

private:
    void
    componentComplete() override;

    void
    valueChanged(const QVariant& value, bool) override;

    Model*
    model() const;
    void
    setModel(Model* model);

    const QString&
    endpoint() const;
    void
    setEndpoint(const QString& endpoint);

    Model* model_{};
    QString endpoint_;
};

} // namespace app
