#include"head.h"

using namespace std;
extern ImgDriver g_ImgDriver;
extern SuperBlock g_SuperBlock;
extern SuperBlockManager g_SuperBlockManager;
extern InodeManager g_InodeManager;

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
    users_name = USERS_NAME[user_id];
    catalog = "/";
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
        cout << "[" << users_name << "@YC_FS " << catalog  << "] ";
        getline(cin, line);
        if(line.size() == 0)
        {
            cout << "===" << endl;
            continue;
        }
        order = split(line, " ");
        cout << order[0] << endl;
        if(order[0] == "format")
        {
            /* format 格式化 */
            format();
            cout << "----format" << endl;
        }
        else if(order[0] == "ls")
        {
            /* ls 列出目录结构 */
            cout << "----ls" << endl;
        }
        else if(order[0] == "mkdir")
        {
            /* mkdir 创建文件夹 */
            cout << "----mkdir" << endl;
        }
        else if(order[0] == "fcreat")
        {
            /* fcreat 创建文件 */
            cout << "----fcreat" << endl;
        }
        else if(order[0] == "fopen")
        {
            /* fopen 打开文件 */
            cout << "----fopen" << endl;
        }
        else if(order[0] == "fclose")
        {
            /* fclose 关闭文件 */
            cout << "----fclose" << endl;
        }
        else if(order[0] == "fwrite")
        {
            /* fwrite 写文件 */
            cout << "----fwrite" << endl;
        }
        else if(order[0] == "fread")
        {
            /* fread 读文件 */
            cout << "----fread" << endl;
        }
        else if(order[0] == "flseek")
        {
            /* flseek 写文件 */
            cout << "----flseek" << endl;
        }
        else if(order[0] == "fdelete")
        {
            /* fdelete 读文件 */
            cout << "----fdelete" << endl;
        }
        else if(order[0] == "cd")
        {
            /* cd 进入目录 */
            cout << "----cd" << endl;
        }
        else if(order[0] == "cp")
        {
            /* cp 拷贝 */
            cout << "----cp" << endl;
        }
        else if(order[0] == "print")
        {
            /* 输出信息 */
            if(order[1] == "SuperBlock")g_SuperBlock.printSuperBlock();
            if(order[1] == "Inode" && order.size() == 3)
            {
                g_InodeManager.printInode(atoi(order[2].c_str()));
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
    
    root_inode->i_number = 0;
    IM->addInode(root_inode);

    // 修改 SuperBlock 中 根目录节点信息
    int one = 1;
    SB->s_ibitmap[0] = (one << 7);
    SB->s_dirty = 1;

}    