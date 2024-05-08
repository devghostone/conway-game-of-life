#include "file_manager.hpp"

/// #region FileDialog Method Definition
FileDialog::FileDialog(string title, FileDialogType fileDialogType, char const *filePattern)
{
    this->title = title;
    this->fileDialogType = fileDialogType;
    this->filePattern = filePattern;
}

string FileDialog::OpenDialog()
{
    string finalPath = "";
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
    std::cout << filePath << std::endl;
    ofstream outputFile(filePath);
    if (outputFile.is_open())
    {
        outputFile << content;
        outputFile.close();
    }
}
/// #endregion