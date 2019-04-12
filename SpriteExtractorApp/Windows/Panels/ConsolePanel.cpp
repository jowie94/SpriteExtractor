#include "ConsolePanel.hpp"

#include <list>
#include <string>
#include <mutex>

#include <spdlog/sinks/base_sink.h>

#include "Logger/Logger.hpp"

template<typename Mutex>
class ConsoleSink : public spdlog::sinks::base_sink<Mutex>
{
public:
    const std::list<std::string>& GetMessageList()
    {
        return _messageList;
    }

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        fmt::memory_buffer formatted;
        spdlog::sinks::sink::formatter_->format(msg, formatted);

        _messageList.emplace_back(fmt::to_string(formatted));
        if (_messageList.size() >= kMaxMessages)
        {
            _messageList.erase(_messageList.begin());
        }
    }

    void flush_() override
    {
    }

private:
    static const size_t kMaxMessages = 200;

    std::list<std::string> _messageList;
};

using ConsoleSinkPtr = ConsoleSink<std::mutex>;

template<typename Sink>
class AutoRegisterSink
{
public:
    AutoRegisterSink()
    {
        _sink = std::make_shared<Sink>();
        Logger::RegisterSink(_sink);
    }

    std::shared_ptr<Sink> GetSink()
    {
        return _sink;
    }

private:
    std::shared_ptr<Sink> _sink;
};

static AutoRegisterSink<ConsoleSinkPtr> autoRegisterSink;

ConsolePanel::ConsolePanel()
: PanelWindow("Console", ImVec2(300.0f, 100.0f), ImGuiWindowFlags_HorizontalScrollbar)
{
    SetClosePolicy(PanelWindow::ClosePolicy::Close);
}

void ConsolePanel::Draw()
{
    PanelWindow::Draw();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1));
    std::list<std::string> messages = autoRegisterSink.GetSink()->GetMessageList();
    for (const auto& msg : messages)
    {
        ImGui::TextUnformatted(msg.c_str());
    }
    ImGui::PopStyleVar();
}
