#include "fs_ops.h"

int main(int argv, char* argsv[]){
    std::string selection;
    auto fs = new FS::FSManage{};
    
    while(true){
        std::cout << "Enter a selection: ";
        std::cin >> selection;
        if(selection.size() > 1){
            std::cout << "Invalid Selection. Reenter: ";
            continue;
        }
        switch(std::toupper(selection.at(0))){
            case 'C': 
                std::cout << "Enter number of blocks for fs: ";
                std::cin >> selection;
                fs -> create_fs(std::stoi(selection));

                std::cout << "FormatFS #filenames #DABPTEntries: ";
                int names, entries;
                std::cin >> names;
                std::cin >> entries;
                fs -> format_fs(names, entries);
                break;
            case 'F':
                
                break;
            case 'P':
                std::cout << "Enter name of file: ";
                std::cin >> selection;
                fs -> put(selection);
                break;
            case 'Q':
                exit(EXIT_SUCCESS);
            case 'S':
                    std::cout << "Enter a name for fs: ";
                    std::cin >> selection;
                    fs -> save_fs(selection);
                break;
            case 'O':
                    std::cout << "Enter a name for fs: ";
                    std::cin >> selection;
                    fs -> open_fs(selection);
                break;
            default:
                std::cout << "Invalid Selection. Reenter: ";
                break;
        }
    }
    return EXIT_SUCCESS;
}