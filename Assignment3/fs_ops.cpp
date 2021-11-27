//  Name: William Sigala
//  CSE 3320
//  Assignment 3
#include "fs_ops.h"

FSManage::FSManage() : current{new Disk{}}{}
bool FSManage::create_fs(int num_blocks){
    current = new Disk{num_blocks};
    return EXIT_SUCCESS;
}
Disk::Disk(){};
Disk::Disk(int __num_blocks) : num_blocks{__num_blocks}{
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

    //  Check if there is more than one entry in the table.
    auto f = std::count_if(entry_table.begin(), entry_table.end(),
    [temp](const FS::Entry& e){
        return !std::strcmp(e.name, temp);
    });
    
    //  If more than one entry, prompt user to continue.
    while(f > 0){
        std::string selection;
        std::cout << "This file name is already on the disk.\n";
        std::cout << "Replace(Y/N)? ";
        std::cin >> selection; 
        if(selection.size() > 1){
            std::cout << "Invalid selection!\n";
            continue;
        } else if(std::toupper(selection[0]) == 'N')
            return;
        else if(std::toupper(selection[0]) == 'Y'){
            remove(name);
            break;
        }
        else
            std::cout << "Invalid selection!\n";
    }
    if(fstat(::open(c_temp, std::ios::in), &file_stat) < 0)
        return throw std::runtime_error{"Error reading file."};

    //  Check if enough space is available.
    auto time = gmtime(&file_stat.st_mtime);
    auto pw = getpwuid(file_stat.st_uid);

    auto space = std::count_if(free_space.begin(), free_space.end(),
    [](const bool& b){
        return !b;
    });

    // Put in FNT
    FS::Entry file = {"", (int)file_stat.st_ino};
    bool found;

    for(auto i{0}; i < name.length(); i++)
        file.name[i] = name.at(i);
    for(auto e : entry_table){
        if(found = (e.inode == file.inode))
            break;
    }
    if((space*FS::BLOCK_SIZE < file_stat.st_size) && !found)
        return throw std::runtime_error{"Not enough space on the disk!"};
    for(auto i{0}; i <= entry_table.size(); i++){
        if(i == entry_table.size())
            return throw std::runtime_error{"Ran out of reserved file spaces!"};
        if(!entry_table[i].name[0]){
            entry_table[i] = file;
            break;
        }
    } if(found) return;
    //  Put in DABPT
    
    FS::DiskAttribute DA{
        "",
        (int)file_stat.st_size,
        (int)file_stat.st_mtim.tv_sec,
        (int)file_stat.st_ino,
        (int)block_pointer_table.size()
    };
    for(auto i{0}; pw -> pw_name[i] != '\0' &&
        i < sizeof(DA.user); i++)
        DA.user[i] = pw -> pw_name[i];

    //  Put in blocks.  Will account for bitmap
    auto disk = std::ifstream{name, std::ios::binary};
    auto curr{0}, prev{-1};

    while(!disk.eof()){
        FS::DataBlock DB;
        FS::BlockPointer BP;
        for(auto i{0}; i < FS::BLOCK_SIZE; i++)
            disk.read((char*)&DB.data[i], sizeof(char));
        
        for(; curr < free_space.size(); curr++){ //  curr for a slot that is free
            if(!free_space.at(curr)){
                BP.sector = curr; // ok
                block_pointer_table[curr] = BP;
                if(prev == -1){       //  first case
                    block_pointer_table[curr].next = prev;
                    DA.bp = curr;
                }
                else
                    block_pointer_table[prev].next = curr;
                free_space[curr] = true;// ok
                blocks[curr] = DB;// ok
                prev = curr;
                break;
            }
        }
        if(curr == free_space.size())
            return throw std::runtime_error{"Not enough space available!"};           
    }
    for(auto i{0}; i < meta.size() + 1; i++){
        if(i == meta.size())
            return throw std::runtime_error{"Ran out of reserved file spaces!"};
        if(!meta[i].user[0]){
            meta[i] = DA;
            break;
        }
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
    //  Find entry in table
    auto it1 = std::find_if(entry_table.begin(), entry_table.end(),  
    [name](const FS::Entry& e){
        return !std::strcmp(e.name, name.c_str());
    });
    // If it's not in the table
    if(it1 == entry_table.end())
        return throw std::runtime_error{"The file you've given is not on the disk."};
    //  Find the corresponding meta info
    auto it2 = std::find_if(meta.begin(), meta.end(),        
    [it1](const FS::DiskAttribute& e){
        return it1 -> inode == e.inode;
    });
    //  Start writing data to file
    FS::BlockPointer bp = block_pointer_table[it2 -> bp];
    auto file = std::ofstream{name, std::ios::binary};  
    auto byte{0};
    do{
        auto i{0};
        while(i < FS::BLOCK_SIZE && (byte++) < it2 -> size)
            file.write((char*)&blocks[bp.sector].data[i++], sizeof(char));
        
        if(bp.next == -1)
            break;
        bp = block_pointer_table[bp.next];
    } while (true);
    file.close();
    
    uid_t          uid;
    gid_t          gid;
    struct passwd *pwd;
    struct group  *grp;
    
    pwd = getpwnam(it2 -> user);
    
    if (pwd == NULL) {
        return throw std::runtime_error{"Failed to get uid! Try changing the user who owns this file."};
    }
    uid = pwd->pw_uid;
    grp = getgrnam(it2 -> user);
    if (grp == NULL) {
        return throw std::runtime_error{"Failed to get gid!"};
    }
    gid = grp->gr_gid;
    
    if (chown(name.c_str(), uid, gid) == -1) {
        return throw std::runtime_error{"Changing ownership failed!"};
    }
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
        if(e.name[0] != '\0'){
            auto it = std::find_if(meta.begin(), meta.end(),        //  Find the corresponding meta info
            [e](const FS::DiskAttribute& da){
                return e.inode == da.inode;
            });
            
            std::cout << "File Name: " << e.name << std::endl;
            std::cout << "\tinode: " << e.inode << std::endl;
            std::cout << "\tSize: " << it -> size << " Bytes\n";
            std::cout << "\tLast date: " << it -> time << " Secs\n";
            std::cout << "\tUser: " << it -> user << std::endl;
            //std::cout << std::endl;
        }
    }
}
bool FSManage::remove(std::string name){
    try{    current -> remove(name);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void Disk::remove(std::string name){
    if(!entry_table.size())
        return throw std::runtime_error{"No disk opened. Open or create a new disk."};
    //  Find entry in table
    auto it1 = std::find_if(entry_table.begin(), entry_table.end(),
    [name](const FS::Entry& e){
        return !std::strcmp(e.name, name.c_str());
    });
    // If it's not in the table
    if(it1 == entry_table.end())
        return throw std::runtime_error{"The file you've given is not on the disk."};
    //  If multiple files share same inode, only remove the selected entry
    auto c = std::count_if(entry_table.begin(), entry_table.end(),
    [it1](const FS::Entry& e){
        return it1 -> inode == e.inode;
    });
    if(c > 1){
        *it1 = *(new FS::Entry{});
        return;    
    }
    // If it's not in the table
    if(it1 == entry_table.end())             
        return throw std::runtime_error{"The file you've given is not on the disk."};
    //  Find the corresponding meta info
    auto it2 = std::find_if(meta.begin(), meta.end(),        
    [it1](const FS::DiskAttribute& e){
        return it1 -> inode == e.inode;
    });
    //  Remove corresponding data blocks
    FS::BlockPointer bp = block_pointer_table[it2 -> bp];
    auto curr{it2 -> bp};
    do{
        free_space[bp.sector] = *(new bool{false});
        blocks[bp.sector] = *(new FS::DataBlock{});
        block_pointer_table[curr] = *(new FS::BlockPointer{});
        if(bp.next == -1)
            break;
        curr = bp.next;
        bp = block_pointer_table[curr];
    } while (true);
    //  Remove Entry and Meta info
    *it1 = *(new FS::Entry{});
    *it2 = *(new FS::DiskAttribute{});
}
bool FSManage::rename(std::string old_file_name, std::string new_file_name){
    try{    current -> rename(old_file_name, new_file_name);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void Disk::rename(std::string old_file_name, std::string new_file_name){
    auto it1 = std::find_if(entry_table.begin(), entry_table.end(),
    [old_file_name](const FS::Entry& e){
        return !std::strcmp(e.name, old_file_name.c_str());
    });
    if(it1 == entry_table.end())             
        return throw std::runtime_error{"The file you've given is not on the disk."};
    FS::Entry e{"", it1 -> inode};
    for(auto i{0}; i < new_file_name.size(); i++)
        e.name[i] = new_file_name[i];
    *it1 = e;
}
bool FSManage::user(std::string name){
    try{    current -> user(name);}
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
void Disk::user(std::string file_name){
    auto it1 = std::find_if(entry_table.begin(), entry_table.end(),
    [file_name](const FS::Entry& e){
        return !std::strcmp(e.name, file_name.c_str());
    });
    if(it1 == entry_table.end())             
        return throw std::runtime_error{"The file you've given is not on the disk."};
    auto it2 = std::find_if(meta.begin(), meta.end(),        
    [it1](const FS::DiskAttribute& e){
        return it1 -> inode == e.inode;
    });
    std::cout << it2 -> user << " owns " << file_name << std::endl;
    std::cout << "Would you like to change who owns this file(Y/N)? ";

    while(true){
        std::string selection;
        std::cin >> selection; 
        if(selection.size() > 1){
            std::cout << "Invalid selection!\n";
            continue;
        } else if(std::toupper(selection[0]) == 'N')
            return;
        else if(std::toupper(selection[0]) == 'Y')
            break;
        else
            std::cout << "Invalid selection!\n";
    }
    std::string new_name;
    std::cout << "Enter the user: ";
    std::cin >> new_name;

    auto i{0};
    for(; i < new_name.size() &&
    i < (FS::BLOCK_SIZE/4 - sizeof(int)*4 - 1); i++)
        it2 -> user[i] = new_name[i];
    it2 -> user[i] = '\0';
}