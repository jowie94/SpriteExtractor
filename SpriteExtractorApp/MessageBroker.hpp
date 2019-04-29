#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>

class MessageCallbackBase
{
public:
    virtual ~MessageCallbackBase() = default;

    template<typename T>
    void Invoke(const T& msg) const
    {
        InvokeImpl(&msg);
    }

protected:

    virtual void InvokeImpl(const void* msg) const = 0;
};

template<typename T>
class MessageCallback : public MessageCallbackBase
{
public:
    MessageCallback(const std::function<void(const T&)>& func)
    : _func(func)
    {
    }

    MessageCallback(std::function<void(const T&)>&& func)
    : _func(std::move(func))
    {
    }

    MessageCallback(void(*func)(const T&))
    : _func(func)
    {
    }

    template<typename U>
    MessageCallback(U* ptr, void(U::* func)(const T&))
    : _func(std::bind(func, ptr, std::placeholders::_1))
    {
    }

    MessageCallback(const MessageCallback& other)
    : _func(other._func)
    {
    }

    MessageCallback(MessageCallback&& other) noexcept
    : _func(std::move(other._func))
    {
    }

    ~MessageCallback() override = default;

    MessageCallback& operator=(const MessageCallback& other)
    {
        _func = other._func;

        return *this;
    }

    MessageCallback& operator=(MessageCallback&& other) noexcept
    {
        _func.swap(other._func);

        return *this;
    }

private:
    void InvokeImpl(const void* msg) const override
    {
        _func(*static_cast<const T*>(msg));
    }

    std::function<void(const T&)> _func;
};

class MessageBroker
{
public:
    using SubscriptionId = int64_t;

    static MessageBroker& GetInstance()
    {
        static MessageBroker instance;
        return instance;
    }

    template<typename U, typename T>
    SubscriptionId Subscribe(const std::function<void(U::*)(const T&)>& function)
    {
        return Subscribe<T>(std::make_unique<MessageCallback<T>>(function));
    }

    template<typename U, typename T>
    SubscriptionId Subscribe(U* ptr, void(U::* function)(const T&))
    {
        return Subscribe<T>(std::make_unique<MessageCallback<T>>(function));
    }

    template<typename T>
    SubscriptionId Subscribe(std::function<void(const T&)>&& function)
    {
        return Subscribe<T>(std::make_unique<MessageCallback<T>>(function));
    }

    template<typename T>
    void Unsubscribe(SubscriptionId id)
    {
        auto it = _messageCallbacks.find(typeid(T));

        if (it != _messageCallbacks.end())
        {
            it->second.erase(id);
        }
    }

    template<typename T>
    void Broadcast(const T& message)
    {
        auto it = _messageCallbacks.find(typeid(T));

        if (it != _messageCallbacks.end())
        {
            for (const auto& callback : it->second)
            {
                callback.second->Invoke(message);
            }
        }
    }

private:
    template<typename T>
    SubscriptionId Subscribe(std::unique_ptr<MessageCallback<T>>&& ptr)
    {
        SubscriptionId id = reinterpret_cast<SubscriptionId>(ptr.get());
        _messageCallbacks[typeid(T)].emplace(id, std::forward<std::unique_ptr<MessageCallback<T>>>(ptr));
        return id;
    }

    using CallbackMap = std::unordered_map<SubscriptionId, std::unique_ptr<MessageCallbackBase>>;
    std::unordered_map<std::type_index, CallbackMap> _messageCallbacks;
};
