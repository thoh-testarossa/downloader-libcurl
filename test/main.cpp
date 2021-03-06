#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include "../downloader/downloader-libcurl.h"

int main()
{
    std::ifstream fin("test.txt");
    std::vector<std::string> downloadURL = std::vector<std::string>();

    std::string tmp = std::string();
    if(fin.is_open())
    {
        while(!fin.eof())
        {
            fin >> tmp;
            downloadURL.push_back(tmp);
        }
    }
    else return -1;

    fin.close();

    std::string path = std::string("./");

    Downloader_libcurl d = Downloader_libcurl();
    d.downloadURLSet = downloadURL;
    d.downloadAll(defaultAsyncNum, path, true);

    for(const auto &p : d.downloadedFileSet)
    {
        std::ofstream fout(path + p.first, std::ios::out | std::ios::binary);
        if (fout.is_open()) fout.write(p.second.c_str(), p.second.length());
        fout.close();
    }

    return 0;
}