#pragma once
#define IBITMAP_SIZE 128
#define INODE_NUM 1024
#define DISK_BLOCK_NUM 64 * 1024
#define SUPERBLOCK_SIZE 1024
#define BLOCK_SIZE 512

class SuperBlock
{
public:
    int s_isize;  // inode总个数
    char s_ibitmap[IBITMAP_SIZE];// 记录 inode 分配情况
    int s_ilock;

    int s_fsize;// 盘块总数
    int s_nfree;// 直接管理的空闲盘块数
    int s_free[100]; // 直接管理的空闲盘块
    int s_flock;
    
    int s_dirty;
    char padding[472];//填充至1024字节

public:
    SuperBlock();
    ~SuperBlock();
    
    void printSuperBlock();

    

};