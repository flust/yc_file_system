#include"head.h"


Block::Block(){
    is_folder = true;
    for(int i = 0; i < 16; i++)
    {
        memset(&content[i * 32], '0', 28);
        memset(&content[i * 32 + 28], 0, 4);
    }
    b_flag = 0;
    b_blkno = -1;
    b_use = 0;
};

Block::~Block(){};

void Block::clear()
{
    is_folder = true;
    for(int i = 0; i < 16; i++)
    {
        memset(&content[i * 32], '0', 28);
        memset(&content[i * 32 + 28], 0, 4);
    }
    b_flag = 0;
    b_blkno = -1;
    b_use = 0;
}

folder_node* Block::getFolderList()
{
    folder_node* FL;
    FL = new folder_node[16];
    memcpy(FL, content, sizeof(content));
    for(int i = 0; i < 16; i++)
    {
        for(int j = 27; j >=0; j--)
        {
            if(FL[i].file_name[j] == '*')
                FL[i].file_name[j] = '\0';
        }
    }
    return FL; 
}

int* Block::getIndexList()
{
    int* IN;
    IN = new int[128];
    memcpy(IN, content, sizeof(content));
    return IN;
}


void Block::printBlock()
{
    if(is_folder == 1)
    {
        cout << "========== Block is folder ===========" << endl;
        cout << (char*)content << endl;
        folder_node* fn = getFolderList();
        for(int i = 0; i < 16; i++)
        {
            cout << i + 1 << " : " << fn[i].file_name << ' ' << fn[i].inode_number << endl; 
        }
        cout << "======================================" << endl;
        int* in = getIndexList();
        for(int i = 0; i < 10; i++)
        {
            cout << i + 1 << "addr: " << in[i] << endl; 
        }
    }
    else
    {
        cout << "======= Block is not folder ==========" << endl;
        cout << content << endl;
        cout << "======================================" << endl;
    }
    
}

Buffer::Buffer(){
    b_blocks = new Block[BUFFER_BLOCK_COUNT];
    for(int i = 0; i < BUFFER_BLOCK_COUNT; i++)
    {
        b_blocks[i].is_folder = true;
        for(int i = 0; i < 16; i++)
        {
            memset(&b_blocks[i].content[i * 32], '0', 28);
            memset(&b_blocks[i].content[i * 32 + 28], 0, 4);
        }
        b_blocks[i].b_flag = 0;
        b_blocks[i].b_blkno = -1;
        b_blocks[i].b_use = 0;
    }
}

Buffer::~Buffer(){
    delete [] b_blocks;
}

void Buffer::clear()
{
    for(int i = 0; i < BUFFER_BLOCK_COUNT; i++)
    {
        b_blocks->clear();
    }
}