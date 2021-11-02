#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <unordered_set>
#include "EightQueens.h"

using namespace std;

struct task_des{
    int id,difficulty;
};

static vector<int> nVals = {5,8};
static vector<vector<int>> dVals = {{1,2,5},{1,2,4,8}};

void simulation(int n,int d,int total);   //n servers and d choices and total tasks
vector<double> servingTime;     //the serving time of a server
vector<bool> isExcecuted;       //if a task has been executed
//vector<vector<task_des>> thread_tasks;
void thread_execute(int threadId,vector<task_des>& tasks);  //tasks for a thread to execute
void task_execute(int id,int n);        //the formal definition of a task

int main() {
    for(int i=0;i<2;i++){
        int n = nVals[i];
        for(int j=0;j<dVals[i].size();j++){
            int d = dVals[i][j];
            for(int k=1;k<=10;k++){
                int tt = 500*k;
                simulation(n,d,tt);
            }
        }
    }
    return 0;
}

void simulation(int n,int d,int total){
    srand(time(NULL));
    servingTime.resize(n,0);
    isExcecuted.resize(total, false);
    //thread_tasks.resize(n);
    vector<vector<task_des>> thread_tasks(n);
    for(int i=0;i<total;i++){
        //randomly choose a difficulty
        int dif = rand()%7;
        dif+=4;

        //randomly choose d servers
        unordered_set<int> choices;
        while(choices.size()<d){
            int tmp = rand()%n;
            choices.insert(tmp);
        }
        unsigned int choose = -1,min_task = 0;
        for(int choice:choices){
            //choose a thread with least tasks
            if(choose==-1||thread_tasks[choice].size()<min_task){
                choose = choice;
                min_task = thread_tasks[choice].size();
            }
        }
        thread_tasks[choose].push_back({i,dif});
    }
//    for(auto tasks:thread_tasks){
//        for(task_des task:tasks){
//            cout<<task.id<<" ";
//        }
//        cout<<endl;
//    }
    //spawning n threads
    vector<thread> threads(n);
    for(int i=0;i<n;i++){
        threads[i] = thread(thread_execute,i,thread_tasks[i]);
    }
    for(int i=0;i<n;i++){
        threads[i].join();
    }
    //system("pause");
    double maxTime = *std::max_element(servingTime.begin(),servingTime.end());
    string outFile = "testOut.txt";
    ofstream outF(outFile,ios::app);
    outF<<n<<"\t"<<d<<"\t"<<total<<"\t"<<maxTime<<endl;
}

void thread_execute(int threadID,vector<task_des>& tasks){
    //auto tasks = thread_tasks[threadID];
    //start clock
    clock_t startTime = clock();
    for(auto task:tasks){
        task_execute(task.id,task.difficulty);
    }
    clock_t endTime = clock();
    servingTime[threadID] = (double)(endTime-startTime)/CLOCKS_PER_SEC;
}

void task_execute(int id,int n){
    if(isExcecuted[id]){
        return;
    }
    std::cout<<"task id: " << id <<" difficulty: "<<n<<std::endl;
    EightQueens eightQueens(n);
    eightQueens.Solve(0);
    eightQueens.printnum();
    isExcecuted[id] = true;
}
