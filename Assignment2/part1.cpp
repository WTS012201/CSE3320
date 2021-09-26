#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <iostream>

class data{
    private:
        std::string str;
        int num;
    public:
        data(std::string STR, int NUM): str{STR}, num{NUM}{}
        std::string get_str(){  return str;}
        int get_num(){  return num;}
         
};

void load_data(std::ifstream& ifs, std::vector<data>& set){
    std::string rec, num;

    while(std::getline(ifs, rec, ',')){
        std::getline(ifs, num);
        set.push_back(data{rec, std::stoi(num)});
    }
    ifs.close();
}

void place_data(std::ofstream& ofs, std::vector<data>& set){
    for(data d : set){
        ofs << d.get_str() << ",";
        ofs << d.get_num() << std::endl;
    }
    ofs.close();
}

void b_sort(std::vector<data>& set){
    int i, j;
    auto swap = [](data *xp, data *yp)  mutable{
        data temp = *xp;
        *xp = *yp;
        *yp = temp;
    };

    for (i = 0; i < set.size()-1; i++)   
        for (j = 0; j < set.size()-i-1; j++){
            if (set[j].get_num() > set[j+1].get_num())
                swap(&set[j], &set[j+1]);
        }
}

int main(void){
    std::ifstream ifs{"data.csv"};
    std::ofstream ofs{"data_sorted.csv"};
    std::vector<data> set;
    load_data(ifs, set);
    b_sort(set);
    place_data(ofs, set);
    
    return EXIT_SUCCESS;
}