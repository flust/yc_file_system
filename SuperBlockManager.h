#pragma once
#include"SuperBlock.h"
#include"ImgDriver.h"

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

    

};

