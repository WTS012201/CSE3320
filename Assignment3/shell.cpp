#include "fs_ops.h"

int main(int argv, char* argsv[]){
    std::string selection;
    auto fs = new FSManage{};
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
                std::cin >> names >> entries;
                fs -> format_fs(names, entries);
                break;
            case 'R':
                std::cout << "Enter name of file: ";
                std::cin >> selection;
                fs -> remove(selection);
                break;
            case 'P':
                std::cout << "Enter name of file: ";
                std::cin >> selection;
                fs -> put(selection);
                break;
            case 'G':
                std::cout << "Enter name of file: ";
                std::cin >> selection;
                fs -> get(selection);
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
            case 'L':
                fs -> list_fs();
                break;
            default:
                std::cout << "Invalid Selection. Reenter: ";
                break;
        }
    }
    return EXIT_SUCCESS;
}