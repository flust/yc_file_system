#include<iostream>
#include<fstream>
#include"Inode.h"
#include"InodeManager.h"
#include"ImgDriver.h"
#include"SuperBlock.h"
#define SUPERBLOCK_SIZE 1024

using namespace std;

ImgDriver g_ImgDriver;
InodeManager g_InodeManager;

int main()
{
    /*
    char buf[512];
    char c = '0';
    //sprintf(buf, "12345aaa");
    for(int i = 0; i < 65536; i++)
    {
        memset(buf, c, sizeof(buf));
        c++;
        if(c == '9')c = '0';
        g_ImgDriver.write(buf, sizeof(buf), i * 512);
    }
    
    char buf2[1024];

    g_ImgDriver.read(buf2, sizeof(buf2), 1024);
    
    cout << buf2 << endl;
    cout << sizeof(buf2) << endl;
    cout << buf2[1023] << endl;
    */
    Inode new_inode1;
    new_inode1 = g_InodeManager.readFromDisk(1);
    cout << new_inode1.i_mode << endl;

    //Inode new_inode2;
    //new_inode2.i_number = 1;
    //g_InodeManager.writeInode(&new_inode2);

    int t;
    while(cin >> t)
    {
        if(t == -1)
            return 0;
    };
    return 0;
}