#ifndef __FS_OPS_H
#define __FS_OPS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <ios>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>          

namespace FS{
    const static int BLOCK_SIZE = 256;
    const static int ENTRY_MAX = 50;
    const static int DA_PER_BLOCK = 4;

    typedef struct Entry{
        char* name;
        int inode;
    }   Entry;

    typedef struct DataBlock{
        char data[FS::BLOCK_SIZE - sizeof(int)];
        int next;
    }   DataBlock; 

    typedef struct DiskAttributes{  //  4 per block
        int time, size, date, pointer;
    } DiskAttributes;

    typedef std::vector<bool> Bitmap;
    typedef std::vector<Entry> EntryTable;
    typedef std::vector<DiskAttributes> DABPT;
    typedef std::vector<DataBlock> Sectors;

    class Disk{
        private:
            std::string disk_name;
            Bitmap free_space;
            Sectors blocks;
            EntryTable entry_table;
            DABPT meta;

            int num_blocks;
            int num_file_names;
            int num_DABPT_entries;
        public:
            Disk();
            Disk(int);
            void format(int file_names, int DABPT_entries);
            void put(std::string name);
            void get(std::string name);
            void save(std::string name);
            void open_disk(std::string name);
            template<typename T>
                void write(T obj, std::ofstream& os);
            template<typename T>
                void read(T obj, std::ifstream& is);
    };
    class FSManage{
        private:
            Disk* current;

        public:
            FSManage();
            void create_fs(int num_blocks);
            void format_fs(int file_names, int DABPT_entries);
            void save_fs(std::string disk_name);
            void open_fs(std::string disk_name);
            void list();
            void remove(std::string file_name);
            void rename(std::string old_file_name, std::string new_file_name);
            void put(std::string name); //  puts file on virtual disk
            void get(std::string ext_file);  // gets file on virtual disk
    };
}
#endif