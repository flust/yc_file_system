#include"head.h"

extern ImgDriver g_ImgDriver;
extern SuperBlock g_SuperBlock;
extern SuperBlockManager g_SuperBlockManager;
extern Buffer g_Buffer;


BufferManager::BufferManager()
{
    ID = &g_ImgDriver;
    SB = &g_SuperBlock;
    SBM = &g_SuperBlockManager;
    BF = &g_Buffer;
}

BufferManager::~BufferManager(){}

void BufferManager::clear()
{
    BF->clear();
}

/*
 * 找到buffer中一个空闲的Block
 */
int BufferManager::allocBlock()
{
    for(int i = 0; i < BUFFER_BLOCK_COUNT; i++)
    {
        if(BF->b_blocks[i].b_use == 0)
            return i;
    }
    return -1;
}
/*
 * 释放一个Block
 */
void BufferManager::freeBlock(int no)
{
}
/*
 * 从buffer中读一个block，如果读不到，则从硬盘读入
 */
int BufferManager::readBlock(int d_blkno)
{
    int blk_no;
    blk_no = allocBlock();
    for(int i = 0; i < BUFFER_BLOCK_COUNT; i++)
    {
        if(BF->b_blocks[i].b_blkno == d_blkno)
        {
            blk_no = i;
        }
    }
    ID->read((char*)&BF->b_blocks[blk_no], BLOCK_SIZE, d_blkno * BLOCK_SIZE);
    BF->b_blocks[blk_no].b_use = 1;
    return blk_no;
}

void BufferManager::writeBlock(int no, Block* block)
{
    ID->write((char*)block, BLOCK_SIZE, no * BLOCK_SIZE);
    //cout << "write block" << no << endl;
    //block->printBlock();
    return ;
}

/*
 * 刷新buffer，将延迟写的缓存Block写入磁盘
 */
void BufferManager::flushBuffer()
{
    for(int i = 0; i < BUFFER_BLOCK_COUNT; i++)
    {
        if(BF->b_blocks[i].b_blkno != -1)
        {
            writeBlock(BF->b_blocks[i].b_blkno, &BF->b_blocks[i]);
        }
    }
    return;
}


/*
 * 打印
 */
void BufferManager::printBuffer(int no)
{
    cout << "======== Buffer " << no << " ==========" << endl;
    cout << "content: " << BF->b_blocks[no].content << endl;
    cout << "is_folder: " << BF->b_blocks[no].is_folder << endl;
    cout << "b_flag: " << BF->b_blocks[no].b_flag << endl;
    cout << "b_blkno: " << BF->b_blocks[no].b_blkno << endl;
    cout << "b_use: " << BF->b_blocks[no].b_use << endl;
    cout << "===================================" << endl;
    return;
}

void BufferManager::printBlock(int no)
{
    Block* block = new Block();
    ID->read((char*)block, BLOCK_SIZE, no * BLOCK_SIZE);
    block->is_folder = 1;
    block->printBlock();
    return;
}

