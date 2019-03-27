#include "RightPanelWidget.hpp"

#include <ImGui/imgui.h>

#include "MessageBroker.hpp"
#include "Messages/RightPanelActions.hpp"
#include "Messages/GenericActions.hpp"
#include "Messages/SpriteSearchMessages.hpp"

#include "imgui-extra.hpp"

RightPanelWidget::RightPanelWidget(Position position)
: IPanelWidget("Search Sprites", ImVec2(30.0f, 30.0f), position)
{
}

void RightPanelWidget::Init()
{
    IPanelWidget::Init();
    
    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe<GenericActions::ImageOpened>(std::bind(&RightPanelWidget::OnImageOpened, this, std::placeholders::_1));
    broker.Subscribe<GenericActions::ColorHovered>(std::bind(&RightPanelWidget::OnColorHovered, this, std::placeholders::_1));
    broker.Subscribe<GenericActions::ColorPicked>(std::bind(&RightPanelWidget::OnColorPicked, this, std::placeholders::_1));
    broker.Subscribe<SpriteSearchMessages::SpriteSearchFinished>(std::bind(&RightPanelWidget::OnSpriteSearchFinished, this, std::placeholders::_1));
}

void RightPanelWidget::Draw()
{
    IPanelWidget::Draw();

    float col4[4];
    _alphaColor.ToFloat(col4);
    if (ImGui::ColorPicker4("Alpha Color", col4))
    {
        _alphaColor = col4;
    }

    bool colorPickerEnabled = _enableColorPicker;

    MessageBroker& broker = MessageBroker::GetInstance();

    if (colorPickerEnabled)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImColor(96, 198, 53).Value);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(74, 153, 41).Value);
    }
    if (ImGui::Button("Pick Color"))
    {
        _enableColorPicker = !_enableColorPicker;
        broker.Broadcast(RightPanelActions::ToggleColorPicker{_enableColorPicker});
    }
    if (colorPickerEnabled)
    {
        ImGui::PopStyleColor(2);
    }

    if (ImGui::Button("Search Sprites", _imageIsOpened))
    {
        broker.Broadcast(RightPanelActions::SearchSprites{_alphaColor});
        //OnSearchSprites();
    }

    if (ImGui::Button("Save", _hasSprites))
    {
        broker.Broadcast(RightPanelActions::SaveFile());
        //OnSaveFile();
    }
}

void RightPanelWidget::OnImageOpened(const GenericActions::ImageOpened& /*openedImage*/)
{
    _imageIsOpened = true;
    _hasSprites = false;
}

void RightPanelWidget::OnColorHovered(const GenericActions::ColorHovered& colorHovered)
{
    _alphaColor = colorHovered.HoveredColor;
}

void RightPanelWidget::OnColorPicked(const GenericActions::ColorPicked& colorPicked)
{
    _alphaColor = colorPicked.PickedColor;
    _enableColorPicker = false;
}

void RightPanelWidget::OnSpriteSearchFinished(const SpriteSearchMessages::SpriteSearchFinished& foundSprites)
{
    _hasSprites = !foundSprites.FoundSprites.lock()->empty();
}
