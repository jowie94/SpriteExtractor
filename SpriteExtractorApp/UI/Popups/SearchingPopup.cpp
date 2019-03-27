#include "SearchingPopup.hpp"

#include <ImGui/imgui.h>

#include "MessageBroker.hpp"
#include "Messages/SpriteSearchMessages.hpp"
#include "Messages/MainWindowActions.hpp"

SearchingPopup::SearchingPopup()
: PopupWidget("Searching Popup", false)
{}

SearchingPopup::~SearchingPopup()
{
    MessageBroker& broker = MessageBroker::GetInstance();
    broker.Unsubscribe<SpriteSearchMessages::ProgressUpdate>(_progressUpdateSubscription);
    broker.Unsubscribe<SpriteSearchMessages::SpriteSearchFinished>(_searchFinishedSubscription);
}

void SearchingPopup::Init()
{
    MessageBroker& broker = MessageBroker::GetInstance();
    _progressUpdateSubscription = broker.Subscribe<SpriteSearchMessages::ProgressUpdate>(std::bind(&SearchingPopup::OnProgressUpdate, this, std::placeholders::_1));
    _searchFinishedSubscription = broker.Subscribe<SpriteSearchMessages::SpriteSearchFinished>(std::bind(&SearchingPopup::OnSearchFinished, this, std::placeholders::_1));
}

void SearchingPopup::DrawPopup()
{
    std::string message;
    switch (_stage)
    {
        case SpriteExtractor::Task::Stage::GenerateMatrix:
        {
            message = "Generating binary matrix";
            break;
        }
        case SpriteExtractor::Task::Stage::FindSprites:
        {
            message = "Searching sprites";
            break;
        }
        default:
        {
            message = "ERROR: Unknown stage";
            break;
        }
    }

    ImGui::Text("%s", message.c_str());

    ImGui::ProgressBar(_progress);

    if (ImGui::Button("Cancel"))
    {
        MessageBroker::GetInstance().Broadcast(MainWindowActions::CancelSearch());
        Close();
    }
}

void SearchingPopup::OnProgressUpdate(const SpriteSearchMessages::ProgressUpdate& progressUpdate)
{
    _progress = progressUpdate.Progress;
    _stage = progressUpdate.Stage;
}

void SearchingPopup::OnSearchFinished(const SpriteSearchMessages::SpriteSearchFinished& searchFinished)
{
    Close();
}
