#pragma once

#include <vca/userdb.h>

#include "../Handler.h"

namespace app
{
namespace handler
{

class SearchInput : public Handler
{
public:
    SearchInput();

    QString
    endpoint() const override;

    void
    set_user_db(const vca::UserDb& user_db);

    void
    valueChanged(const QVariant& value, bool input) override;

private:
    const vca::UserDb* m_user_db = nullptr;
};

} // namespace handler
} // namespace app
