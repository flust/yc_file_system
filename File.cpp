#include"head.h"

extern SuperBlockManager g_SuperBlockManager;

File::File(Inode* inode)
{
    SBM = &g_SuperBlockManager;
    f_inode = inode;
    f_isFolder = (inode->i_mode & Inode::IFDIR);
    f_offset = 0;
}

File::~File(){};

int File::getFileSize()
{
    return f_inode->i_size;
}

int File::getBlkNum()
{
    int blk_num = f_inode->i_size / 512;
    if(f_inode->i_size % 512 == 0)
        blk_num++;
    return blk_num;
}

int File::getLastBlkRest()
{
    return f_inode->i_size % 512;
}

int File::read(char* content, int length)
{






    return 0;
}

int File::write(char* content, int length)
{





    
    return 0;
}
