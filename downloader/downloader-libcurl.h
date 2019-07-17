//
// Created by Thoh Testarossa on 2019-05-06.
//

#pragma once

#ifndef DOWNLOADER_LIBCURL_DOWNLOADER_LIBCURL_H
#define DOWNLOADER_LIBCURL_DOWNLOADER_LIBCURL_H

#include <string>
#include <vector>

#include "../include/config.h"

typedef struct memBlockStruc
{
    int realSize;
    int allocSize;
    char *memBlock;
}memBlockStruc;

class Downloader_libcurl
{
public:
    Downloader_libcurl();
    ~Downloader_libcurl();

    void downloadAll(int asyncNum, const std::string &path, bool neededWriteBack);

    std::string getFileNameFromURL(const std::string &downloadURL);

    std::vector<std::string> downloadURLSet;

    std::vector<std::pair<std::string, std::string>> downloadedFileSet;

protected:
    std::vector<std::pair<std::string, std::string>> downloadAsyncTask(int asyncNo, int asyncNum, const std::string &path, bool neededWriteBack);
    std::pair<std::string, std::string> download(const std::string &downloadURL, const std::string &path, bool neededWriteBack);

    //This function is used to fit the form of curl-write function
    static size_t fetchDownloadedData(char *ptr, size_t size, size_t nmemb, void *stream);

    static int isInit;
};

#endif //DOWNLOADER_LIBCURL_DOWNLOADER_LIBCURL_H
