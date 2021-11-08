// SuperMarketModel.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
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

static vector<int> nVals = { 5,8 };
static vector<vector<int>> dVals = { {1,2,5},{1,2,4,8} };

void simulation(int n, int d, int total);   //n servers and d choices and total tasks
vector<double> servingTime;     //the serving time of a server
vector<bool> isExcecuted;       //if a task has been executed
void thread_execute(int threadId);  //tasks for a thread to execute
void task_execute(int id, int n);        //the formal definition of a task
void taskCreation(int n, int d, int total);            //create d tasks
vector<ThreadSafeQueue> thread_tasks;

int main() {
    for (int i = 0; i < 2; i++) {
        int n = nVals[i];
        for (int j = 0; j < dVals[i].size(); j++) {
            int d = dVals[i][j];
            for (int k = 1; k <= 10; k++) {
                int tt = 500 * k;
                simulation(n, d, tt);
            }
        }
    }
    return 0;
}

void simulation(int n, int d, int total) {
    //initialization
    srand(time(NULL));
    servingTime.resize(n, 0);
    isExcecuted.resize(total, false);
    thread_tasks.resize(n,ThreadSafeQueue());
    //spawning n threads
    vector<thread> threads(n);
    for (int i = 0; i < n; i++) {
        threads[i] = thread(thread_execute, i);
    }
    threads[n] = thread(taskCreation, n, d, total);
    for (int i = 0; i <= n; i++) {
        threads[i].join();
    }
    //system("pause");
    double maxTime = *std::max_element(servingTime.begin(), servingTime.end());
    string outFile = "testOut.txt";
    ofstream outF(outFile, ios::app);
    outF << n << "\t" << d << "\t" << total << "\t" << maxTime << endl;
}

void thread_execute(int threadID) {
    //start clock
    clock_t startTime = clock();
    while (!thread_tasks[threadID].empty()) {
        task_des task{};
        thread_tasks[threadID].wait_and_pop(task);
        task_execute(task.id, task.difficulty);
    }
    //end clock
    clock_t endTime = clock();
    servingTime[threadID] = (double)(endTime - startTime) / CLOCKS_PER_SEC;
}

void task_execute(int id, int n) {
    if (isExcecuted[id]) {
        return;
    }
    isExcecuted[id] = true;
    EightQueens eightQueens(n);
    eightQueens.Solve(0);
}

void taskCreation(int n, int d, int total) {
    for (int i = 0; i < total; i++) {
        //randomly choose a difficulty
        int dif = rand() % 7;
        dif += 4;

        //randomly choose d servers
        unordered_set<int> choices;
        while (choices.size() < d) {
            int choose = rand() % n;
            choices.insert(choose);
        }
        for (int choice : choices) {
            task_des temp{};
            temp.id = i; temp.difficulty = dif;
            thread_tasks[choice].push(temp);
        }
    }
}