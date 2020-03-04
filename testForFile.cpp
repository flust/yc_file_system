#include<iostream>
#include<fstream>
#include<stdio.h>
#include<cmath>
//#include<cstring>
#define BLOCK_SIZE 5
using namespace std;

int main()
{
    string file_name = "./123.txt";
    fstream fin;
    fin.open(file_name, ios::in | ios::binary);
    char buffer[BLOCK_SIZE];
    int length;
    fin.seekg(0, fin.end);
    length = fin.tellg();
    fin.seekg(0, fin.beg);
    cout << length << endl;
    int offset = 0;
    int readsize = min(BLOCK_SIZE, length - offset);
    cout << readsize << endl;
    while(readsize > 0 && fin.read(buffer, readsize))
    {
        //硬盘申请一个block
        
        //setFileBlock，，修改文件Inode

        //申请一个内存block

        //直接向内存block的content里读



        offset += readsize;
        readsize = min(BLOCK_SIZE, length - offset);
        cout << "buffer: " << buffer << endl;
        cout << "strlen(buffer): " << strlen(buffer) << endl;
        cout << "offset: " << offset << endl;
        memset(buffer, '\0', BLOCK_SIZE);
    }

    return 0;
}