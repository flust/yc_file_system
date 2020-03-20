#include"head.h"

extern ImgDriver g_ImgDriver;


InodeManager::InodeManager()
{
    cout << "InodeManager begin" << endl;
    ID = &g_ImgDriver;

}

InodeManager::~InodeManager()
{
    /*
    cout << "Iset.size() = " << Iset.size() << endl;
    set<Inode*>::iterator it;
    for(it = Iset.begin(); it != Iset.end(); it++)
    {
        if((*it) -> i_mode & Inode::IUPD)
        {
            cout << "update an inode" << endl;
            writeInode(*it);
        }
    }
    //delete *it;
    cout << "~InodeManager end"  << endl;
    */
}

void InodeManager::saveAll()
{
    //cout << "Iset.size() = " << Iset.size() << endl;
    set<Inode*>::iterator it;
    for(it = Iset.begin(); it != Iset.end(); it++)
    {
        if((*it) -> i_mode & Inode::IUPD)
        {
            //cout << "update an inode" << endl;
            writeInode(*it);
        }
    }
}

/*
    从磁盘中读一个 Inode
*/
Inode* InodeManager::readFromDisk(int no)
{
    Inode* new_inode;
    new_inode = new Inode;
    ID -> read((char*)new_inode, sizeof(DiskInode), SUPERBLOCK_SIZE + no * sizeof(DiskInode));
    new_inode -> i_number = no;
    set<Inode*>::iterator it;
    for(it = Iset.begin(); it != Iset.end(); it++)
    {
        if((*it)->i_number == no)
            return *it;
    }
    Iset.insert(new_inode);
    //new_inode -> printInode();
    return new_inode;
}

void InodeManager::addInode(Inode* inode)
{
    Iset.insert(inode);
}

/*
    获取一个空闲Inode

Inode InodeManager::getNewInode()
{
    Inode new_inode;
    return new_inode;
}
*/
/*
    向磁盘写一个 Inode
*/
void InodeManager::writeInode(Inode* inode)
{
    //cout << "write Inode no: " << inode->i_number << endl;
    //inode->printInode();
    ID->write((char*)inode, sizeof(DiskInode), SUPERBLOCK_SIZE + inode->i_number * sizeof(DiskInode));
}


void InodeManager::printInode(int no)
{
    Inode inode;
    ID->read((char*)&inode, sizeof(DiskInode), SUPERBLOCK_SIZE + no * sizeof(DiskInode));
    cout << "========== " << no << " Inode ===========" << endl;
    cout << "i_mode = " << inode.i_mode << endl;
    cout << "i_count = " << inode.i_count << endl;
    cout << "i_nlink = " << inode.i_nlink << endl;
    cout << "i_uid = " << inode.i_uid << endl;
    cout << "i_gid = " << inode.i_gid << endl;
    cout << "i_size = " << inode.i_size << endl;
    cout << "i_addr[0] = " << inode.i_addr[0] << endl;
    cout << "i_addr[1] = " << inode.i_addr[1] << endl;
    cout << "i_addr[2] = " << inode.i_addr[2] << endl;
    cout << "i_addr[3] = " << inode.i_addr[3] << endl;
    cout << "i_addr[4] = " << inode.i_addr[4] << endl;
    cout << "i_addr[5] = " << inode.i_addr[5] << endl;
    cout << "i_addr[6] = " << inode.i_addr[6] << endl;
    cout << "i_addr[7] = " << inode.i_addr[7] << endl;
    cout << "i_addr[8] = " << inode.i_addr[8] << endl;
    cout << "i_addr[9] = " << inode.i_addr[9] << endl;
    cout << "i_number = " << inode.i_number << endl; 
    cout << "==============================" << endl;
}