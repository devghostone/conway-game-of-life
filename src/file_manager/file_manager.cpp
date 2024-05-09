#include "file_manager.hpp"

FileLoadCompletionEvent gfle([](){});

void fileLoadCompleted(){
    gfle.triggerCallback();
    std::cout << "C++ recognized File Load Completion " << std::endl;
}

/// #region FileDialog Method Definition
FileDialog::FileDialog(string title, FileDialogType fileDialogType, char const *filePattern, char const* fileExt)
{
    this->title = title;
    this->fileDialogType = fileDialogType;
    this->filePattern = filePattern;
    this->fileExt = fileExt;
}

string FileDialog::OpenDialog()
{
    string finalPath = "";
    #ifdef __EMSCRIPTEN__
    emscripten::val::global("window").call<void>(
        "triggerFileInput"
    );
    finalPath = "user_input" + string(fileExt);
    #else
        if (fileDialogType == FileDialogType::Open)
        {   
            char const *filePath = tinyfd_openFileDialog(
                title.c_str(),
                "",
                1,
                {&filePattern},
                NULL,
                0);
            if (filePath)
            {
                finalPath = string(filePath);
            }
        }
        else
        {
            char const *filePath = tinyfd_saveFileDialog(
                title.c_str(),
                filePattern,
                1,
                {&filePattern},
                NULL);
            if (filePath)
            {
                finalPath = string(filePath);
            }
        }
    #endif
    return finalPath;
}
/// #endregion

/// #region FileManager
string FileManager::LoadFromFile(string filePath)
{
    ifstream file(filePath);
    if (file.is_open())
    {
        string line;
        string contents;

        while (getline(file, line))
        {
            contents += line + "\n";
        }

        file.close();
        return contents;
    }
    else
    {
        return "";
    }
}

void FileManager::SaveToFile(string filePath, string content)
{
    ofstream outputFile(filePath);
    if (outputFile.is_open())
    {
        outputFile << content;
        outputFile.close();
    }
}

void FileManager::DownloadFileMEMFSToDisk(string filename, string extension, string content){
    FileManager::SaveToFile(filename + extension, content);
    #ifdef __EMSCRIPTEN__
      emscripten::val::global("window").call<void>(
        "offerFileAsDownload",
        filename + extension,
        string("application/octet-stream")
        );
    #endif
}
/// #endregion