#pragma once

#include <vector>
#include <memory>

#include "MessageBroker.hpp"

class CommandQueue
{
public:
	// TODO FWD
	class ICommand
	{
	public:
		virtual ~ICommand() = default;

		virtual void redo() = 0;
		virtual void undo() = 0;
	};

	struct PushCommandMessage
	{
		PushCommandMessage(const std::shared_ptr<ICommand> command_)
		: command(command_)
		{}

		std::shared_ptr<ICommand> command;
	};

	CommandQueue();
	~CommandQueue();

	void Undo();
	void Clear();

	void Update();

private:
	void OnPushCommand(const PushCommandMessage& command);

	std::vector<std::shared_ptr<ICommand>> _commandsPending;
	std::vector<std::shared_ptr<ICommand>> _commandList;

	MessageBroker::SubscriptionId _pushSubscription;
};
