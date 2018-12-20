#include "GenericPlatform.h"

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

#include <string>
#include <vector>

namespace Platform
{
    std::vector<std::string> SplitString(const std::string& str, char delimiter)
    {
        std::vector<std::string> splitted;

        std::string::size_type i = 0;
        std::string::size_type j = str.find(delimiter);

        while (j != std::string::npos) 
        {
            splitted.push_back(str.substr(i, j-i));
            i = ++j;
            j = str.find(delimiter, j);

            if (j == std::string::npos)
            {
                splitted.push_back(str.substr(i, str.length()));
            }
        }

        return splitted;
    }

    NSArray* FormatFilters(const std::vector<FileFilter>& filters)
    {
        NSMutableArray* formattedFilters = [NSMutableArray array];

        for (const auto& filter : filters)
        {
            std::vector<std::string> splittedFilters = SplitString(filter.spec, ';');

            for (const std::string& filterSpec : splittedFilters)
            {
                std::string::size_type dot = filterSpec.find('.') + 1;
                NSString* nsFilter =[NSString stringWithUTF8String:filterSpec.substr(dot, filterSpec.length()).c_str()];
                [formattedFilters addObject:nsFilter];
            }
        }

        return formattedFilters;
    }
}

bool Platform::ShowOpenFileDialogue(const std::string& title, std::string& out, const std::vector<FileFilter>& filters)
{
    std::vector<std::string> fileList;
    // Create a File Open Dialog class.
    NSOpenPanel* openDlg = [NSOpenPanel openPanel];
    [openDlg setLevel:CGShieldingWindowLevel()];
    // Set array of file types

    NSArray* nsFilters = FormatFilters(filters);
    NSString* nsTitle = [NSString stringWithUTF8String:title.c_str()];
    
    // Enable options in the dialog.
    [openDlg setTitle:nsTitle];
    [openDlg setMessage:nsTitle];
    [openDlg setCanChooseFiles:YES];
    [openDlg setAllowsOtherFileTypes:TRUE];
    [openDlg setAllowedFileTypes:nsFilters];
    [openDlg setAllowsMultipleSelection:FALSE];
    // [openDlg setDirectoryURL:[NSURL URLWithString:[NSString stringWithUTF8String:aDefaultPathAndFile ] ] ];

    // Display the dialog box. If the OK pressed,
    // process the files.
    if ([openDlg runModal] == NSModalResponseOK)
    {
        // Gets list of all files selected
        NSArray *files = [openDlg URLs];
        out = std::string([[[files firstObject] path] UTF8String]);

        return true;
    }

    return false;
}