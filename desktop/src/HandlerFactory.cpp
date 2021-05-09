#include "HandlerFactory.h"
#include "Model.h"

#include "handler/SearchInput.h"
#include "handler/SearchOutput.h"

namespace
{

template <typename HandlerType>
HandlerType&
insertHandler(std::vector<std::unique_ptr<app::Handler>>& handlers,
              app::Model& model)
{
    auto handler = std::make_unique<HandlerType>();
    handler->setModel(model);
    auto handler_ptr = handler.get();
    handlers.push_back(std::move(handler));
    return *handler_ptr;
}

} // namespace

namespace app
{

std::vector<std::unique_ptr<Handler>>
makeHandlers(Model& model, const vca::UserDb& user_db)
{
    std::vector<std::unique_ptr<Handler>> handlers;
    auto& search_input = insertHandler<handler::SearchInput>(handlers, model);
    search_input.set_user_db(user_db);
    insertHandler<handler::SearchOutput>(handlers, model);
    return handlers;
}

} // namespace app
