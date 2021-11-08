#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <cmath>
#include <deque>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <unordered_set>
#include "EightQueens.h"
#include "ThreadSafeQueue.h"
#include "task_des.h"

using namespace std;

static vector<int> nVals = {5,8};
static vector<vector<int>> dVals = {{1,2,5},{1,2,4,8}};

void simulation(int n,int d,int total);   //n servers and d choices and total tasks
vector<double> servingTime;     //the serving time of a server
vector<bool> isExcecuted;       //if a task has been executed
void thread_execute(int threadId);  //tasks for a thread to execute
void task_execute(int id,int n);        //the formal definition of a task
void taskCreation(int n,int d,int total);            //create d tasks
vector<ThreadSafeQueue> thread_tasks;

int main() {
    srand(time(NULL));
    for(int i=0;i<2;i++){
        int n = nVals[i];
        for(int d : dVals[i]){
            for(int k=1;k<=10;k++){
                int tt = 500*k;
                simulation(n,d,tt);
            }
        }
    }
    return 0;
}

void simulation(int n,int d,int total){
    //initialization
    servingTime.clear();
    servingTime.resize(n+1,0);
    isExcecuted.clear();
    isExcecuted.resize(total, false);
    thread_tasks.clear();
    thread_tasks.resize(n);
    //cout<<"log 1"<<endl;
    //spawning n server threads and a creation thread
    vector<thread> threads(n+1);
    //initialize the creation thread first
    threads[n] = thread(taskCreation,n,d,total);
    //initialize n threads to calculate
    for(int i=0;i<n;i++){
        threads[i] = thread(thread_execute,i);
    }

    for(int i=0;i<=n;i++){
        threads[i].join();
    }
    //wait for threads to end
    cout<<"All threads ended, collecting results"<<endl;
    for(auto t:servingTime){
        cout<<t<<endl;
    }
    //it's ok because task creating time can't be longer than task processing time
    double maxTime = *std::max_element(servingTime.begin(),servingTime.end());
    double averageTime = 0;
    for(double val:servingTime){
        averageTime+=val;
    }
    averageTime /= n*d;
    maxTime -= servingTime.back();
    maxTime /= d;
    string outFile = "testOut.txt";
    ofstream outF(outFile,ios::app);
    outF<<n<<"\t"<<d<<"\t"<<total<<"\t"<<maxTime<<"\t"<<averageTime<<endl;
}

void thread_execute(int threadID){
    //start clock
    clock_t startTime = clock();
    while(!thread_tasks[threadID].empty()){
        task_des task{};
        thread_tasks[threadID].wait_and_pop(task);
        task_execute(task.id,task.difficulty);
    }
    //end clock
    clock_t endTime = clock();
    servingTime[threadID] = (double)(endTime-startTime)/CLOCKS_PER_SEC;
}

void task_execute(int id,int n){
    if(isExcecuted[id]){
        return;
    }
    // cout<<"task id: "<<id<<" task difficulty: "<< n <<endl;
    // consuming IO tasks
    isExcecuted[id] = true;
    EightQueens eightQueens(n);
    eightQueens.Solve(0);
}

void taskCreation(int n,int d,int total){
    //calculate the task creation time and minus it
    clock_t startTime = clock();
    for(int i=0;i<total;i++){
        //randomly choose a difficulty
        int dif = rand()%7;
        dif+=4;

        //randomly choose d servers
        unordered_set<int> choices;
        while(choices.size()<d){
            int choose = rand()%n;
            choices.insert(choose);
        }
        for(int choice:choices){
            task_des temp{};
            temp.id = i; temp.difficulty = dif;
            thread_tasks[choice].push(temp);
        }
    }
    clock_t endTime = clock();
    servingTime[n] = (double)(endTime-startTime)/CLOCKS_PER_SEC;
}