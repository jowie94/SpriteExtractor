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
