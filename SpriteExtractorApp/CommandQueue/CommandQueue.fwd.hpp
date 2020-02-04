#pragma once

#include <memory>

namespace Commands
{
    class ICommand
    {
    public:
        virtual ~ICommand() = default;

        virtual void redo() = 0;
        virtual void undo() = 0;
    };

	class NestedCommand : public ICommand
	{
	public:
		NestedCommand()
        {}

		template<typename Command, typename... Args>
		void AddCommand(Args&&... args)
		{
			_commands.emplace_back(std::make_unique<Command>(std::forward<Args>(args)...));
		}
		
		void redo() override
		{
			for (auto& command : _commands)
			{
	            command->redo();
			}
		}

		void undo() override
		{
			for (auto it = _commands.rbegin(); it != _commands.rend(); ++it)
			{
	            (*it)->undo();
			}
		}

	private:
        std::vector<std::unique_ptr<ICommand>> _commands;
	};

    struct PushCommandMessage
    {
        PushCommandMessage(const std::shared_ptr<ICommand> command_)
            : command(command_)
        {}

        template<typename Command, typename... Args>
        static PushCommandMessage Create(Args&&... args)
        {
            static_assert(std::is_base_of_v<ICommand, Command>, "Command doesn't inherit from ICommand");
            return PushCommandMessage(std::make_shared<Command>(std::forward<Args>(args)...));
        }

        std::shared_ptr<ICommand> command;
    };
}
