#pragma once
#include"SuperBlockManager.h"
#include"SuperBlock.h"
#include"Inode.h"
#include"InodeManager.h"

#define SMALL_FILE_BLK_SIZE 6
#define LARGE_FILE_BLK_SIZE 6 + 128 * 2
#define HUGE_FILE_BLK_SIZE 6 + 128 * 2 + 128 * 128 * 2

class File
{
public:
    SuperBlockManager* SBM;
    Inode* f_inode;
    bool f_isFolder;
    int f_offset;
public:
    File(Inode* inode);
    ~File();

    int getFileSize();

    int getBlkNum();

    int getLastBlkRest();

    int read(char* content, int length);

    int write(char* content, int length);



};