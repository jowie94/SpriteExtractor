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

        std::shared_ptr<ICommand> command;
    };
}
