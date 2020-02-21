#pragma once
#include"Inode.h"
#include"ImgDriver.h"
#include<set>

class InodeManager{
public:
    set<Inode*> Iset;
    ImgDriver* ID;

public:
    InodeManager();
    ~InodeManager();

    /*
     * 从磁盘中读一个 Inode
     */
    Inode readFromDisk(int no);

    /*
     * 添加一个节点
     */
    void addInode(Inode* inode);

    /*
        获取一个空闲Inode
    Inode getNewInode();
    */


    /*
        向磁盘写一个 Inode
    */
    void writeInode(Inode* inode);


    void printInode(int no);
};