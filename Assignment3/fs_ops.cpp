#include "fs_ops.h"

FSManage::FSManage() : current{new Disk{}}{}
bool FSManage::create_fs(int num_blocks){
    current = new Disk{num_blocks};
    return EXIT_SUCCESS;
}
Disk::Disk(){};
Disk::Disk(int __num_blocks) : num_blocks{__num_blocks},
    offset{0}{
        manage_data(free_space, [](bool o){ o = false;}, __num_blocks);
        manage_data(blocks, [](FS::DataBlock){}, __num_blocks);
        manage_data(block_pointer_table, [](FS::BlockPointer){}, __num_blocks);
    };
bool FSManage::format_fs(int file_names, int DABPT_entries){
    try{    current -> format(file_names, DABPT_entries);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void Disk::format(int file_names, int DABPT_entries){
    num_file_names = file_names;
    manage_data(entry_table, [](FS::Entry){}, num_file_names);
    num_DABPT_entries = DABPT_entries;
    manage_data(meta, [](FS::DiskAttribute){}, num_DABPT_entries);
}
template<class T, typename F>
    void Disk::manage_data(std::vector<T>& obj, F expr, int size){
        if(!size)   //  Write or write over
            for(auto i{0}; i < obj.size(); i++)
                expr(obj.at(i));
        else        //  Read or reserve
            for(auto i{0}; i < size; i++){
                T new_o;
                expr(new_o);
                obj.push_back(new_o);
            }
    }
bool FSManage::open_fs(std::string disk_name){
    try{
        delete current;
        current = new Disk{};
        current -> open(disk_name);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void Disk::open(std::string name){
    auto disk = std::ifstream{name, std::ios::binary};
    
    disk.read((char*)&num_blocks, sizeof(int));
    disk.read((char*)&num_file_names, sizeof(int));
    disk.read((char*)&num_DABPT_entries, sizeof(int));
    manage_data(entry_table, [&](FS::Entry& o){      //FNT
        disk.read((char*)&o, sizeof(o));
    }, num_file_names);
    manage_data(meta, [&](FS::DiskAttribute& o){     //  DABPT
        disk.read((char*)&o, sizeof(o));
    }, num_DABPT_entries);
    manage_data(block_pointer_table, [&](FS::BlockPointer& o){   //  BPT
        disk.read((char*)&o, sizeof(o));
    }, num_blocks);
    manage_data(blocks, [&](FS::DataBlock& o){       //  Blocks
        disk.read((char*)&o, sizeof(o));
    }, num_blocks);
    manage_data(free_space, [&](const bool& o){        //  Bitmap
        disk.read((char*)&o, sizeof(o));
    }, num_blocks);
    disk.close();
    std::cout << entry_table[0].name << std::endl;
}
bool FSManage::save_fs(std::string disk_name){
    try{    current -> save(disk_name);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void Disk::save(std::string name){
    auto disk = std::ofstream{name, std::ios::binary};

    disk.write((char*)&num_blocks, sizeof(int));
    disk.write((char*)&num_file_names, sizeof(int));
    disk.write((char*)&num_DABPT_entries, sizeof(int));
    manage_data(entry_table, [&](FS::Entry o){      //  FNT
        disk.write((char*)&o, sizeof(o));});
    manage_data(meta, [&](FS::DiskAttribute o){     //  DABPT
        disk.write((char*)&o, sizeof(o));});
    manage_data(block_pointer_table, [&](FS::BlockPointer o){   //  BPT
        disk.write((char*)&o, sizeof(o));});
    manage_data(blocks, [&](FS::DataBlock o){       //  Blocks
        disk.write((char*)&o, sizeof(o));});
    manage_data(free_space, [&](bool o){            //  Bitmap
        disk.write((char*)&o, sizeof(o));});
    disk.close();
}
bool FSManage::put(std::string name){
    try{    current -> put(name);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void Disk::put(std::string name){
    if(name.length() > FS::ENTRY_MAX)
        return throw std::runtime_error{"File name too big."};
    if(!free_space.size())
        return throw std::runtime_error{"No disk opened. Open or create a new disk."};

    auto temp = new char{FS::ENTRY_MAX};   strcpy(temp, name.c_str());
    struct stat file_stat;
    const char* c_temp = temp;

    // Put in FNT
    if(fstat(::open(c_temp, std::ios::in), &file_stat) < 0)
        return throw std::runtime_error{"Error reading file."};

    FS::Entry file = {"", (int)file_stat.st_ino};
    for(auto i{0}; i < name.length(); i++)
        file.name[i] = name.at(i);
    for(auto i{0}; i < entry_table.size() + 1; i++){
        if(i == entry_table.size())
            return throw std::runtime_error{"Ran out of reserved file spaces."};
        if(!entry_table[i].name[0]){
            entry_table[i] = file;
            break;
        }
    }
    //  Put in DABPT
    auto time = gmtime(&file_stat.st_mtime);
    auto pw = getpwuid(file_stat.st_uid);
    
    FS::DiskAttribute DA{
        "",
        (int)file_stat.st_size,
        (int)file_stat.st_mtim.tv_sec,
        (int)file_stat.st_ino,
    };
    for(auto i{0}; pw -> pw_name[i] != '\0' &&
        i < sizeof(DA.user); i++)
        DA.user[i] = pw -> pw_name[i];
    //meta.push_back(DA);

    //  Put in blocks.  Will account for bitmap later
    auto disk = std::ifstream{name, std::ios::binary};
    auto scan{0};

    while(!disk.eof()){
        FS::DataBlock DB;
        for(auto i{0}; i < FS::BLOCK_SIZE; i++)
            disk.read((char*)&DB.data[i], sizeof(char));
        
        for(; scan < free_space.size(); scan++){ //  scan for a slot that is free
            if(!free_space.at(scan)){
                free_space[scan] = true;
                blocks[scan] = DB;
                break;
            }
        }
        if(scan == free_space.size())
            return throw std::runtime_error{"Not enough space available."};           
    }
}
bool FSManage::get(std::string name){
    try{    current -> get(name);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void Disk::get(std::string name){
    if(!entry_table.size())
        return throw std::runtime_error{"No disk opened. Open or create a new disk."};
    /*
    for(auto i{0}; i < entry_table.size(); i++){
        if(!std::strcmp(entry_table[i].name, name.c_str())){
            break;
        }
    }*/
    auto file = std::ofstream{name, std::ios::binary};
    auto it = std::find_if(entry_table.begin(), entry_table.end(),
    [name](const FS::Entry& e){
        return !std::strcmp(e.name, name.c_str());
    });
    if(it == entry_table.end())
        return throw std::runtime_error{"The file you've given is not on the disk."};
    

}
bool FSManage::list_fs(){
    try{    current -> list();}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void Disk::list(){
    if(!entry_table.size())
        return throw std::runtime_error{"No disk opened. Open or create a new disk."};
    for(auto e : entry_table){
        if(e.name[0] != '\0')
            std::cout << "File Name: " << e.name << " inode: " << e.inode <<std::endl;
    }
}