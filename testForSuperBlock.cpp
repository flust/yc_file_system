#include<iostream>
#include<fstream>
#include"Inode.h"
#include"InodeManager.h"
#include"ImgDriver.h"
#include"SuperBlock.h"
#include"SuperBlockManager.h"
#define SUPERBLOCK_SIZE 1024

using namespace std;

ImgDriver g_ImgDriver;
InodeManager g_InodeManager;
SuperBlock g_SuperBlock;
SuperBlockManager g_SuperBlockManager;

int main()
{

    g_SuperBlockManager.load();

    string t;
    while(cin >> t)
    {
        if(t == "SuperBlock")
            g_SuperBlock.printSuperBlock();
        if(t == "0")
            return 0;
    };
    return 0;
}