#pragma once
#include"ImgDriver.h"
#include"SuperBlock.h"
#include"Inode.h"
#include"Buffer.h"
#include"SuperBlockManager.h"

class BufferManager
{
public:
    ImgDriver* ID;
    SuperBlock* SB;
    SuperBlockManager* SBM;
    Buffer* BF;

public:
    BufferManager();
    ~BufferManager();
    
    void clear();
    
    /*
     * 找到buffer中一个空闲的Block
     */
    int allocBlock();

    /*
        * 释放一个Block
        */
    void freeBlock(int no);

    /*
        * 从buffer中读一个block，如果读不到，则从硬盘读入
        */
    int readBlock(int d_blkno);

    /*
        * 写入一个block
        */
    void writeBlock(int no, Block* block);

    /*
        * 读取文件中一个 Block 输入块号 t 和 Inode
        */
    Block* readFileBlock(Inode *f_inode, int t);


    void setNewFileBlock(Inode *f_inode, int blkno);

    void writeFileBlock(Inode *f_inode, Block*new_block, int t);

    /*
        * 刷新buffer，将延迟写的缓存Block写入磁盘
        */
    void flushBuffer();    

    void printBuffer(int no);
    void printBlock(int no);
};