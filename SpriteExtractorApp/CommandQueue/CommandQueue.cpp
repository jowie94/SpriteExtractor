#include "CommandQueue.hpp"

#include "Services/Services.hpp"
#include "Services/Scheduler/Scheduler.hpp"

CommandQueue::CommandQueue()
{
    _pushSubscription = MessageBroker::GetInstance().Subscribe<Commands::PushCommandMessage>(
        std::bind(&CommandQueue::OnPushCommand, this, std::placeholders::_1));

    Services::GetInstance().Get<Scheduler>()->Schedule(this, std::bind(&CommandQueue::Update, this));
}

CommandQueue::~CommandQueue()
{
    MessageBroker::GetInstance().Unsubscribe<Commands::PushCommandMessage>(_pushSubscription);

    ServicePtr<Scheduler> scheduler = Services::GetInstance().Get<Scheduler>();
    if (scheduler)
    {
        scheduler->Unschedule(this);
    }
}

void CommandQueue::Undo()
{
}

void CommandQueue::Clear()
{
    _commandList.clear();
    _commandsPending.clear();
}

void CommandQueue::Update()
{
    for (auto& pendingCommand : _commandsPending)
    {
        pendingCommand->redo();
        _commandList.emplace_back(std::move(pendingCommand));
    }

    _commandsPending.clear();
}

void CommandQueue::OnPushCommand(const Commands::PushCommandMessage& command)
{
    _commandsPending.push_back(command.command);
}
