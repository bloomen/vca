#include "SettingsBar.h"

namespace app
{
namespace handler
{

SettingsBar::SettingsBar()
    : Handler(42)
{
}

QString
SettingsBar::endpoint() const
{
    return "/settings/bar";
}

} // namespace handler
} // namespace app
