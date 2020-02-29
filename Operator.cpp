#include"head.h"

using namespace std;
extern ImgDriver g_ImgDriver;
extern SuperBlock g_SuperBlock;
extern SuperBlockManager g_SuperBlockManager;
extern InodeManager g_InodeManager;
extern BufferManager g_BufferManager;
extern Buffer g_Buffer;

const char* USERS_NAME[] = {"root", "user1", "user2"};
const char* USERS_PASSWORD[] = {"123", "123", "123"};

/*
 *  分割字符串，用于处理用户输入的命令
 */
vector<string> split(string str, const string pattern)
{
    string::size_type pos;
    vector<string> result;

    str += pattern;//扩展字符串以方便操作
    int size = str.size();

    for (int i = 0; i<size; i++) {
        pos = str.find(pattern, i);
        if (pos<size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}



Operator::Operator()
{
    ID = &g_ImgDriver;
    SB = &g_SuperBlock;
    SBM = &g_SuperBlockManager;
    IM = &g_InodeManager;
    BM = &g_BufferManager;
    users_name = USERS_NAME[user_id];
    current_inode_id = 0;
    f_isopen = 0;
    file_seek = 0;
}
Operator::~Operator(){}

/*
 * 用户登录
 */
void Operator::login()
{
    string user, password;
    int len = sizeof(USERS_NAME) / sizeof(USERS_NAME[0]);
    while(1)
    {
        cout << "Please input user name:" << endl;
        getline(cin,user);
        cout << "Please input the password:" << endl;
        getline(cin,password);
        for(int i = 0; i < len; i++)
        {
            if(user == USERS_NAME[i] && password == USERS_PASSWORD[i])
            {
                cout << "==============================================" << endl;
                cout << "=                                            =" << endl;
                cout << "        " << USERS_NAME[i] << " log success. Welcome!     " << endl;
                cout << "=                                            =" << endl;
                cout << "==============================================" << endl;

                user_id = i;
                return ;
            }
        }
        cout << "log Fail! Please check the password." << endl;
    }
}

/*
 * 初始化
 */
void Operator::init()
{
    SBM->load();
    CL.push(0);
    CLN.push("/");
}

/*
 * 接受用户指令
 */
void Operator::receiveOrder()
{
    string line;
    vector<string> order;
    char c;

    while(1)
    {
        cout << "[" << users_name << "@YC_FS " << CLN.top() << "] ";
        if(f_isopen)cout << "<" << f_name << "> has opened--->";
        getline(cin, line);
        if(line.size() == 0)
        {
            cout << "===" << endl;
            continue;
        }
        order = split(line, " ");
        //cout << order[0] << endl;
        if(order[0] == "format")
        {
            /* format 格式化 */
            format();
            //cout << "----format" << endl;
        }
        else if(order[0] == "ls")
        {
            /* ls 列出目录结构 */
            ls();
            //cout << "----ls" << endl;
        }
        else if(order[0] == "mkdir" && order.size() == 2)
        {
            /* mkdir 创建文件夹 */
            mkdir(order[1]);
            //cout << "----mkdir" << endl;
        }
        else if(order[0] == "fcreat")
        {
            /* fcreat 创建文件 */
            fcreate(order[1]);
            //cout << "----fcreat" << endl;
        }
        else if(order[0] == "fopen")
        {
            /* fopen 打开文件 */
            //cout << "----fopen" << endl;
        }
        else if(order[0] == "fclose")
        {
            /* fclose 关闭文件 */
            //cout << "----fclose" << endl;
        }
        else if(order[0] == "fwrite")
        {
            /* fwrite 写文件 */
            //cout << "----fwrite" << endl;
        }
        else if(order[0] == "fread")
        {
            /* fread 读文件 */
            //cout << "----fread" << endl;
        }
        else if(order[0] == "flseek")
        {
            /* flseek 写文件 */
            //cout << "----flseek" << endl;
        }
        else if(order[0] == "fdelete")
        {
            /* fdelete 读文件 */
            //cout << "----fdelete" << endl;
        }
        else if(order[0] == "cd")
        {
            /* cd 进入目录 */
            cd(order[1]);
            //cout << "----cd" << endl;
        }
        else if(order[0] == "cp")
        {
            /* cp 拷贝 */
            //cout << "----cp" << endl;
        }
        else if(order[0] == "print")
        {
            /* 输出信息 */
            if(order[1] == "SuperBlock")
            {
                g_SuperBlock.printSuperBlock();
            }
            if(order[1] == "Inode" && order.size() == 3)
            {
                g_InodeManager.printInode(atoi(order[2].c_str()));
            }
            if(order[1] == "Block" && order.size() == 3)
            {
                g_BufferManager.printBlock(atoi(order[2].c_str()));
            }
            if(order[1] == "Free")//freeblock，成组链接法
            {
                g_SuperBlock.printFree();
            }
            if(order[1] == "Buffer" && order.size() == 3)
            {
                g_BufferManager.printBuffer(atoi(order[2].c_str()));
            }
            //cout << "----cp" << endl;
        }
        else if(order[0] == "exit")
        {
            return ;
        }
        else
        {
            cout << "----input error" << endl;
        }
        IM->saveAll();
        BM->flushBuffer();
    }
}

/*
 * 格式化
 */
void Operator::format()
{
    IM->Iset.clear();

    //更新SuperBlock
    SuperBlock new_SB;
    memcpy(SB, &new_SB, sizeof(SuperBlock));
    SBM->save();
    // 修改 SuperBlock 中 根目录节点信息
    int one = 1;
    SB->s_ibitmap[0] = (one << 7);
    SB->s_dirty = 1;

    //创建根目录Inode节点
    int root_inode_num = 0;
    Inode* root_inode = new Inode;
    root_inode->i_mode = Inode::IUPD | Inode::IFDIR;
    root_inode->i_count = 1;
    root_inode->i_nlink = 1;
    root_inode->i_uid = user_id;
    root_inode->i_gid = user_id;
    memset(root_inode->i_addr, 0, sizeof(root_inode->i_addr));
    root_inode->i_addr[0] = SUPERBLOCK_SIZE / BLOCK_SIZE + INODE_NUM / sizeof(DiskInode);
    
    cout << root_inode->i_addr[0] << endl;

    root_inode->i_number = 0;
    IM->addInode(root_inode);  //添加根目录 Inode

    //设置硬盘中文件第一块内容（root节点目录）
    Block* root_block;
    root_block = new Block();
    root_block->is_folder = 1;
    BM->writeBlock(root_inode->i_addr[0], root_block);

    //初始化空闲盘块管理
    for(int i = 19; i < 64 * 1024; i++)
    {
        SBM->freeBlock(i);
    }

    BM->clear();
    IM->saveAll();
    SBM->save();

    current_inode_id = 0;
}    

void Operator::ls()
{
    //cout << "current_inode_id" << current_inode_id << endl;
    Inode* current_inode = IM->readFromDisk(current_inode_id);
    int i = 0;
    while(1)
    {
        if(current_inode->i_addr[i] == 0)
            return;
        int blkno = g_BufferManager.readBlock(current_inode->i_addr[i]);
        folder_node* fn = g_Buffer.b_blocks[blkno].getFolderList();
        //cout << "================= ls ===================" << endl;
        for(int k = 0; k < 16; k++)
        {
            if(fn[k].inode_number == 0)return;
            cout << fn[k].file_name << endl;
        }
        //cout << "========================================" << endl;
        i++;
    }
}

void Operator::mkdir(string folder_name)
{
    int name_len = strlen(folder_name.c_str());
    if(name_len > 28)
    {
        cout << "ERROR:文件名过长"  << endl;
        return;
    }

    //新建一个Inode
    Inode* new_inode = new Inode;
    new_inode->i_number = SBM->allocInode();
    new_inode->i_mode = Inode::IUPD | Inode::IFDIR;
    new_inode->i_count = 1;
    new_inode->i_nlink = 1;
    new_inode->i_uid = user_id;
    new_inode->i_gid = user_id;
    memset(new_inode->i_addr, 0, sizeof(new_inode->i_addr));
    new_inode->i_addr[0] = SBM->allocBlock();
    //cout << "new_inode->i_number: " << new_inode->i_number << endl;
    //向磁盘中写入新建文件夹的Inode
    IM->writeInode(new_inode);

    //向磁盘中写入新建文件夹的Block
    int new_b_blkno = BM->allocBlock();
    //cout << "new_b_blkno:" << new_b_blkno << endl;
    g_Buffer.b_blocks[new_b_blkno].is_folder = 1;
    g_Buffer.b_blocks[new_b_blkno].b_use = 1;
    g_Buffer.b_blocks[new_b_blkno].b_blkno = new_inode->i_addr[0];

    //读入当前节点目录信息
    Inode* current_inode = new Inode;
    current_inode = IM->readFromDisk(current_inode_id);

    //修改当前文件夹的目录信息
    int fold_cnt = 0;
    folder_node* fn;
    int i_blkno;
    int i = 0;
    bool isok = 0;
    while(1)
    {
        if(current_inode->i_addr[i] == 0)
            break;
        i_blkno = g_BufferManager.readBlock(current_inode->i_addr[i]);
        g_Buffer.b_blocks[i_blkno].b_blkno = current_inode->i_addr[i];
        fn = g_Buffer.b_blocks[i_blkno].getFolderList();
        for(int j = 0; j < 16; j++)
        {
            if(fn[j].inode_number == 0)
            {
                isok = 1;
                break;
            }
            fold_cnt++;
        }
        if(isok == 1)break;
        i++;
    }
    fn[fold_cnt].inode_number = new_inode->i_number;
    //cout << "new folder inumber: " << fn[fold_cnt].inode_number << endl;
    //cout << "new_inode->i_number: " << new_inode->i_number << endl;
    //cout << "name len:" << name_len << endl;
    for(int i = 0; i < 28 - name_len; i++)
    {
        folder_name = folder_name + '*';
    }
    sprintf(fn[fold_cnt].file_name, "%s", folder_name.c_str());
    fn[fold_cnt].inode_number = new_inode->i_number;
    memcpy((char*)&g_Buffer.b_blocks[i_blkno], fn, BLOCK_SIZE);
    //cout << "fold_cnt: " << fold_cnt << endl;
    //cout << "new folder name: " << fn[fold_cnt].file_name << endl;
    //cout << "new folder inumber: " << fn[fold_cnt].inode_number << endl;
    //cout << "new_inode->i_number: " << new_inode->i_number << endl;

    //delete current_inode;
    
    //g_BufferManager.flushBuffer();

}

void Operator::cd(string folder_name)
{
    //cout << "folder_name: " << folder_name << endl;
    int name_len = strlen(folder_name.c_str());
    if(name_len > 28)
    {
        cout << "ERROR:文件名过长"  << endl;
        return;
    }
    if(folder_name == "..")
    {
        if(CL.size() == 0)
        {
            cout << "catalog error" << endl;
            return;
        }
        current_inode_id = CL.top();
        CL.pop();
        CLN.pop();
        return;
    }
    //读入当前节点目录信息
    Inode* current_inode = new Inode;
    current_inode = IM->readFromDisk(current_inode_id);

    int fold_cnt = 0;
    folder_node* fn;
    int i_blkno;
    int i = 0;
    bool isok = 0;
    while(1)
    {
        if(current_inode->i_addr[i] == 0)
            break;
        i_blkno = g_BufferManager.readBlock(current_inode->i_addr[i]);
        g_Buffer.b_blocks[i_blkno].b_blkno = current_inode->i_addr[i];
        fn = g_Buffer.b_blocks[i_blkno].getFolderList();
        for(int j = 0; j < 16; j++)
        {
            //cout << "---------  folder_name: ----------" << endl;
            //cout << fn[j].file_name << endl;
            //cout << folder_name << endl;
            if(fn[j].inode_number == 0)
            {
                isok = 1;
                break;
            }
            //比较文件夹名称，若相同，则进入文件夹
            //cout << "fn[j].file_name" << fn[j].file_name << endl;
            //cout << "folder_name.c_str()" << folder_name.c_str() << endl;
            if(strcmp(fn[j].file_name,folder_name.c_str()) == 0)
            {
                //cout << "------------ find folder ------------" << endl;
                Inode* new_inode = new Inode;
                new_inode = IM->readFromDisk(fn[j].inode_number);
                //new_inode->printInode();
                if((new_inode->i_mode & Inode::IFDIR) != 0)
                {
                    CL.push(current_inode_id);
                    CLN.push(fn[j].file_name);
                    current_inode_id = fn[j].inode_number;
                    //delete new_inode;
                    return;
                }
            }
            fold_cnt++;
        }
        if(isok == 1)break;
        i++;
    }
    cout << "Folder " << folder_name << " not find" << endl;
    return;
}

void Operator::fcreate(string file_name)
{
    //创建inode
    //创建block
    //修改当前节点目录项

    int name_len = strlen(file_name.c_str());
    if(name_len > 28)
    {
        cout << "ERROR:文件名过长"  << endl;
        return;
    }

    //新建一个Inode
    Inode* new_inode = new Inode;
    new_inode->i_number = SBM->allocInode();
    new_inode->i_mode = Inode::IUPD;
    new_inode->i_count = 1;
    new_inode->i_nlink = 1;
    new_inode->i_uid = user_id;
    new_inode->i_gid = user_id;
    new_inode->i_size = 0;
    memset(new_inode->i_addr, 0, sizeof(new_inode->i_addr));
    new_inode->i_addr[0] = SBM->allocBlock();
    //cout << "new_inode->i_number: " << new_inode->i_number << endl;
    //向磁盘中写入新建文件夹的Inode
    IM->writeInode(new_inode);

    //向磁盘中写入新建文件夹的Block
    int new_b_blkno = BM->allocBlock();//分配一个缓冲区节点
    //cout << "new_b_blkno:" << new_b_blkno << endl;
    g_Buffer.b_blocks[new_b_blkno].is_folder = 0;
    g_Buffer.b_blocks[new_b_blkno].b_use = 1;
    g_Buffer.b_blocks[new_b_blkno].b_blkno = new_inode->i_addr[0];

    //读入当前节点目录信息
    Inode* current_inode = new Inode;
    current_inode = IM->readFromDisk(current_inode_id);

    //修改当前文件夹的目录信息
    int fold_cnt = 0;
    folder_node* fn;
    int i_blkno;
    int i = 0;
    bool isok = 0;
    while(1)
    {
        if(current_inode->i_addr[i] == 0)
            break;
        i_blkno = g_BufferManager.readBlock(current_inode->i_addr[i]);
        g_Buffer.b_blocks[i_blkno].b_blkno = current_inode->i_addr[i];
        fn = g_Buffer.b_blocks[i_blkno].getFolderList();
        for(int j = 0; j < 16; j++)
        {
            if(fn[j].inode_number == 0)
            {
                isok = 1;
                break;
            }
            fold_cnt++;
        }
        if(isok == 1)break;
        i++;
    }
    fn[fold_cnt].inode_number = new_inode->i_number;
    //cout << "new folder inumber: " << fn[fold_cnt].inode_number << endl;
    //cout << "new_inode->i_number: " << new_inode->i_number << endl;
    //cout << "name len:" << name_len << endl;
    for(int i = 0; i < 28 - name_len; i++)
    {
        file_name = file_name + '*';
    }
    sprintf(fn[fold_cnt].file_name, "%s", file_name.c_str());
    fn[fold_cnt].inode_number = new_inode->i_number;
    memcpy((char*)&g_Buffer.b_blocks[i_blkno], fn, BLOCK_SIZE);
}

void Operator::fopen(string file_name)
{
     //cout << "folder_name: " << folder_name << endl;
    int name_len = strlen(file_name.c_str());
    if(name_len > 28)
    {
        cout << "ERROR:文件名过长"  << endl;
        return;
    }

    //读入当前节点目录信息
    Inode* current_inode = new Inode;
    current_inode = IM->readFromDisk(current_inode_id);

    int fold_cnt = 0;
    folder_node* fn;
    int i_blkno;
    int i = 0;
    bool isok = 0;
    while(1)
    {
        if(current_inode->i_addr[i] == 0)
            break;
        i_blkno = g_BufferManager.readBlock(current_inode->i_addr[i]);
        g_Buffer.b_blocks[i_blkno].b_blkno = current_inode->i_addr[i];
        fn = g_Buffer.b_blocks[i_blkno].getFolderList();
        for(int j = 0; j < 16; j++)
        {
            //cout << "---------  folder_name: ----------" << endl;
            //cout << fn[j].file_name << endl;
            //cout << folder_name << endl;
            if(fn[j].inode_number == 0)
            {
                isok = 1;
                break;
            }
            if(strcmp(fn[j].file_name,file_name.c_str()) == 0)
            {
                //cout << "------------ find folder ------------" << endl;
                Inode* new_inode = new Inode;
                new_inode = IM->readFromDisk(fn[j].inode_number);
                //new_inode->printInode();
                if((new_inode->i_mode & Inode::IFDIR) == 0)
                {
                    //成功找到文件=============================
                    f_inode = new_inode;
                    f_name = file_name;
                    f_isopen = 1;
                    //delete new_inode;======================
                    return;
                }
            }
            fold_cnt++;
        }
        if(isok == 1)break;
        i++;
    }
    cout << "File " << file_name << " not exist." << endl;
    return;
}


void Operator::fclose()
{
    f_isopen = 0;
    f_seek = 0;
}

void Operator::fwrite(const char* buffer, const int size)
{

}

void Operator::fread(char* buffer, const int size)
{

}

bool Operator::flseek(int pos)
{
    if(pos > f_inode->i_size)
    {
        cout << "ERROR: flseek exceeded file size." << endl;
        return 0;
    }
    f_seek = pos;
    return 1;
}

void Operator::fdelete(string file_name)
{
    int name_len = strlen(file_name.c_str());
    if(name_len > 28)
    {
        cout << "ERROR: file name is too long."  << endl;
        return;
    }



    
    //新建一个Inode
    Inode* new_inode = new Inode;
    new_inode->i_number = SBM->allocInode();
    new_inode->i_mode = Inode::IUPD;
    new_inode->i_count = 1;
    new_inode->i_nlink = 1;
    new_inode->i_uid = user_id;
    new_inode->i_gid = user_id;
    new_inode->i_size = 0;
    memset(new_inode->i_addr, 0, sizeof(new_inode->i_addr));
    new_inode->i_addr[0] = SBM->allocBlock();
    //cout << "new_inode->i_number: " << new_inode->i_number << endl;
    //向磁盘中写入新建文件夹的Inode
    IM->writeInode(new_inode);

    //向磁盘中写入新建文件夹的Block
    int new_b_blkno = BM->allocBlock();//分配一个缓冲区节点
    //cout << "new_b_blkno:" << new_b_blkno << endl;
    g_Buffer.b_blocks[new_b_blkno].is_folder = 0;
    g_Buffer.b_blocks[new_b_blkno].b_use = 1;
    g_Buffer.b_blocks[new_b_blkno].b_blkno = new_inode->i_addr[0];

    //读入当前节点目录信息
    Inode* current_inode = new Inode;
    current_inode = IM->readFromDisk(current_inode_id);

    //修改当前文件夹的目录信息
    int fold_cnt = 0;
    folder_node* fn;
    int i_blkno;
    int i = 0;
    bool isok = 0;
    while(1)
    {
        if(current_inode->i_addr[i] == 0)
            break;
        i_blkno = g_BufferManager.readBlock(current_inode->i_addr[i]);
        g_Buffer.b_blocks[i_blkno].b_blkno = current_inode->i_addr[i];
        fn = g_Buffer.b_blocks[i_blkno].getFolderList();
        for(int j = 0; j < 16; j++)
        {
            if(fn[j].inode_number == 0)
            {
                isok = 1;
                break;
            }
            fold_cnt++;
        }
        if(isok == 1)break;
        i++;
    }
    fn[fold_cnt].inode_number = new_inode->i_number;
    //cout << "new folder inumber: " << fn[fold_cnt].inode_number << endl;
    //cout << "new_inode->i_number: " << new_inode->i_number << endl;
    //cout << "name len:" << name_len << endl;
    for(int i = 0; i < 28 - name_len; i++)
    {
        file_name = file_name + '*';
    }
    sprintf(fn[fold_cnt].file_name, "%s", file_name.c_str());
    fn[fold_cnt].inode_number = new_inode->i_number;
    memcpy((char*)&g_Buffer.b_blocks[i_blkno], fn, BLOCK_SIZE);
}