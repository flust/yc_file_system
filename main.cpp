#include"head.h"
using namespace std;

ImgDriver g_ImgDriver;
SuperBlock g_SuperBlock;
SuperBlockManager g_SuperBlockManager;
InodeManager g_InodeManager;
Operator g_Operator;
Buffer g_Buffer;
BufferManager g_BufferManager;

int main()
{
    //g_Operator.login();
    g_Operator.init();
    g_Operator.receiveOrder();
}