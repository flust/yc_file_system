#include"head.h"

using namespace std;
SuperBlock::SuperBlock()
{
    int i;
    s_isize = INODE_NUM;
    memset(s_ibitmap, 0, sizeof(s_ibitmap));

    s_ilock = 0;
    s_fsize = DISK_BLOCK_NUM;
    s_nfree = 100;

    memset(s_free, 0, sizeof(s_free));
    s_flock = 0;
    s_dirty = 0;
}

SuperBlock::~SuperBlock(){}

void SuperBlock::printSuperBlock()
{
    cout << "=========== SuperBlock ============" << endl;
    cout << "s_isize = " << s_isize << endl;
    cout << "s_ilock = " << s_ilock << endl;
    cout << "s_fsize = " << s_fsize << endl;
    cout << "s_nfree = " << s_nfree << endl;
    cout << "s_flock = " << s_flock << endl;
    cout << "s_dirty = " << s_dirty << endl;
    cout << "===================================" << endl;
}