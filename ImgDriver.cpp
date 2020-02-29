#include"head.h"
using namespace std;

const char* ImgDriver::DISK_FILE_NAME = "./disk.img";

ImgDriver::ImgDriver(){
    cout << "ImgDriver begin" << endl;
    fs.open(DISK_FILE_NAME, ios::binary | ios::in | ios::out);
}

ImgDriver::~ImgDriver(){
    fs.close();
    cout << "~ImgDriver end" << endl;
}

bool ImgDriver::write(const char* buf, const int buf_size, const int offset)
{
    if(fs.is_open() == 0) 
        return 0;
    fs.seekp(offset, ios::beg);
    fs.write(buf, buf_size);
    //cout << "!!!!!!!Img write to position " << offset << " to " << offset + buf_size << endl;
    return 1;
}


bool ImgDriver::read(char* buf, const int buf_size, const int offset)
{
    if(fs.is_open() == 0) 
        return 0;
    fs.seekg(offset, ios::beg);
    fs.read(buf, buf_size);
    //cout << "!!!!!!Img read from position " << offset << " to " << offset + buf_size << endl;
    return 1;
}