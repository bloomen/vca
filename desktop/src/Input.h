#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtQuick/QQuickItem>

#include "Model.h"

namespace app
{

class Input : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Model* model READ model WRITE setModel)
    Q_PROPERTY(QString endpoint READ endpoint WRITE setEndpoint)
    Q_PROPERTY(QVariant value READ value WRITE setValue)
public:
    Input() = default;

    Q_DISABLE_COPY(Input)

private:
    void
    componentComplete() override;

    Model*
    model() const;
    void
    setModel(Model* model);

    const QString&
    endpoint() const;
    void
    setEndpoint(const QString& endpoint);

    QVariant
    value() const;
    void
    setValue(const QVariant& value);

    Model* m_model{};
    QString m_endpoint;
    QVariant m_value;
};

} // namespace app
