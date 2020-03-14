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
        {
            memset(&BF->b_blocks[i], 0, sizeof(BF->b_blocks[i]));
            return i;
        }
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
    /*
    for(int i = 0; i < BUFFER_BLOCK_COUNT; i++)
    {
        if(BF->b_blocks[i].b_blkno == d_blkno)
        {
            blk_no = i;
        }
    }
    if(BF->b_blocks[blk_no].b_use == 0)
    */
    ID->read((char*)&BF->b_blocks[blk_no], BLOCK_SIZE, d_blkno * BLOCK_SIZE);
    BF->b_blocks[blk_no].b_use = 1;
    BF->b_blocks[blk_no].b_blkno = d_blkno;
    return blk_no;
}

void BufferManager::writeBlock(int no, Block* block)
{
    ID->write((char*)block, BLOCK_SIZE, no * BLOCK_SIZE);
    //cout << "write block" << no << endl;
    //block->printBlock();
    return ;
}

/**
 * 读取文件第 t 块内容，返回值为Block
 * 其中包括了对于大文件的处理
 */
Block* BufferManager::readFileBlock(Inode* f_inode, int t)
{
    int no;
    int i_blkno;
    int* in;
    if(t < 6)
    {
        no = f_inode->i_addr[t];
    }
    else if(t >= 6 && t <= 6 + 128 * 2)
    {
        i_blkno = readBlock(f_inode->i_addr[(t - 6) / 128 + 6]);
        BF->b_blocks[i_blkno].b_blkno = f_inode->i_addr[(t - 6) / 128 + 6];
        in = g_Buffer.b_blocks[i_blkno].getIndexList();
        if((t - 6) / 128 + 6 == 6)
            no = in[t - 6];
        else
            no = in[t - 6 - 128];
        if(no == 0)cout << "ERROR: read file error." << endl;
    }
    else
    {
        i_blkno = readBlock(f_inode->i_addr[(t - 6 - 128 * 2) / 128 / 128 + 8]);
        BF->b_blocks[i_blkno].b_blkno = f_inode->i_addr[(t - 6 - 128 * 2) / 128 / 128 + 8];
        in = BF->b_blocks[i_blkno].getIndexList();

        int t2;
        if((t - 6 - 128 * 2) / 128 / 128 + 8 == 8)
            t2 = (t - 6 - 128 * 2) / 128;
        else
            t2 = (t - 6 - 128 * 2 - 128 * 128) / 128;
        int i_blkno2 = readBlock(in[t2]);
        g_Buffer.b_blocks[i_blkno2].b_blkno = in[t2];
        int* in2 = g_Buffer.b_blocks[i_blkno2].getIndexList();
        if((t - 6 - 128 * 2) / 128 / 128 + 8 == 8)
            no = in2[(t - 6 - 128 * 2) % 128];
        else
            no = in2[(t - 6 - 128 * 2 - 128 * 128) % 128];
        if(no == 0)cout << "ERROR: read file error." << endl;
    }
    int i_blkno2 = readBlock(no);
    return &BF->b_blocks[i_blkno2];
}

