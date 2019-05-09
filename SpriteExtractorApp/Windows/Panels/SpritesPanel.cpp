#include "SpritesPanel.hpp"

#include <ImGui/imgui.h>

#include <algorithm>

#include "MessageBroker.hpp"
#include "Messages/GenericActions.hpp"
#include "Messages/RightPanelActions.hpp"
#include "Messages/SpriteSearchMessages.hpp"

#include "Model/ModelManager.hpp"
#include "Model/SpriteSheet/SpriteSheet.hpp"

#include "imgui-extra.hpp"

namespace SpritesPanelConst
{
    float CalculateImageScale(const IImage& image, const ImVec2& windowSize)
    {
        ImageSize size = image.Size();
        ImVec2 imgSize(static_cast<float>(size.X), static_cast<float>(size.Y));

        if (windowSize.x > imgSize.x && windowSize.y > imgSize.y)
        {
            return 1.0f;
        }

        float scaleX = windowSize.x / imgSize.x;
        float scaleY = windowSize.y / imgSize.y;

        return std::min(scaleX, scaleY);
    }

    float kZoomFactor = 0.3f;
}

SpritesPanel::SpritesPanel()
: PanelWindow("Spritesheet", ImVec2(-300.0f, -30.0f), ImGuiWindowFlags_NoMove)
{
}

void SpritesPanel::Init()
{
	PanelWindow::Init();

    MessageBroker& broker = MessageBroker::GetInstance();

    broker.Subscribe<RightPanelActions::ToggleColorPicker>(std::bind(&SpritesPanel::OnToggleColorPicker, this, std::placeholders::_1));
    broker.Subscribe<GenericActions::ImageOpened>(std::bind(&SpritesPanel::OnImageOpened, this, std::placeholders::_1));
    broker.Subscribe<SpriteSearchMessages::SpriteSearchFinished>(std::bind(&SpritesPanel::OnSpritesFound, this, std::placeholders::_1));
}

void SpritesPanel::Draw()
{
	PanelWindow::Draw();

    DrawImage();

    ImVec2 cursorPos = ImGui::GetWindowSize();
    cursorPos.x -= 95.0f;
    cursorPos.y -= 50.0f;

    ImGui::SetCursorPos(cursorPos);

    DrawZoom();
}

void SpritesPanel::OnToggleColorPicker(const RightPanelActions::ToggleColorPicker& toggle)
{
    _enableColorPicker = toggle.Enabled;
}

void SpritesPanel::OnImageOpened(const GenericActions::ImageOpened& openedImage)
{
    _openedImage = openedImage.OpenedImage;

    if (auto image = _openedImage.lock())
    {
        _imageScale = SpritesPanelConst::CalculateImageScale(*image, _imageWindowSize);
        _textureResource = image->GetTextureResource();
    }
}

void SpritesPanel::OnSpritesFound(const SpriteSearchMessages::SpriteSearchFinished& spritesFound)
{
	// TODO;
	_spriteSheet = ModelManager::GetInstance().Get<SpriteSheet>();
}

void SpritesPanel::DrawImage()
{
    ImGui::BeginChild("Image", ImVec2(0.0f, 0.0f), false, ImGuiWindowFlags_HorizontalScrollbar);
    _imageWindowSize = ImGui::GetWindowSize();

    if (auto image = _openedImage.lock())
    {
        ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
        ImGui::Image(*_textureResource, ImVec2(_textureResource->Size.X * _imageScale, _textureResource->Size.Y * _imageScale));

        if (_enableColorPicker && ImGui::IsWindowHovered())
        {
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 relativeMousePos((mousePos.x - cursorScreenPos.x) / _imageScale, (mousePos.y - cursorScreenPos.y) / _imageScale);
            Color alphaColor = CalculateHoveredColor(relativeMousePos, image);

            MessageBroker &broker = MessageBroker::GetInstance();
            broker.Broadcast(GenericActions::ColorHovered{alphaColor});

            if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
            {
                _enableColorPicker = false;
                broker.Broadcast(GenericActions::ColorPicked{alphaColor});
            }
        }

        // TODO: This access should be thread safe
        if (_spriteSheet)
        {
            for (const auto &sprite : _spriteSheet->GetSprites())
            {
                ImVec2 rectPos(cursorScreenPos.x + sprite.BoundingBox.X * _imageScale, cursorScreenPos.y + sprite.BoundingBox.Y * _imageScale);
                ImVec2 maxRect(rectPos.x + ((sprite.BoundingBox.Width + 1.0f) * _imageScale), rectPos.y + (sprite.BoundingBox.Height + 1.0f) * _imageScale);
                ImGui::GetWindowDrawList()->AddRect(rectPos, maxRect, ImColor(255, 0, 0));
            }
        }
    }
    ImGui::EndChild();
}

void SpritesPanel::DrawZoom()
{
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(0, 0, 0).Value);
    ImGui::BeginChild("Zoom", ImVec2(75.0f, 30.0f), true);
    if (ImGui::SmallButton("+") && _imageScale > SpritesPanelConst::kZoomFactor)
    {
        _imageScale += SpritesPanelConst::kZoomFactor;
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("-") && _imageScale > SpritesPanelConst::kZoomFactor)
    {
        _imageScale -= SpritesPanelConst::kZoomFactor;
    }
    ImGui::SameLine();
    if (ImGui::SmallButton("="))
    {
        if (auto image = _openedImage.lock())
        {
            _imageScale = SpritesPanelConst::CalculateImageScale(*image, _imageWindowSize);
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

Color SpritesPanel::CalculateHoveredColor(const ImVec2& mousePosition, const std::shared_ptr<IImage> image)
{
    if (mousePosition.x >= 0 && mousePosition.x <= image->Size().X && mousePosition.y >= 0 && mousePosition.y <= image->Size().Y)
    {
        return image->GetPixel(static_cast<unsigned int>(mousePosition.x), static_cast<unsigned int>(mousePosition.y));
    }
    else
    {
        return Color();
    }
}
