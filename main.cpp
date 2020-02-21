#include<iostream>
#include<cmath>
#include<cstring>
#include<algorithm>
#include"ImgDriver.h"
#include"constant.h"
#include"Inode.h"
#include"InodeManager.h"
#include"SuperBlock.h"
#include"SuperBlockManager.h"
#include"Operator.h"
using namespace std;

ImgDriver g_ImgDriver;
SuperBlock g_SuperBlock;
SuperBlockManager g_SuperBlockManager;
InodeManager g_InodeManager;
Operator g_Operator;

int main()
{
    g_Operator.login();
    g_Operator.init();
    g_Operator.receiveOrder();

}