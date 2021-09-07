#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

namespace vca
{

class View : public QObject
{
    Q_OBJECT
public:
    View() = default;

    Q_DISABLE_COPY(View)

    Q_INVOKABLE QList<QString>
    getHostDirectories(const QString& host, const QString& port) const;
};

} // namespace vca
