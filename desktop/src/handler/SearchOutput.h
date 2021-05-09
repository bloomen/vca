#pragma once

#include "../Handler.h"

namespace app
{
namespace handler
{

class SearchOutput : public Handler
{
public:
    SearchOutput();

    QString
    endpoint() const override;
};

} // namespace handler
} // namespace app
