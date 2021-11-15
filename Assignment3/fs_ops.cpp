#include "fs_ops.h"

using namespace FS;

FSManage::FSManage() : current{new Disk{}}{}
void FSManage::create_fs(int num_blocks){
    current = new Disk{num_blocks};
}

Disk::Disk(){};
Disk::Disk(int __num_blocks) : num_blocks{__num_blocks},
    disk_name{"unsaved"}{};
void FSManage::format_fs(int file_names, int DABPT_entries){
    try{    current -> format(file_names, DABPT_entries);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}
void Disk::format(int file_names, int DABPT_entries){
    num_file_names = file_names;
    num_DABPT_entries = DABPT_entries;
}
template<typename T>
    void Disk::write(T obj, std::ofstream& os){
        for(auto o : obj)
            os.write((char*)&o, sizeof(o));
    }
template<typename T>
    void Disk::read(T obj, std::ifstream& is){
        for(auto o : obj)
            is.read((char*)&o, sizeof(o));
    }
void FSManage::save_fs(std::string disk_name){
    try{    current -> save(disk_name);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}
void Disk::save(std::string name){
    auto disk = std::ofstream{name, std::ios::binary | std::ios::out};

    write(entry_table, disk);
    write(free_space, disk);
    write(meta, disk);
    write(blocks, disk);
}
void FSManage::open_fs(std::string disk_name){
    try{
        delete current;
        current = new Disk{};
        current -> open_disk(disk_name);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}
void Disk::open_disk(std::string name){
    auto disk = std::ifstream{name, std::ios::binary | std::ios::out};
    
    read(entry_table, disk);
    read(free_space, disk);
    read(meta, disk);
    read(blocks, disk);
}
void FSManage::put(std::string name){
    try{    current -> put(name);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}
void Disk::put(std::string name){
    if(name.length() > FS::ENTRY_MAX)
        return throw std::runtime_error{"File name too big"};

    auto disk = std::ifstream{disk_name, std::ios::binary | std::ios::out};
    auto temp = new char{FS::ENTRY_MAX};   strcpy(temp, name.c_str());
    struct stat file_stat;
    const char* c_temp = temp;

    if(fstat(open(c_temp, std::ios::in), &file_stat) < 0)
        return throw std::runtime_error{"Error reading disk"};
    Entry file = {temp, (int)file_stat.st_ino};

    disk.close();
}