#pragma once


/*
    一个 DiskInode 64 字节，一个块内 8 个 DiskInode
*/

class DiskInode{
public:
    unsigned int d_mode; //文件工作方式
    
    int d_count; //引用计数
    int d_nlink; //文件联结计数，文件在目录树中不同路径的数量

    short d_uid; //用户编号
    short d_gid; //用户组编号

    int d_size; // 文件大小
    int d_addr[10]; //逻辑块号和物理块号转换

    int d_atime;
public:
    DiskInode();

    ~DiskInode();

};

class Inode{
public:
    enum INodeFlags{
        ILOCK = 0x1,		/* 索引节点上锁 */
		IUPD  = 0x2,		/* 内存inode被修改过，需要更新相应外存inode */
		IACC  = 0x4,		/* 内存inode被访问过，需要修改最近一次访问时间 */
        IFDIR = 0x4000,		/* 文件类型：目录文件 */
        ILARG = 0x1000		/* 文件长度类型：大型或巨型文件 */
    };

    unsigned int i_mode; //文件工作方式
    
    int i_count; //引用计数
    int i_nlink; //文件联结计数，文件在目录树中不同路径的数量

    short i_uid; //用户编号
    short i_gid; //用户组编号

    int i_size; // 文件大小
    int i_addr[10]; //逻辑块号和物理块号转换

    int i_atime;

    int i_number;//块编号

public:
    Inode();

    ~Inode();

    /*
     * 根据盘块 i_addr 读取文件数据
     */
    void ReadI();

    /*
     * 根据 i_addr 写入文件数据
     */
    void WriteI();

    void printInode();
};
