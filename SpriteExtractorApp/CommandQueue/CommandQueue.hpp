#pragma once

#include <vector>
#include <memory>

#include "MessageBroker.hpp"
#include "CommandQueue.fwd.hpp"

class CommandQueue
{
public:
    CommandQueue();
    ~CommandQueue();
    
    void Undo();
    void Clear();

private:
    void OnPushCommand(const Commands::PushCommandMessage& command);

    void Update();

    std::vector<std::shared_ptr<Commands::ICommand>> _commandsPending;
    std::vector<std::shared_ptr<Commands::ICommand>> _commandList;

    MessageBroker::SubscriptionId _pushSubscription;
};
