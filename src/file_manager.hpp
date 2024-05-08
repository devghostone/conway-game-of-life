#pragma once

#include "tinyfiledialogs.h"

#include <string>
#include <fstream>
#include <iostream>

using std::string, std::ifstream, std::ofstream, std::getline;

class FileDialog{
    public:
        enum FileDialogType{
            Open,
            SaveAs
        };

        string title;
        FileDialogType fileDialogType;
        char const* filePattern;

        FileDialog(string title, FileDialogType fileDialogType, char const* filePattern);

        /// @brief Open the dialog the requests for the file
        /// @return 
        string OpenDialog();
};

class FileManager{
    public:
        /// @brief load string from a specific file
        /// @param filePath 
        /// @return 
        static string LoadFromFile(string filePath);
        /// @brief save string to a specific file
        /// @param filePath 
        /// @param content 
        static void SaveToFile(string filePath, string content);
};