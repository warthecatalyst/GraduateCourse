#pragma once
#include <vector>
#include <iostream>

class EightQueens {
private:
    int n;
    std::vector<int> col;   //列有没有被占用
    std::vector<int> diagram;   //左上到右下的主对角线有没有被占用
    std::vector<int> anti_diagram;  //右上到左下的反对角线有没有被占用
    std::vector<int> queens;    //皇后摆在哪一个
    int cnt;
public:
    EightQueens() :n(1), cnt(0) {
        col.resize(1, 0);
        diagram.resize(1, 0);
        anti_diagram.resize(1, 0);
        queens.resize(1, 0);
    }

    EightQueens(int _n) :n(_n), cnt(0) {
        col.resize(n, 0);
        diagram.resize(2 * n - 1, 0);
        anti_diagram.resize(2 * n - 1, 0);
        queens.resize(n, 0);
    }
    //backtrack to solve eightQueens
    void Solve(int i);
    //to print the answer
    void print();
    //to print the number of results
    void printnum() const;
};

