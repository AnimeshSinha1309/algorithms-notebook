/**
 * @Problem
 * See if flipping ith row with ith column can convert one matrix to another
 * given matrix.
 * @Reference
 * https://www.codechef.com/problems/ADAMTR
 * 2-SAT, math, matrices
 */

#include <iostream>
using namespace std;

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        int matA[n][n], matB[n][n];
        bool swap[n][n], solvable = true;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                cin >> matA[i][j];
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                cin >> matB[i][j];
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                if (matA[i][j] == matB[i][j] && matA[j][i] == matB[j][i])
                    swap[i][j] = swap[j][i] = false;
                else if (matA[i][j] == matB[j][i] && matA[j][i] == matB[i][j])
                    swap[i][j] = swap[j][i] = true;
                else
                    solvable = false;
            }
        }
        if (!solvable) {
            cout << "No" << endl;
            continue;
        }
        bool flip[n], ans = true;
        flip[0] = false;
        for (int i = 0; i < n; i++)
            flip[i] = (swap[0][i] != flip[0]);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (swap[i][j] == (flip[i] == flip[j]))
                    ans = false;
        cout << (ans ? "Yes" : "No") << endl;
    }
}