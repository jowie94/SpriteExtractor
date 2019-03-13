#pragma once

#include "PopupWidget.hpp"
#include "SpriteExtractor.h"
#include "MessageBroker.hpp"

namespace SpriteSearchMessages
{
    struct ProgressUpdate;
    struct SpriteSearchFinished;
}

class SearchingPopup : public PopupWidget
{
public:
    SearchingPopup();
    ~SearchingPopup() override;

    void Init() override;
    void DrawPopup() override;

private:
    void OnProgressUpdate(const SpriteSearchMessages::ProgressUpdate& progressUpdate);
    void OnSearchFinished(const SpriteSearchMessages::SpriteSearchFinished& searchFinished);

    SpriteExtractor::Task::Stage _stage = SpriteExtractor::Task::Stage::None;
    float _progress = 0.0f;

    MessageBroker::SubscriptionId _progressUpdateSubscription;
    MessageBroker::SubscriptionId _searchFinishedSubscription;
};
