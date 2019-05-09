#include "CommandQueue.hpp"

CommandQueue::CommandQueue()
{
	_pushSubscription = MessageBroker::GetInstance().Subscribe<Commands::PushCommandMessage>(
		std::bind(&CommandQueue::OnPushCommand, this, std::placeholders::_1));
}

CommandQueue::~CommandQueue()
{
	MessageBroker::GetInstance().Unsubscribe<Commands::PushCommandMessage>(_pushSubscription);
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
