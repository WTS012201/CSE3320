#include "fs_ops.h"

static std::unordered_map<std::string, int> inputs{{
    {"C", 0},
    {"F", 1},
    {"S", 2},
    {"O", 3},
    {"LI", 4},
    {"RM", 5},
    {"RN", 6},
    {"P", 7},
    {"G", 8},
    {"U", 9},
    {"LK", 10},
    {"Q", 11},
    {"H", 12},
}};

int main(){
    std::string selection;
    auto fs = new FSManage{};
    std::cout << "Enter H for help\n\n";
    while(true){
        std::string arg1, arg2;
        std::cout << ">> ";
        std::cin >> selection;
        std::transform(selection.begin(), selection.end(), selection.begin(), ::toupper);

        if(!inputs.count(selection)){
            std::cout << "Invalid Input!\n";
            continue;
        }
        switch(inputs[selection]){
            case 0:
                std::cin >> selection;
                fs -> create_fs(std::stoi(selection));
                std::cout << "FormatFS #filenames #DABPTEntries: ";
                std::cin >> arg1 >> arg2;
                fs -> format_fs(std::stoi(arg1), std::stoi(arg2));
                break;
            case 1:
                std::cin >> arg1 >> arg2;
                fs -> format_fs(std::stoi(arg1), std::stoi(arg2));
                break;
            case 2:
                std::cin >> selection;
                fs -> save_fs(selection);
                break;
            case 3:
                std::cin >> selection;
                fs -> open_fs(selection);
                break;
            case 4:
                fs -> list_fs();
                break;
            case 5:
                std::cin >> selection;
                fs -> remove(selection);
                break;
            case 6:
                std::cin >> arg1 >> arg2;
                fs -> rename(arg1, arg2);
                break;
            case 7:
                std::cin >> selection;
                fs -> put(selection);
                break;
            case 8:
                std::cin >> selection;
                fs -> get(selection);
                break;
            case 9:
                std::cin >> selection;
                fs -> user(selection);
                break;
            case 10:
                std::cin >> selection;
                //fs -> link(selection);
                break;
            case 11:
                exit(EXIT_SUCCESS);
                break;
            case 12:
                std::cout << "C: Createfs #ofblocks\n";
                std::cout << "F: Formatfs #filenames #DABPTentries\n";
                std::cout << "S: Savefs name– save the “disk” image in a file “name”\n";
                std::cout << "O: Openfs name- use an existing disk image\n";
                std::cout << "Li: List – list files (and other meta-information) in a FS\n";
                std::cout << "Rm: Remove name –remove named file from fs\n";
                std::cout << "Rn: Rename oldname newname – rename a file stored in the FS\n";
                std::cout << "P: Put ExternalFile – put (store) Host OS file into the disk\n";
                std::cout << "G: Get ExternalFile – get disk file, copy from “disk” to host OS file system\n";
                std::cout << "U: User name – show/change name of user who owns this file\n";
                std::cout << "Lk: Link/Unlink – Unix style file linking\n";
                std::cout << "Q: Quit\n";
                break;
            default:
                std::cout << "Invalid Selection!\n";
                break;
        }
    }
    return EXIT_SUCCESS;
}