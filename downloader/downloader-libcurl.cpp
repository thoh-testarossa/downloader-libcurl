//
// Created by Thoh Testarossa on 2019-05-06.
//

#include "downloader-libcurl.h"
#include "curl/curl.h"

#include <future>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstring>

int Downloader_libcurl::isInit = 0;

Downloader_libcurl::Downloader_libcurl()
{
    if(Downloader_libcurl::isInit == 0)
        curl_global_init(CURL_GLOBAL_ALL);
    Downloader_libcurl::isInit++;

    this->downloadURLSet = std::vector<std::string>();
    this->downloadedFileSet = std::vector<std::pair<std::string, std::string>>();
}

Downloader_libcurl::~Downloader_libcurl()
{
    Downloader_libcurl::isInit--;
    if(Downloader_libcurl::isInit == 0)
        curl_global_cleanup();

    this->downloadURLSet.clear();
    this->downloadedFileSet.clear();
}

void Downloader_libcurl::downloadAll(int asyncNum, const std::string &path, bool neededWriteBack)
{
    auto asyncTaskSet = std::vector<std::future<std::vector<std::pair<std::string, std::string>>>>();

    for(int i = 0; i < asyncNum; i++)
        asyncTaskSet.emplace_back(std::async(&Downloader_libcurl::downloadAsyncTask, this, i, asyncNum, path, neededWriteBack));

    for(int i = 0; i < asyncNum; i++)
    {
        auto tempResult = asyncTaskSet.at(i).get();
        for(auto p : tempResult) this->downloadedFileSet.emplace_back(p);
    }
}

std::vector<std::pair<std::string, std::string>> Downloader_libcurl::downloadAsyncTask(int asyncNo, int asyncNum, const std::string &path, bool neededWriteBack)
{
    auto ret = std::vector<std::pair<std::string, std::string>>();

    for(int i = asyncNo; i < this->downloadURLSet.size(); i += asyncNum)
        ret.emplace_back(this->download(this->downloadURLSet.at(i), path, neededWriteBack));

    return ret;
}

std::pair<std::string, std::string> Downloader_libcurl::download(const std::string &downloadURL, const std::string &path, bool neededWriteBack)
{
    auto ret = std::pair<std::string, std::string>();

    auto fileName = this->getFileNameFromURL(downloadURL);

    ret.first = fileName;
    ret.second = std::string();

    //curl environment setup
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, downloadURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->fetchDownloadedData);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, lowSpeedTimeout);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, lowSpeedBound);

    bool needTry = true;

    int retryCount = 0;
    auto output = std::string();

    while(needTry)
    {
        retryCount++;

        if(retryCount > retryLimit)
        {
            output += "Retry limit exceeded: ";
            output += fileName;
            output += ", download failed\n";
            std::cout << output;
            output.clear();
            needTry = false;
            continue;
        }

        output += "Downloading ";
        output += fileName;
        output += ", retry counter: ";
        output += std::to_string(retryCount);
        output += "\n";
        std::cout << output;
        output.clear();

        //temporary storage allocation
        auto *buf = new memBlockStruc {0, 1, nullptr};

        //download part
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)buf);
        auto err = curl_easy_perform(curl);

        //needTry comes false when download completed or retry limit exceeded
        if(err == CURLE_OK)
        {
            output += "Downloaded ";
            output += fileName;
            output += "\n";
            std::cout << output;
            output.clear();

            for(int i = 0; i < buf->realSize; i++) ret.second += buf->memBlock[i];
            needTry = false;

            if(neededWriteBack)
            {
                std::ofstream fout(path + ret.first, std::ios::out | std::ios::binary);
                if (fout.is_open()) fout.write(ret.second.c_str(), ret.second.length());
                fout.close();
            }
        }

        //temporary storage release
        delete buf;
    }

    //curl exit
    curl_easy_cleanup(curl);

    //return
    return ret;
}

std::string Downloader_libcurl::getFileNameFromURL(const std::string &downloadURL)
{
    std::string result = std::string("");
    for (int i = downloadURL.length() - 1; i >= 0; i--)
    {
        if (downloadURL[i] == '/' && result != std::string("")) break;
        result += downloadURL[i];
    }
    std::reverse(result.begin(), result.end());
    return result;
}

size_t Downloader_libcurl::fetchDownloadedData(char *ptr, size_t size, size_t nmemb, void *stream)
{
    auto *mem = (memBlockStruc *)stream;

    if(mem->memBlock == nullptr) mem->memBlock = (char *)malloc(mem->allocSize);
    if(mem->memBlock == nullptr) return -1;
    while(mem->realSize + size * nmemb > mem->allocSize)
    {
        mem->allocSize <<= 1;
        mem->memBlock = (char *)realloc(mem->memBlock, mem->allocSize);
    }

    memcpy(&(mem->memBlock[mem->realSize]), ptr, size * nmemb);
    mem->realSize += size * nmemb;
    return size * nmemb;
}
