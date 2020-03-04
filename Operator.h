#pragma once
#include"ImgDriver.h"
#include"constant.h"
#include"Inode.h"
#include"InodeManager.h"
#include"SuperBlock.h"
#include"SuperBlockManager.h"

class Operator{
public:
    ImgDriver* ID;
    SuperBlock* SB;
    SuperBlockManager* SBM;
    InodeManager* IM;
    BufferManager* BM;
    int user_id;
    string users_name;
    int current_inode_id;
    stack<int> CL;                  //目录编号
    stack<string> CLN;              //目录名称

    //以下是文件处理的内容
    bool f_isopen;              //文件是否打开
    string f_name;        //打开的文件名称
    Inode* f_inode;             //打开的文件的Inode
    int f_seek;                  //偏移量指针


public:
    Operator();
    ~Operator();

    /*
     * 用户登陆
     */
    void login();

    /*
     * 初始化
     */
    void init();

    /*
     * 接受用户指令
     */
    void receiveOrder();

    /*
     * 格式化
     */
    void format();

    /*
     * 列出文件列表
     */
    void ls();

    /*
     * 创建文件夹
     */
    void mkdir(string folder_name);

    /*
     * 进入文件夹
     */
    void cd(string folder_name);

    /*
     * 将外部文件拷贝进内部
     */
    void movein(string out_file_name, string in_file_name);

    /*
     * 创建文件
     */
    void fcreate(string folder_name);

    /*
     * 打开文件
     */
    void fopen(string folder_name);

    /*
     * 关闭文件
     */
    void fclose();

    /*
     * 写入文件内容
     */
    void fwrite(const char* buffer, const int size);

    /*
     * 读文件内容
     */
    void fread(char* buffer, const int size);

    /*
     * 设置文件读写指针位置
     */
    bool flseek(int pos);

    /*
     * 删除文件
     */
    void fdelete(string file_name);


    void flushAll();

    void printFileInformation();
};