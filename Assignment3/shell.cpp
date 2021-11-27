//  Name: William Sigala
//  CSE 3320
//  Assignment 3
#include "fs_ops.h"

static std::unordered_map<std::string, int> inputs{{
    {"C", 0},
    {"S", 1},
    {"O", 2},
    {"LS", 3},
    {"RM", 4},
    {"RN", 5},
    {"P", 6},
    {"G", 7},
    {"U", 8},
    {"Q", 9},
    {"H", 10},
}};

int main(){
    std::string selection;
    auto fs = new FSManage{};
    std::cout << "Enter H for help\n\n";
    while(true){
        std::string arg1, arg2;
        int a, b;
        std::cout << ">> ";
        std::cin >> selection;
        std::transform(selection.begin(), selection.end(), selection.begin(), ::toupper);

        if(!inputs.count(selection)){
            std::cout << "Invalid Input!\n";
            continue;
        }
        switch(inputs[selection]){
            case 0:
                std::cin >> a;
                fs -> create_fs(a);
                std::cout << "FormatFS #filenames #DABPTEntries: ";
                std::cin >> a >> b;
                
                fs -> format_fs(a, b);
                break;
            case 1:
                std::cin >> selection;
                fs -> save_fs(selection);
                break;
            case 2:
                std::cin >> selection;
                fs -> open_fs(selection);
                break;
            case 3:
                fs -> list_fs();
                break;
            case 4:
                std::cin >> selection;
                fs -> remove(selection);
                break;
            case 5:
                std::cin >> arg1 >> arg2;
                fs -> rename(arg1, arg2);
                break;
            case 6:
                std::cin >> selection;
                fs -> put(selection);
                break;
            case 7:
                std::cin >> selection;
                fs -> get(selection);
                break;
            case 8:
                std::cin >> selection;
                fs -> user(selection);
                break;
            case 9:
                exit(EXIT_SUCCESS);
                break;
            case 10:
                std::cout << "C: Createfs #ofblocks\n";
                std::cout << "S: Savefs name– save the “disk” image in a file “name”\n";
                std::cout << "O: Openfs name- use an existing disk image\n";
                std::cout << "Ls: List – list files (and other meta-information) in a FS\n";
                std::cout << "Rm: Remove name –remove named file from fs\n";
                std::cout << "Rn: Rename oldname newname – rename a file stored in the FS\n";
                std::cout << "P: Put ExternalFile – put (store) Host OS file into the disk\n";
                std::cout << "G: Get ExternalFile – get disk file, copy from “disk” to host OS file system\n";
                std::cout << "U: User name – show/change name of user who owns this file\n";
                std::cout << "Q: Quit\n";
                break;
            default:
                std::cout << "Invalid Selection!\n";
                break;
        }
    }
    return EXIT_SUCCESS;
}