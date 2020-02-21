#include"SuperBlockManager.h"
#include"ImgDriver.h"
#include"SuperBlock.h"
#include<iostream>
#include<cstring>

using namespace std;
extern ImgDriver g_ImgDriver;
extern SuperBlock g_SuperBlock;

SuperBlockManager::SuperBlockManager()
{
    ID = &g_ImgDriver;
    SB = &g_SuperBlock;
}

SuperBlockManager::~SuperBlockManager(){
    if(SB->s_dirty == 1)save();
    return;
};

void SuperBlockManager::load()
{
    ID -> read((char*)SB, SUPERBLOCK_SIZE, 0);
    cout << "load" << endl;
    //SB -> printSuperBlock();
}

void SuperBlockManager::save()
{
    ID -> write((char*)SB, SUPERBLOCK_SIZE, 0);
    //SB -> printSuperBlock();
}

int SuperBlockManager::allocInode()
{
    char s_ibitmap[IBITMAP_SIZE];
    char c;
    int one = 1;
    cout << (int)one << endl;
    for(int i = 0; i < IBITMAP_SIZE; i++)
    {
        c = s_ibitmap[i];
        for(int j = 0; j < 8; j++)
        {
            if((c & (one << (7 - i))) == 0)
            {
                cout << "分配的节点为 " << 8 * i + j << endl;
                //分配成功
                

                //补充操作
                return (8 * i + j);
            }
        }
    }
    cout << "Inode 节点用尽" << endl;
    return -1;
}