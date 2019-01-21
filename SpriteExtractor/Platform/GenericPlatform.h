#pragma once

#include <string>
#include <vector>

namespace Platform
{
    struct FileFilter
    {
        std::string Name;
        std::string Spec;
    };

    bool ShowOpenFileDialogue(const std::string& title, std::string& out, const std::vector<FileFilter>& filters = {});
    bool ShowSaveFileDialogue(const std::string& title, std::string& out, const std::vector<FileFilter>& filters = {});
}
