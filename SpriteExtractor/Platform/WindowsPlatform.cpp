#include "GenericPlatform.h"

#include <shlwapi.h>
#include <shlobj_core.h>
#include <AtlBase.h>
#include <atlconv.h>

namespace Platform
{
    LPCWSTR s2ws(const std::string& str)
    {
        int len;
        int slength = (int)str.length() + 1;
        len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
        wchar_t* buf = new wchar_t[len];
        MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
        return buf;
    }

    COMDLG_FILTERSPEC* GenerateFilterList(const std::vector<FileFilter>& filters)
    {
        COMDLG_FILTERSPEC* res = new COMDLG_FILTERSPEC[filters.size()];

        for (size_t i = 0; i < filters.size(); ++i)
        {
            const FileFilter& filter = filters.at(i);

            res[i].pszName = s2ws(filter.name);
            res[i].pszSpec = s2ws(filter.spec);
        }

        return res;
    }

    void CleanFilterSpec(COMDLG_FILTERSPEC* filterSpec, size_t size)
    {
        for (size_t i = 0; i < size; ++i)
        {
            delete filterSpec[i].pszName;
            filterSpec[i].pszName = nullptr;

            delete filterSpec[i].pszSpec;
            filterSpec[i].pszSpec = nullptr;
        }

        delete[] filterSpec;
    }
}

bool Platform::ShowOpenFileDialogue(const std::string& title, std::string& out, const std::vector<FileFilter>& filters)
{
    IFileOpenDialog* pfd = nullptr;
    HRESULT result = CoCreateInstance(__uuidof(FileOpenDialog), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pfd));

    if (FAILED(result))
    {
        return false;
    }

    DWORD dwFlags;
    result = pfd->GetOptions(&dwFlags);

    if (FAILED(result))
    {
        pfd->Release();
        return false;
    }

    // In this case, get shell items only for file system items.
    result = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);

    if (FAILED(result))
    {
        pfd->Release();
        return false;
    }

    COMDLG_FILTERSPEC* filterSpec = GenerateFilterList(filters);
    result = pfd->SetFileTypes(static_cast<UINT>(filters.size()), filterSpec);
    
    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        return false;
    }

    result = pfd->SetTitle(ATL::CA2W(title.c_str()));

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        return false;
    }

    // Show the dialog
    result = pfd->Show(NULL);

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        return false;
    }

    // Obtain the result once the user clicks the 'Open' button.
    // The result is an IShellItem object.
    IShellItem* psiResult;
    result = pfd->GetResult(&psiResult);

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        return false;
    }

    PWSTR pszFilePath = nullptr;
    result = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        psiResult->Release();
        return false;
    }

    out = ATL::CW2A(pszFilePath);

    CleanFilterSpec(filterSpec, filters.size());
    CoTaskMemFree(pszFilePath);
    psiResult->Release();
    pfd->Release();

    return true;
}

bool Platform::ShowSaveFileDialogue(const std::string& title, std::string& out, const std::vector<FileFilter>& filters)
{
    IFileSaveDialog* pfd = nullptr;
    HRESULT result = CoCreateInstance(__uuidof(FileSaveDialog), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pfd));

    if (FAILED(result))
    {
        return false;
    }

    DWORD dwFlags;
    result = pfd->GetOptions(&dwFlags);

    if (FAILED(result))
    {
        pfd->Release();
        return false;
    }

    // In this case, get shell items only for file system items.
    result = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);

    if (FAILED(result))
    {
        pfd->Release();
        return false;
    }

    COMDLG_FILTERSPEC* filterSpec = GenerateFilterList(filters);
    result = pfd->SetFileTypes(static_cast<UINT>(filters.size()), filterSpec);

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        return false;
    }

    result = pfd->SetTitle(ATL::CA2W(title.c_str()));

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        return false;
    }

    // Show the dialog
    result = pfd->Show(NULL);

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        return false;
    }

    // Obtain the result once the user clicks the 'Open' button.
    // The result is an IShellItem object.
    IShellItem* psiResult;
    result = pfd->GetResult(&psiResult);

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        return false;
    }

    PWSTR pszFilePath = nullptr;
    result = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        psiResult->Release();
        return false;
    }

    UINT fileTypeIndex;
    result = pfd->GetFileTypeIndex(&fileTypeIndex);

    if (FAILED(result))
    {
        CleanFilterSpec(filterSpec, filters.size());
        pfd->Release();
        psiResult->Release();
        return false;
    }

    out = ATL::CW2A(pszFilePath);
    if (StrCmpW(PathFindExtensionW(pszFilePath), L"") == 0)
    {
        out.append(filters.at(fileTypeIndex - 1).spec.substr(1));
    }


    CleanFilterSpec(filterSpec, filters.size());
    CoTaskMemFree(pszFilePath);
    psiResult->Release();
    pfd->Release();

    return true;
}
