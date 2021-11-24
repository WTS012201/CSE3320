#include "fs_ops.h"

int main(){
    std::string selection;
    std::vector<std::string> args;
    auto fs = new FSManage{};

    while(true){
        std::cout << ">> ";
        std::cin >> selection;

        if(selection.size() > 1){
            std::cout << "Invalid Selection. Reenter: ";
            continue;
        }
        switch(std::toupper(selection.at(0))){
            case 'C': 
                std::cin >> selection;
                fs -> create_fs(std::stoi(selection));
                std::cout << "FormatFS #filenames #DABPTEntries: ";
                int names, entries;
                std::cin >> names >> entries;
                fs -> format_fs(names, entries);
                break;
            case 'R':
                std::cin >> selection;
                fs -> remove(selection);
                break;
            case 'P':
                std::cin >> selection;
                fs -> put(selection);
                break;
            case 'G':
                std::cin >> selection;
                fs -> get(selection);
                break;
            case 'Q':
                exit(EXIT_SUCCESS);
            case 'S':
                std::cin >> selection;
                fs -> save_fs(selection);
                break;
            case 'O':
                std::cin >> selection;
                fs -> open_fs(selection);
                break;
            case 'L':
                fs -> list_fs();
                break;
            case 'H':
                std::cout << "C: Createfs #ofblocks\n";
                std::cout << "F: Formatfs #filenames #DABPTentries\n";
                std::cout << "S: Savefs name– save the “disk” image in a file “name”\n";
                std::cout << "O: Openfs name- use an existing disk image\n";
                std::cout << "L: List – list files (and other meta-information) in a FS\n";
                std::cout << "Rm: Remove name –remove named file from fs\n";
                std::cout << "Rn: Rename oldname newname – rename a file stored in the FS\n";
                std::cout << "P: Put ExternalFile – put (store) Host OS file into the disk\n";
                std::cout << "G: Get ExternalFile – get disk file, copy from “disk” to host OS file system\n";
                std::cout << "U: User name – show/change name of user who owns this file\n";
                std::cout << "Li: Link/Unlink – Unix style file linking\n";
                std::cout << "Q: Quit\n";
                break;
            default:
                std::cout << "Invalid Selection!\n";
                break;
        }
    }
    return EXIT_SUCCESS;
}