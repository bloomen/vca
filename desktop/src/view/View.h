#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

namespace vca {

class View : public QObject
{
    Q_OBJECT
public:
    View() = default;

    Q_DISABLE_COPY(View)
};

}
