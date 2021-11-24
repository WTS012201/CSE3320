#include "fs_ops.h"

int main(){
    std::string selection;
    auto fs = new FSManage{};
    std::cout << "Enter H for help\n\n";
    while(true){
        std::cout << ">> ";
        std::cin >> selection;

        if((std::toupper(selection.at(0)) == 'R' ||
            std::toupper(selection.at(0)) == 'L') &&
            selection.size() != 2){
            std::cout << "Invalid Selection!\n";
            continue;
        } else if(!(std::toupper(selection.at(0)) == 'R' ||
            std::toupper(selection.at(0)) == 'L') &&
            selection.size() != 1){
            std::cout << "Invalid Selection!\n";
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
                if(std::toupper(selection.at(1)) == 'M'){
                    std::cin >> selection;
                    fs -> remove(selection);
                } else if(std::toupper(selection.at(1)) == 'N'){
                    std::string name1, name2;
                    std::cin >> name1 >> name2;
                    fs -> rename(name1, name2);
                } else
                    std::cout << "Invalid Selection!\n";    
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
                if(std::toupper(selection.at(1)) == 'I'){
                    fs -> list_fs();
                } else if(std::toupper(selection.at(1)) == 'K'){

                } else
                    std::cout << "Invalid Selection!\n"; 
                break;
            case 'H':
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