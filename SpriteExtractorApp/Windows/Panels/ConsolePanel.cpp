#include "ConsolePanel.hpp"

#include <list>
#include <string>
#include <mutex>
#include <unordered_map>

#include <spdlog/sinks/base_sink.h>

#include "imgui-sfml/imgui-SFML.h"

#include "Logger/Logger.hpp"
#include "Services/Services.hpp"
#include "Services/AssetManager/AssetManager.hpp"

template<typename Mutex>
class ConsoleSink : public spdlog::sinks::base_sink<Mutex>
{
public:
    struct Log
    {
        spdlog::level::level_enum Level = spdlog::level::level_enum::off;
        std::string Message;
    };

    using MessageList = std::list<Log>;

    const MessageList& GetMessageList()
    {
        return _messageList;
    }

protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        fmt::memory_buffer formatted;
        spdlog::sinks::sink::formatter_->format(msg, formatted);

        Log log = {msg.level, fmt::to_string(formatted)};
        _messageList.emplace_back(std::move(log));
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

    MessageList _messageList;
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

namespace ConsolePanelConst
{
    const std::unordered_map<spdlog::level::level_enum, ImColor> kLevelColorMap = {
            { spdlog::level::level_enum::trace, ImColor(83, 148, 236) },
            { spdlog::level::level_enum::debug, ImColor(199, 199, 255) },
            { spdlog::level::level_enum::info, ImColor(126, 174, 241) },
            { spdlog::level::level_enum::warn, ImColor(255, 255, 0) },
            { spdlog::level::level_enum::err, ImColor(255, 103, 103) },
            { spdlog::level::level_enum::critical, ImColor(255, 0, 0) },
            { spdlog::level::level_enum::off, ImColor(0, 0, 0) }
    };
}

ConsolePanel::ConsolePanel()
: PanelWindow("Console", ImVec2(300.0f, 100.0f), ImGuiWindowFlags_HorizontalScrollbar)
{
    // TODO: Improve
    _font = Services::GetInstance().Get<AssetManager>()->GetAsset<ImFontAsset>("resources/noto-mono.ttf,16");

    SetClosePolicy(PanelWindow::ClosePolicy::Close);
}

void ConsolePanel::Draw()
{
    PanelWindow::Draw();

    ImGui::PushFont(_font->GetFont());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1));
    ConsoleSinkPtr::MessageList messages = autoRegisterSink.GetSink()->GetMessageList();
    for (const auto& msg : messages)
    {
        ImGui::TextColored(ConsolePanelConst::kLevelColorMap.at(msg.Level), "%s", msg.Message.c_str());
    }
    ImGui::PopStyleVar();

    float currentScroll = ImGui::GetScrollY();
    float currentMaxScroll = ImGui::GetScrollMaxY();

    bool atBottom = currentScroll >= currentMaxScroll;

    if (atBottom && _lastMessageCount != messages.size())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    _lastMessageCount = messages.size();

    ImGui::PopFont();
}
