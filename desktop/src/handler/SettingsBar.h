#pragma once

#include "../Handler.h"

namespace app
{
namespace handler
{

class SettingsBar : public Handler
{
public:
    SettingsBar();

    QString
    endpoint() const override;
};

} // namespace handler
} // namespace app
