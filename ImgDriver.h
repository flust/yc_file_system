#pragma once
#include<fstream>
using namespace std;

/*
    对于物理磁盘的读写
*/
class ImgDriver
{
public:
    fstream fs;
    static const char* DISK_FILE_NAME;
public:
    ImgDriver();
    ~ImgDriver();

    //写操作
    bool write(const char* buf, const int buf_size, const int offset);

    //读操作
    bool read(char* buf, const int buf_size, const int offset);
};