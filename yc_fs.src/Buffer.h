#pragma once

#define BUFFER_BLOCK_COUNT 100
struct folder_node
{
    char file_name[28];
    int inode_number;
};

class Block{
public:
    /* flags中标志位 */
	enum BlockFlag {
		B_WRITE = 0x1,	    /* 写操作。将缓存中的信息写到硬盘上去 */
		B_READ = 0x2,       /* 读操作。从盘读取信息到缓存中 */
		B_DONE = 0x4,       /* I/O操作结束 */
		B_ERROR = 0x8,      /* I/O因出错而终止 */
		B_BUSY = 0x10,	    /* 相应缓存正在使用中 */
		B_WANTED = 0x20,    /* 有进程正在等待使用该buf管理的资源，清B_BUSY标志时，要唤醒这种进程 */
		B_ASYNC = 0x40,		/* 异步I/O，不需要等待其结束 */
		B_DELWRI = 0x80		/* 延迟写，在相应缓存要移做他用时，再将其内容写到相应块设备上 */
	};
    char content[512];
    bool is_folder;
    int b_flag;
    int b_blkno;
    bool b_use;
    bool is_index;

public:
    Block();

    ~Block();

    void clear();

    //由content转换为 folder list
    folder_node* getFolderList();

    //由content转换为 index list
    int* getIndexList();

    void printBlock();
};


class Buffer{
public:
    Block* b_blocks;

public:
    Buffer();
    ~Buffer();
    void clear();
};