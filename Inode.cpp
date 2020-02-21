#include"Inode.h"
#include<iostream>
#include<cstring>
using namespace std;

DiskInode::DiskInode()
{
    d_mode = 0;
    d_count = 0;
    d_nlink = 0;
    d_uid = -1;
    d_gid = -1;
    d_size = 0;
    memset(d_addr, 0, sizeof(d_addr));
    d_atime = 0;
}

DiskInode::~DiskInode(){}

Inode::Inode()
{
    i_mode = 0;
    i_count = 0;
    i_nlink = 0;
    i_uid = -1;
    i_gid = -1;
    i_size = 0;
    memset(i_addr, 0, sizeof(i_addr));
    i_atime = 0;
    i_number = -1;
}

Inode::~Inode(){};

void Inode::ReadI()
{

}

void Inode::WriteI()
{

}

void Inode::printInode()
{
    cout << "==============================" << endl;
    cout << "i_mode = " << i_mode << endl;
    cout << "i_count = " << i_count << endl;
    cout << "i_nlink = " << i_nlink << endl;
    cout << "i_uid = " << i_uid << endl;
    cout << "i_gid = " << i_gid << endl;
    cout << "i_size = " << i_size << endl;
    cout << "i_number = " << i_number << endl; 
    cout << "==============================" << endl;
}