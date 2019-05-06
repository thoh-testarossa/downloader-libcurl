# downloader-libcurl

downloader-libcurl is a lightweight C++-written downloader library using libcurl

## System Requirement

A suitable C++14 compiler is necessary (such as clang, g++-5 and so on)

Also, you may need a cmake with version newer than 3.9 to compile it easily, but it's no matter whether you use cmake or not.

You also need libcurl for compilation.

## Config
Some configs relevant to libcurl opts, such as timeout and lower speed bound can be found & modified in config.h. Also, the number of task downloader will launch can be modified in the same header.

## Installation

Clone this project to your local machine and move to the root directory of this project

```
cd [path]/[to]/downloader-libcurl
```

Create a build directory

```
mkdir build
cd build
```

Execute cmake

```
cmake ..
```

Compile this project

```
make
```

After that, you can find the library file in build/lib directory, as well as test executable in build/bin directory

## Usage

Likes test/main.cpp shows, when you want to use this downloader class, just pass a downloadURLSet (in which contains all the files' address you need to download) to downloader, then execute downloadAll() method. The downloader will do the rest for you.

## Parts finished

Rewritten using std::future rather than std::thread in ehentaiCrawler-libcurl.

## Todo List

Integrate this downloader into project ehentaiCrawler-libcurl.

Find more apps which need to download something and try to put this downloader into those apps.