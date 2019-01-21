#pragma once

#include <string>
#include <vector>

namespace Platform
{
    struct FileFilter
    {
        std::string name;
        std::string spec;
    };

    bool ShowOpenFileDialogue(const std::string& title, std::string& out, const std::vector<FileFilter>& filters = {});
    bool ShowSaveFileDialogue(const std::string& title, std::string& out, const std::vector<FileFilter>& filters = {});
}
