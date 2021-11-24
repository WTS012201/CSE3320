#ifndef __FS_OPS_H
#define __FS_OPS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <ios>
#include <cstring>
#include <algorithm>

#include <sys/stat.h>
#include <fcntl.h>  
#include <pwd.h>
#include <grp.h>   
#include <typeinfo>     

namespace FS{
    const static int BLOCK_SIZE = 256;
    const static int ENTRY_MAX = 50;
    const static int DA_PER_BLOCK = 4;

    typedef struct Entry{
        char name[ENTRY_MAX];
        int inode;
    }   Entry;

    typedef struct BlockPointer{
        int sector;
        int next = -1;
    } BlockPointer;

    typedef struct DiskAttribute{  //  4 per block
        char user[BLOCK_SIZE/4 - sizeof(int)*4];
        int size, time, inode, bp;
    } DiskAttribute;

    typedef struct DataBlock{
        char data[BLOCK_SIZE];
    } DataBlock; 
}

typedef std::vector<bool> Bitmap;
typedef std::vector<FS::Entry> EntryTable;
typedef std::vector<FS::DiskAttribute> DABPT;
typedef std::vector<FS::DataBlock> Sectors;
typedef std::vector<FS::BlockPointer> BlockPointerTable;
class Disk{
    private:
        Bitmap free_space;
        Sectors blocks;
        EntryTable entry_table;
        BlockPointerTable block_pointer_table;
        DABPT meta;

        int num_blocks;
        int num_file_names;
        int num_DABPT_entries;
        int offset;
        
    public:
        Disk();
        Disk(int);
        void format(int file_names, int DABPT_entries);
        void put(std::string name);
        void get(std::string name);
        void save(std::string name);
        void open(std::string name);
        void list();
        void user();
        void link();
        void remove(std::string name);
        template<typename T, typename F>
            void manage_data(std::vector<T>& obj, F expr, int size = 0);
};

class FSManage{ //  For debugging
    private:
        Disk* current;

    public:
        FSManage();
        bool create_fs(int num_blocks);
        bool format_fs(int file_names, int DABPT_entries);
        bool save_fs(std::string disk_name);
        bool open_fs(std::string disk_name);
        bool list_fs();
        bool remove(std::string file_name);
        bool rename(std::string old_file_name, std::string new_file_name);
        bool put(std::string name); //  puts file on virtual disk
        bool get(std::string name);  // gets file on virtual disk
};
#endif