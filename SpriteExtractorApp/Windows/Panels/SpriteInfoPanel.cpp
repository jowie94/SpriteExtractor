#include "SpriteInfoPanel.hpp"

#include <algorithm>

#include "Messages/GenericActions.hpp"

#include "Model/ModelManager.hpp"
#include "Model/SpriteSheet/SpriteSheet.hpp"
#include "Model/SpriteSheet/SpriteSheetActions.hpp"

#include "Logger/Logger.hpp"

#include "imgui-extra.hpp"

namespace SpriteInfoPanelConst
{
    std::shared_ptr<const Sprite> kEmptySprite = std::make_shared<Sprite>();
}

SpriteInfoPanel::SpriteInfoPanel()
: PanelWindow("Sprite Info", ImVec2(100.0f, 300.0f))
{
}

SpriteInfoPanel::~SpriteInfoPanel()
{
    MessageBroker::GetInstance().Unsubscribe<GenericActions::ImageOpened>(_openImageSubscription);
}

void SpriteInfoPanel::Init()
{
    PanelWindow::Init();

    _openImageSubscription = MessageBroker::GetInstance().Subscribe<GenericActions::ImageOpened>(std::bind(&SpriteInfoPanel::OnImageOpened, this, std::placeholders::_1));
    SetupSpriteSheet();
}

void SpriteInfoPanel::OnImageOpened(const GenericActions::ImageOpened& /*imageOpened*/)
{
    SetupSpriteSheet();
}

void SpriteInfoPanel::Draw()
{
    PanelWindow::Draw();

    if (_spriteSheet)
    {
        std::shared_ptr<const Sprite> sprite = _spriteSheet->GetSelectedSprite().lock();

        bool editable = sprite != nullptr;
        if (!editable)
        {
            sprite = SpriteInfoPanelConst::kEmptySprite;
        }

        DrawSprite(sprite->BoundingBox);

        if (_currentSpriteName != sprite->Name)
        {
            _currentSpriteName = sprite->Name;
            _tmpSpriteName = sprite->Name;
        }

        ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll;

        if (!editable)
        {
            inputFlags |= ImGuiInputTextFlags_ReadOnly;
        }

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Sprite Name");
        ImGui::SameLine();

        if (ImGui::InputTextWithHint("##SpriteName", "Name", _tmpSpriteName, inputFlags) && !_tmpSpriteName.empty())
        {
            // TODO: Validate
            Logger::GetLogger("Info Panel")->debug("New name: {} -> {}", _currentSpriteName, _tmpSpriteName);
            MessageBroker::GetInstance().Broadcast(Commands::PushCommandMessage(std::make_shared<Commands::Model::EditSpriteName>(_currentSpriteName, _tmpSpriteName)));
        }
    }
    else
    {
        ImGui::LabelText("", "No image opened");
    }
}

void SpriteInfoPanel::SetupSpriteSheet()
{
    _spriteSheet = ModelManager::GetInstance().Get<SpriteSheet>();
    _texture = nullptr;
    if (auto image = _spriteSheet->GetImage().lock())
    {
        _texture = image->GetTextureResource();
    }
}

void SpriteInfoPanel::DrawSprite(const BBox& spriteRect)
{
    if (ImGui::BeginChild("Sprite View", ImVec2(0, 300), true))
    {
        auto image = _spriteSheet->GetImage().lock();
        if (_texture && image)
        {
            ImageSize imageSize = image->Size();
            ImVec2 origin = ImVec2(static_cast<float>(spriteRect.X) / imageSize.X, static_cast<float>(spriteRect.Y) / imageSize.Y);
            ImVec2 end = ImVec2(static_cast<float>(spriteRect.X + spriteRect.Width + 1) / imageSize.X, static_cast<float>(spriteRect.Y + spriteRect.Height + 1) / imageSize.Y);

            float ratio = static_cast<float>(spriteRect.Height) / spriteRect.Width;
            ImVec2 imageWindowSize;

            float maxWidth = ImGui::GetWindowContentRegionWidth();
            float maxHeight = ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;

            imageWindowSize.x = std::min(maxHeight / ratio, maxWidth);
            imageWindowSize.y = std::min(maxWidth * ratio, maxHeight);

            ImVec2 windowSize = ImGui::GetWindowSize();
            float leftSpace = (windowSize.x - imageWindowSize.x) * 0.5f;
            float topSpace = (windowSize.y - imageWindowSize.y) * 0.5f;
            ImGui::SetCursorPos(ImVec2(leftSpace, topSpace));

            ImGui::Image(*_texture, imageWindowSize, origin, end);
        }
        else
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 0.0f), "Error loading texture");
        }
    }
    ImGui::EndChild();
}
