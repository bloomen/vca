#pragma once

#include <memory>
#include <vector>

namespace app
{

class Handler;
class Model;

std::vector<std::unique_ptr<Handler>>
makeHandlers(Model& model);

} // namespace app
