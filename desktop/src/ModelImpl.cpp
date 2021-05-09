#include "ModelImpl.h"

namespace app
{

void
ModelImpl::setHandlers(std::vector<std::unique_ptr<Handler>> handlers)
{
    Q_ASSERT(handlers_.empty());
    for (auto& handler : handlers)
    {
        Q_ASSERT(handlers_.find(handler->endpoint()) ==
                 handlers_.end()); // endpoint handler cannot exist yet
        handlers_[handler->endpoint()] = std::move(handler);
    }
    for (auto& handler_pair : handlers_)
    {
        addListener(handler_pair.first, *(handler_pair.second));
    }
}

void
ModelImpl::update(const QString& endpoint, const QVariant& value, bool input)
{
    auto handler_pair = handlers_.find(endpoint);
    if (handler_pair != handlers_.end())
    {
        auto& handler = handler_pair->second;
        if (value != handler->get())
        {
            handler->setValue(value);
            auto listener_pair = listeners_.find(endpoint);
            if (listener_pair != listeners_.end())
            {
                for (auto listener : listener_pair->second)
                {
                    listener->valueChanged(value, input);
                }
            }
        }
    }
    else
    {
        Q_ASSERT(false); // endpoint must be registered
    }
}

QVariant
ModelImpl::defaultValue(const QString& endpoint) const
{
    auto handler_pair = handlers_.find(endpoint);
    if (handler_pair != handlers_.end())
    {
        return handler_pair->second->defaultValue();
    }
    else
    {
        Q_ASSERT(false); // endpoint must be registered
        return {};
    }
}

QVariant
ModelImpl::get(const QString& endpoint) const
{
    auto handler_pair = handlers_.find(endpoint);
    if (handler_pair != handlers_.end())
    {
        return handler_pair->second->get();
    }
    else
    {
        Q_ASSERT(false); // endpoint must be registered
        return {};
    }
}

void
ModelImpl::addListener(const QString& endpoint, Listener& listener)
{
    listener.valueChanged(get(endpoint), false);
    auto& listeners = listeners_[endpoint];
    Q_ASSERT(listeners.find(&listener) ==
             listeners.end()); // listener must not be added yet
    listeners.insert(&listener);
}

void
ModelImpl::removeListener(const QString& endpoint, Listener& listener)
{
    auto listener_pair = listeners_.find(endpoint);
    if (listener_pair != listeners_.end())
    {
        Q_ASSERT(listener_pair->second.find(&listener) !=
                 listener_pair->second.end()); // listener must be present
        listener_pair->second.erase(&listener);
    }
    else
    {
        Q_ASSERT(false); // endpoint must be registered
    }
}

} // namespace app
