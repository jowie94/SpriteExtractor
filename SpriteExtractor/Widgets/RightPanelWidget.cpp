#include "RightPanelWidget.hpp"

#include "MessageBroker.hpp"
#include "Messages/RightPanelActions.hpp"
#include "Messages/GenericActions.hpp"

#include "imgui-extra.hpp"
#include <imgui.h>

void RightPanelWidget::Init()
{
    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe(MessageCallback<GenericActions::OpenImage>(std::bind(&RightPanelWidget::OnOpenImage, this, std::placeholders::_1)));
    broker.Subscribe(MessageCallback<GenericActions::ImageOpened>(std::bind(&RightPanelWidget::OnImageOpened, this, std::placeholders::_1)));
    broker.Subscribe(MessageCallback<GenericActions::ColorHovered>(std::bind(&RightPanelWidget::OnColorHovered, this, std::placeholders::_1)));
    broker.Subscribe(MessageCallback<GenericActions::ColorPicked>(std::bind(&RightPanelWidget::OnColorPicked, this, std::placeholders::_1)));
}

void RightPanelWidget::Draw()
{
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
        broker.Broadcast(RightPanelActions::SearchSprites());
        //OnSearchSprites();
    }

    if (ImGui::Button("Save", _hasSprites))
    {
        broker.Broadcast(RightPanelActions::SaveFile());
        //OnSaveFile();
    }
}

void RightPanelWidget::OnOpenImage(const GenericActions::OpenImage& /*image*/)
{
    _imageIsOpened = false;
    _hasSprites = false;
}

void RightPanelWidget::OnImageOpened(const GenericActions::ImageOpened& /*openedImage*/)
{
    _imageIsOpened = true;
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
