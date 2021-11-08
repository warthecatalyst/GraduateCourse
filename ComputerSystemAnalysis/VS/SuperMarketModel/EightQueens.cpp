#include "EightQueens.h"

void EightQueens::Solve(int i) {
    if (i == n) {
        cnt++;
        //print();  in order not to seize IO, note off if you want to verify the answers
        return;
    }
    for (int j = 0; j < n; j++) {
        if (col[j] == 0 && diagram[n - 1 + j - i] == 0 && anti_diagram[i + j] == 0) {
            //std::cout<<i<<" "<<j<<std::endl;
            queens[i] = j;
            col[j] = 1;
            diagram[n - 1 + j - i] = 1;
            anti_diagram[i + j] = 1;
            Solve(i + 1);
            col[j] = 0;
            diagram[n - 1 + j - i] = 0;
            anti_diagram[i + j] = 0;
        }
    }
}

void EightQueens::print() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (queens[i] == j) {
                std::cout << 'Q';
            }
            else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

void EightQueens::printnum() const {
    std::cout << "the result number of n = " << n << " is ";
    std::cout << cnt << std::endl;
}