void BufferManager::setNewFileBlock(Inode *f_inode, int blkno)
{
    int no;
    int i_blkno;
    int* in;
    int t = f_inode->i_size / 512 + (f_inode->i_size % 512 != 0);
    if(f_inode->i_size == 0)t++;
    //在第t块申请新的block
    if(t < 6)
    {
        f_inode->i_addr[t] = blkno;
    }
    else if(t >= 6 && t <= 6 + 128 * 2)
    {
        if(t == 6 || t == 6 + 128)
        {
            int blkno6or7 = SBM->allocBlock();
            f_inode->i_addr[(t - 6) / 128 + 6] = blkno6or7;
        }
        i_blkno = readBlock(f_inode->i_addr[(t - 6) / 128 + 6]);
        BF->b_blocks[i_blkno].b_blkno = f_inode->i_addr[(t - 6) / 128 + 6];
        in = g_Buffer.b_blocks[i_blkno].getIndexList();
        if((t - 6) / 128 + 6 == 6)
            in[t - 6] = blkno;
        else
            in[t - 6 - 128] = blkno;
    }
    else
    {
        if(t == 6 + 128 * 2 || t == 6 + 128 * 2 + 128 * 128)
        {
            int blkno8or9 = SBM->allocBlock();
            f_inode->i_addr[(t - 6 - 128 * 2) / 128 / 128 + 8] = blkno8or9;
        }
        i_blkno = readBlock(f_inode->i_addr[(t - 6 - 128 * 2) / 128 / 128 + 8]);
        BF->b_blocks[i_blkno].b_blkno = f_inode->i_addr[(t - 6 - 128 * 2) / 128 / 128 + 8];
        in = BF->b_blocks[i_blkno].getIndexList();

        int t2;
        if((t - 6 - 128 * 2) / 128 / 128 + 8 == 8)
            t2 = (t - 6 - 128 * 2) / 128;
        else
            t2 = (t - 6 - 128 * 2 - 128 * 128) / 128;

        if(in[t2] == 0)
        {
            int newblkno = SBM->allocBlock();
            in[t2] = newblkno;
        }
        int i_blkno2 = readBlock(in[t2]);
        g_Buffer.b_blocks[i_blkno2].b_blkno = in[t2];
        int* in2 = g_Buffer.b_blocks[i_blkno2].getIndexList();
        if((t - 6 - 128 * 2) / 128 / 128 + 8 == 8)
            in2[(t - 6 - 128 * 2) % 128] = blkno;
        else
            in2[(t - 6 - 128 * 2 - 128 * 128) % 128] = blkno;
    }

    return ;
}

//t 是指文件的第几块
void BufferManager::writeFileBlock(Inode* f_inode, Block* new_block, int t)
{
    int no;
    int i_blkno;
    int* in;
    if(t < 6)
    {
        no = f_inode->i_addr[t];
    }
    else if(t >= 6 && t <= 6 + 128 * 2)
    {
        i_blkno = readBlock(f_inode->i_addr[(t - 6) / 128 + 6]);
        BF->b_blocks[i_blkno].b_blkno = f_inode->i_addr[(t - 6) / 128 + 6];
        in = g_Buffer.b_blocks[i_blkno].getIndexList();
        if((t - 6) / 128 + 6 == 6)
            no = in[t - 6];
        else
            no = in[t - 6 - 128];
    }
    else
    {
        i_blkno = readBlock(f_inode->i_addr[(t - 6 - 128 * 2) / 128 / 128 + 8]);
        BF->b_blocks[i_blkno].b_blkno = f_inode->i_addr[(t - 6 - 128 * 2) / 128 / 128 + 8];
        in = BF->b_blocks[i_blkno].getIndexList();

        int t2;
        if((t - 6 - 128 * 2) / 128 / 128 + 8 == 8)
            t2 = (t - 6 - 128 * 2) / 128;
        else
            t2 = (t - 6 - 128 * 2 - 128 * 128) / 128;
        int i_blkno2 = readBlock(in[t2]);
        g_Buffer.b_blocks[i_blkno2].b_blkno = in[t2];
        int* in2 = g_Buffer.b_blocks[i_blkno2].getIndexList();
        if((t - 6 - 128 * 2) / 128 / 128 + 8 == 8)
            no = in2[(t - 6 - 128 * 2) % 128];
        else
            no = in2[(t - 6 - 128 * 2 - 128 * 128) % 128];
    }
    
    if(no == 0)cout << "ERROR: write file block error." << endl;
    //写入新文件的Block
    int new_b_blkno = allocBlock();
    //cout << "new_b_blkno:" << new_b_blkno << endl;
    memcpy(BF->b_blocks[new_b_blkno].content, new_block->content, 512);
    BF->b_blocks[new_b_blkno].is_folder = 0;
    BF->b_blocks[new_b_blkno].b_use = 1;
    BF->b_blocks[new_b_blkno].b_blkno = no;
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
            BF->b_blocks[i].b_use = 0;
            writeBlock(BF->b_blocks[i].b_blkno, &BF->b_blocks[i]);
        }
    }
    for(int i = 0; i < BUFFER_BLOCK_COUNT; i++)
    {
        BF->b_blocks[i].is_folder = true;
        for(int i = 0; i < 16; i++)
        {
            memset((char*)&BF->b_blocks[i].content[i * 32], '0', 28);
            memset((char*)&BF->b_blocks[i].content[i * 32 + 28], 0, 4);
        }
        BF->b_blocks[i].b_flag = 0;
        BF->b_blocks[i].b_blkno = -1;
        BF->b_blocks[i].b_use = 0;
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
