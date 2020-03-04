#pragma once
#include"head.h"

class SuperBlockManager
{
private:
    ImgDriver* ID;
    SuperBlock* SB;


public:
    SuperBlockManager();

    ~SuperBlockManager();

    void load();

    void save();

    /*
     * 分配一个Inode节点
     */
    int allocInode();

    /*
     * 释放一个Inode节点
     */
    void freeInode(int no);

    /*
     * 分配一个Block节点
     */
    int allocBlock();

    /*
     * 分配一个Block节点
     */
    void freeBlock(int no);
};

