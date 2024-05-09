#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;

#else
#include "tinyfiledialogs.h"
#endif

#include <string>
#include <fstream>
#include <iostream>
#include <functional>

using std::string, std::ifstream, std::ofstream, std::getline, std::function;

extern "C"{
    void fileLoadCompleted();
}

class FileLoadCompletionEvent{
    public:
        typedef std::function<void()> FunctionObject;
        FileLoadCompletionEvent(FunctionObject func): callback(func) {}
        void triggerCallback(){
            if(callback){
                callback();
            }
        }
    private:
        FunctionObject callback;
};

class FileDialog{
    public:
        enum FileDialogType{
            Open,
            SaveAs
        };

        string title;
        FileDialogType fileDialogType;
        char const* filePattern;
        char const* fileExt;

        FileDialog(string title, FileDialogType fileDialogType, char const* filePattern, char const* fileExt);

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
        

        /// @brief download .gocgol file from browser file system to local file system (only for emscripten)
        /// @param content 
        static void DownloadFileMEMFSToDisk(string filename, string extension, string content);
};

extern FileLoadCompletionEvent gfle;