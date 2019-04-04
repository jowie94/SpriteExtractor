set(UIFrameworkFiles ${UIFrameworkFiles} "UI/BaseWindow.cpp")
set(UIFrameworkFiles ${UIFrameworkFiles} "UI/BaseMainWindow.cpp")
set(UIFrameworkFiles ${UIFrameworkFiles} "UI/PanelWindow.cpp")
set(UIFrameworkFiles ${UIFrameworkFiles} "UI/Popup/PopupWindow.cpp")
set(UIFrameworkFiles ${UIFrameworkFiles} "Controllers/PopupsController.cpp")

add_library(UIFramework STATIC ${UIFrameworkFiles})

target_link_libraries(UIFramework ImGui)

target_include_directories(UIFramework PUBLIC .)
