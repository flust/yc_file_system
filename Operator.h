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
    int user_id;
    string users_name;
    string catalog;

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

};