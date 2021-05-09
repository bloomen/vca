#include "ModelImpl.h"

namespace app
{

void
ModelImpl::setHandlers(std::vector<std::unique_ptr<Handler>> handlers)
{
    Q_ASSERT(m_handlers.empty());
    for (auto& handler : handlers)
    {
        Q_ASSERT(m_handlers.find(handler->endpoint()) ==
                 m_handlers.end()); // endpoint handler cannot exist yet
        m_handlers[handler->endpoint()] = std::move(handler);
    }
    for (auto& handler_pair : m_handlers)
    {
        addListener(handler_pair.first, *(handler_pair.second));
    }
}

void
ModelImpl::update(const QString& endpoint, const QVariant& value, bool input)
{
    auto handler_pair = m_handlers.find(endpoint);
    if (handler_pair != m_handlers.end())
    {
        auto& handler = handler_pair->second;
        if (value != handler->get())
        {
            handler->setValue(value);
            auto listener_pair = m_listeners.find(endpoint);
            if (listener_pair != m_listeners.end())
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
    auto handler_pair = m_handlers.find(endpoint);
    if (handler_pair != m_handlers.end())
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
    auto handler_pair = m_handlers.find(endpoint);
    if (handler_pair != m_handlers.end())
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
    auto& listeners = m_listeners[endpoint];
    Q_ASSERT(listeners.find(&listener) ==
             listeners.end()); // listener must not be added yet
    listeners.insert(&listener);
}

void
ModelImpl::removeListener(const QString& endpoint, Listener& listener)
{
    auto listener_pair = m_listeners.find(endpoint);
    if (listener_pair != m_listeners.end())
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
