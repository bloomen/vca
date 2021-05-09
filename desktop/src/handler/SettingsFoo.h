#pragma once

#include "../Handler.h"

namespace app
{
namespace handler
{

class SettingsFoo : public Handler
{
public:
    SettingsFoo();

    QString
    endpoint() const override;
};

} // namespace handler
} // namespace app
