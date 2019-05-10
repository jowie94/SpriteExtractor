#include "SearchConfigPanel.hpp"

#include <ImGui/imgui.h>

#include "MessageBroker.hpp"
#include "Messages/RightPanelActions.hpp"
#include "Messages/GenericActions.hpp"
#include "Messages/SpriteSearchMessages.hpp"

#include "imgui-extra.hpp"
#include "Model/SpriteSheet/SpriteSheet.fwd.hpp"

SearchConfigPanel::SearchConfigPanel()
: PanelWindow("Search Configuration", ImVec2(30.0f, 30.0f))
{
}

void SearchConfigPanel::Init()
{
    PanelWindow::Init();
    
    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe<GenericActions::ImageOpened>(std::bind(&SearchConfigPanel::OnImageOpened, this, std::placeholders::_1));
    broker.Subscribe<GenericActions::ColorHovered>(std::bind(&SearchConfigPanel::OnColorHovered, this, std::placeholders::_1));
    broker.Subscribe<GenericActions::ColorPicked>(std::bind(&SearchConfigPanel::OnColorPicked, this, std::placeholders::_1));
    broker.Subscribe<SpriteSearchMessages::SpriteSearchFinished>(std::bind(&SearchConfigPanel::OnSpriteSearchFinished, this, std::placeholders::_1));
}

void SearchConfigPanel::Draw()
{
    PanelWindow::Draw();

	MessageBroker& broker = MessageBroker::GetInstance();

    float col4[4];
    _alphaColor.ToFloat(col4);
    if (ImGui::ColorPicker4("Alpha Color", col4))
    {
        _alphaColor = col4;
		MessageBroker::GetInstance().Broadcast(
			Commands::PushCommandMessage(std::make_shared<Commands::Model::UpdateAlphaColorCommand>(_alphaColor)));
    }

    bool colorPickerEnabled = _enableColorPicker;

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
    }

    if (ImGui::Button("Save", _hasSprites))
    {
        broker.Broadcast(RightPanelActions::SaveFile());
    }
}

void SearchConfigPanel::OnImageOpened(const GenericActions::ImageOpened& /*openedImage*/)
{
    _imageIsOpened = true;
    _hasSprites = false;
}

void SearchConfigPanel::OnColorHovered(const GenericActions::ColorHovered& colorHovered)
{
    _alphaColor = colorHovered.HoveredColor;
}

void SearchConfigPanel::OnColorPicked(const GenericActions::ColorPicked& colorPicked)
{
    _alphaColor = colorPicked.PickedColor;
	MessageBroker::GetInstance().Broadcast(
		Commands::PushCommandMessage(std::make_shared<Commands::Model::UpdateAlphaColorCommand>(_alphaColor)));
    _enableColorPicker = false;
}

void SearchConfigPanel::OnSpriteSearchFinished(const SpriteSearchMessages::SpriteSearchFinished& foundSprites)
{
    _hasSprites = foundSprites.spritesFound > 0;
}
