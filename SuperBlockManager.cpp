#include"head.h"

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
    ID->read((char*)SB, SUPERBLOCK_SIZE, 0);
    cout << "load" << endl;
    //SB -> printSuperBlock();
}

void SuperBlockManager::save()
{
    ID->write((char*)SB, SUPERBLOCK_SIZE, 0);
    //SB -> printSuperBlock();
}

int SuperBlockManager::allocInode()
{
    char c;
    int one = 1;
    for(int i = 0; i < IBITMAP_SIZE; i++)
    {
        c = SB->s_ibitmap[i];
        cout << "c: " << (int)c << endl;
        for(int j = 0; j < 8; j++)
        {
            if((c & (one << (7 - j))) == 0)
            {
                cout << "分配的节点为 " << 8 * i + j << endl;
                //分配成功
                
                (SB -> s_ibitmap)[i] |= (one << (7 - j));
                //补充操作
                return (8 * i + j);
            }
        }
    }
    cout << "Inode 节点用尽" << endl;
    return -1;
}

int SuperBlockManager::allocBlock()
{
    int content[101];
    SB->s_dirty = 1;

    int blk_no = SB->s_free[SB->s_nfree - 1];
    SB->s_nfree -= 1;
    
    if(SB->s_nfree == 0)
    {
        ID->read((char*)content, sizeof(content), blk_no * BLOCK_SIZE);

        SB->s_nfree = content[0];
        for(int i = 0; i < 100; i++)
        {
            SB->s_free[i] = content[i + 1];
        }
    }
    return blk_no;
}

void SuperBlockManager::freeBlock(int no)
{
    int content[101];
    SB->s_dirty = 1;
    if(SB->s_nfree == 100)
    {
        content[0] = SB->s_nfree;
        for(int i = 0; i < 100; i++)
        {
            content[i + 1] = SB->s_free[i];
        }
        ID->write((char*)content, sizeof(content), no * BLOCK_SIZE);
        SB->s_nfree = 0;
    }
    SB->s_free[SB->s_nfree] = no;
    SB->s_nfree += 1;
    return ;
}