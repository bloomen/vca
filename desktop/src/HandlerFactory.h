#pragma once

#include <memory>
#include <vector>

namespace vca
{
class UserDb;
}

namespace app
{

class Handler;
class Model;

std::vector<std::unique_ptr<Handler>>
makeHandlers(Model& model, const vca::UserDb& user_db);

} // namespace app
