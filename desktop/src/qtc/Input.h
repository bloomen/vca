#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtQuick/QQuickItem>

#include "Model.h"

namespace qtc
{

class Input : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Model* model READ getModel WRITE setModel)
    Q_PROPERTY(QString endpoint READ getEndpoint WRITE setEndpoint)
    Q_PROPERTY(QString node READ getNode WRITE setNode)
    Q_PROPERTY(QVariant value READ getValue WRITE setValue)
public:
    Input() = default;

    Q_DISABLE_COPY(Input)

private:
    Model*
    getModel() const;
    void
    setModel(Model* model);

    const QString&
    getEndpoint() const;
    void
    setEndpoint(const QString& endpoint);

    const QString&
    getNode() const;
    void
    setNode(const QString& node);

    QVariant
    getValue() const;
    void
    setValue(const QVariant& value);

    Model* mModel{};
    QString mEndpoint;
    QString mNode;
    QVariant mValue;
};

} // namespace qtc
