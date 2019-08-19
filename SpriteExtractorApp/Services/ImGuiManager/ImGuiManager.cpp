#include "ImGuiManager.hpp"

#include "Services/Services.hpp"
#include "Services/Scheduler/Scheduler.hpp"

void ImGuiManager::Init()
{
    Services::GetInstance().Get<Scheduler>()->Schedule(this, std::bind(&ImGuiManager::Update, this));
}

void ImGuiManager::Update()
{

}
