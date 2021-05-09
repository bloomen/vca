#include "SettingsFoo.h"

namespace app
{
namespace handler
{

SettingsFoo::SettingsFoo()
    : Handler(13)
{
}

QString
SettingsFoo::endpoint() const
{
    return "/settings/foo";
}

} // namespace handler
} // namespace app
