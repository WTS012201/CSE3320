//  William Sigala
//  CSE 3320
//  Asssignment 2
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <thread>
#include <chrono>

class data{
    private:
        std::string str;
        int num;
    public:
        data(std::string STR, int NUM): str{STR}, num{NUM}{}
        std::string get_str(){  return str;}
        int get_num() {  return num;}
        friend std::ostream& operator<<(std::ostream& os, const data& d){
            os << d.str << ',' << d.num;
            return os;
        }
};

typedef std::vector<data> data_set;

void load_data(std::ifstream ifs, std::vector<data_set>& set, int thread_num){
    std::string rec, num;
    int c = 0;
    while(std::getline(ifs, rec, ',')){
        std::getline(ifs, num);
        if(set.size() < thread_num){
            data_set subset;
            subset.push_back(data{rec, std::stoi(num)});
            set.push_back(subset);
        }
        else
            set.at(c%thread_num).push_back(data{rec, std::stoi(num)});
        c++;
    }
    ifs.close();
}

void place_data(std::ofstream ofs, data_set& set){
    for(data d : set)
        ofs << d << std::endl;
    ofs.close();
}

void b_sort(data_set& set){
    for (int i = 0; i < set.size()-1; i++)   
        for (int j = 0; j < set.size()-i-1; j++)
            if (set[j].get_num() > set[j+1].get_num())
                std::swap(set[j], set[j+1]);
}
/// As instructed, merges the sorted partitions
data_set merge(data_set& set1, data_set& set2){
    int i = 0, j = 0;
    data_set sorted;

    while(i != set1.size() && j != set2.size()){
        if(set1[i].get_num() < set2[j].get_num())
            sorted.push_back(set1[i++]);
        else
            sorted.push_back(set2[j++]);
    }
    while(i != set1.size())
        sorted.push_back(set1[i++]);
    while(j != set2.size())
        sorted.push_back(set2[j++]);
    return sorted;
}

int main(int argc, char* argv[]){
    std::vector<data_set> set;
    std::chrono::microseconds duration{0};
    int thread_num = (argc == 2) ? std::stoi(argv[1]) : 1;

    if(argc != 2)
        std::cout << "Number of threads not given. Using 1 thread.\n";
    load_data(std::ifstream{"data.csv"}, set, thread_num);

    //  Sorting parts of the array for each thread
    std::vector<std::thread> threads; 
    for(int i = 0; i < thread_num; i++)
        threads.push_back(std::thread{b_sort, std::ref(set[i])});
    
    //  Joining the threads
    auto start = std::chrono::high_resolution_clock::now();
    for(std::thread& t : threads) t.join();
    auto stop = std::chrono::high_resolution_clock::now();
    duration += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    //  Sorting the sorted parts of the array
    start = std::chrono::high_resolution_clock::now();
    while(set.size() > 1){
        set.push_back(merge(set[0], set[1]));
        set.erase(set.begin(), set.begin() + 2);
    }
    stop = std::chrono::high_resolution_clock::now();
    duration += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    place_data(std::ofstream{"data_sorted.csv"}, set[0]);
    std::cout << "Time: " << duration.count() * 1e-6 << " for " << thread_num << " threads.\n";
    return EXIT_SUCCESS;
}