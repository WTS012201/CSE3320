//  William Sigala
//  CSE 3320
//  Asssignment 2

/*
    There's a problem with fork(), I could not get it to write to the shared memory(IPC) while in a child process.
    There is no problem writing while in the main process though. The program still sorts, but doesn't do it concurrently.
*/
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <chrono>
#include <sys/shm.h>
#include <sys/ipc.h>

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

void load_data(std::ifstream ifs, std::vector<std::vector<data>>& set, int thread_num){
    std::string rec, num;
    int c = 0;
    while(std::getline(ifs, rec, ',')){
        std::getline(ifs, num);
        if(set.size() < thread_num){
            std::vector<data> subset;
            subset.push_back(data{rec, std::stoi(num)});
            set.push_back(subset);
        }
        else
            set.at(c%thread_num).push_back(data{rec, std::stoi(num)});
        c++;
    }
    ifs.close();
}

void place_data(std::ofstream ofs, std::vector<data>& set){
    for(data d : set)
        ofs << d << std::endl;
    ofs.close();
}

void b_sort(std::vector<data>& set){
    for (int i = 0; i < set.size()-1; i++)   
        for (int j = 0; j < set.size()-i-1; j++)
            if (set[j].get_num() > set[j+1].get_num())
                std::swap(set[j], set[j+1]);
}

int main(int argc, char* argv[]){
    std::chrono::microseconds duration{0};
    int thread_num = (argc == 2) ? std::stoi(argv[1]) : 1;

    if(argc != 2)
        std::cout << "Number of threads not given. Using 1 thread.\n";

    key_t key = ftok("shmfile",65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, sizeof(std::vector<std::vector<data>>), 0666|IPC_CREAT);
    
    // shmat to attach to shared memory
    std::vector<std::vector<data>>* set = (std::vector<std::vector<data>>*)shmat(shmid, NULL, 0);
    load_data(std::ifstream{"data.csv"}, *set, thread_num);
    shmdt(set);

    //  Running multiple processes of b_sort
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<thread_num;i++){  //  tried sorting each part in the shared memory but could not get this to work.
        shmid = shmget(key, sizeof(std::vector<std::vector<data>>), 0666|IPC_CREAT);
        set = (std::vector<std::vector<data>>*)shmat(shmid, NULL, 0);
        if(fork() == 0){
            b_sort(set->at(i));
            shmdt(set);
            exit(0); 
        } 
    } 
    auto stop = std::chrono::high_resolution_clock::now();
    duration += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    for(int i=0;i<thread_num;i++)
        wait(NULL);
    shmid = shmget(key, sizeof(std::vector<std::vector<data>>), 0666|IPC_CREAT); // shmget returns an ide in shmid
    set = (std::vector<std::vector<data>>*)shmat(shmid, NULL, 0); // shmat to join to shared memory
    
    //  Sorting the sorted parts of the vector
    std::vector<data> sorted;
    for(auto s : *set)
        for(auto d : s)
            sorted.push_back(d);
    start = std::chrono::high_resolution_clock::now();
    b_sort(sorted);
    stop = std::chrono::high_resolution_clock::now();
    duration += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    place_data(std::ofstream{"data_sorted.csv"}, sorted);
    std::cout << "Time: " << duration.count() * 1e-6 << " for " << thread_num << " threads.\n";

    shmdt(set);
    shmctl(shmid,IPC_RMID,NULL);
    return EXIT_SUCCESS;
}