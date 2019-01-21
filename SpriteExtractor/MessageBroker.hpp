#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>
#include <ctime>

class MessageCallbackBase
{
public:
    template<typename T>
    void Invoke(const T& msg) const
    {
        InvokeImpl(&msg);
    }

protected:
    virtual ~MessageCallbackBase() = 0;

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
    static MessageBroker& GetInstance()
    {
        static MessageBroker instance;
        return instance;
    }

    template<typename T>
    int64_t Subscribe(const MessageCallback<T>& callback)
    {
        int64_t id = clock();
        _messageCallbacks[typeid(T)].emplace(id, callback);
        return id;
    }

    template<typename T>
    void Unsubscribe(int64_t id)
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
                callback.second.Invoke(message);
            }
        }
    }

private:
    using CallbackMap = std::unordered_map<int64_t, MessageCallbackBase>;
    std::unordered_map<std::type_index, CallbackMap> _messageCallbacks;
};
