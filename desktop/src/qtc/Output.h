#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtQuick/QQuickItem>

#include "Model.h"

namespace qtc
{

class Output : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Model* model READ getModel WRITE setModel)
    Q_PROPERTY(QString endpoint READ getEndpoint WRITE setEndpoint)
    Q_PROPERTY(QString node READ getNode WRITE setNode)
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

    Model* mModel{};
    QString mEndpoint;
    QString mNode;
    const NodeBase::ListenerUnsafe* mListener{};
};

} // namespace qtc
