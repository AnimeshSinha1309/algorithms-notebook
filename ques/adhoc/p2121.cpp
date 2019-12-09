/**
 * @Problem
 * A hexadecimal representation of the walls around each cell of a maze are
 * given. Print the visualization of this grid.
 * @Reference
 * https://icpcarchive.ecs.baylor.edu/index.php?option=onlinejudge&page=show_problem&problem=122
 * implementation
 */

#include <iostream>
#include <stack>
using namespace std;

enum Wall { PRESENT, ABSENT, CONFLICT };
class Cell {
   public:
    Wall top, bottom, left, right;
    Cell() {
        top = bottom = left = right = CONFLICT;
    }
    explicit Cell(int code) {
        top = (code & 0b0001) == 0 ? PRESENT : ABSENT;
        right = (code & 0b0010) == 0 ? PRESENT : ABSENT;
        bottom = (code & 0b0100) == 0 ? PRESENT : ABSENT;
        left = (code & 0b1000) == 0 ? PRESENT : ABSENT;
    }
    void pTop() {
        if (top == PRESENT)
            cout << "+---";
        else if (top == ABSENT)
            cout << "+   ";
        else if (top == CONFLICT)
            cout << "+xxx";
    }
    void pLeft() {
        if (left == PRESENT)
            cout << "|";
        else if (left == ABSENT)
            cout << " ";
        else if (left == CONFLICT)
            cout << "X";
    }
    void pBottom() {
        if (bottom == PRESENT)
            cout << "+---";
        else if (bottom == ABSENT)
            cout << "+   ";
        else if (bottom == CONFLICT)
            cout << "+xxx";
    }
    void pRight() {
        if (right == PRESENT)
            cout << "|";
        else if (right == ABSENT)
            cout << " ";
        else if (right == CONFLICT)
            cout << "X";
    }
};

int main() {
    for (int i = 0;; i++) {
        int nr, nc, temp;
        cin >> nr >> nc;
        Cell grid[nr][nc];
        if (nr == 0 && nc == 0)
            break;
        // Take the Input and initialize the Cells
        for (int i = 0; i < nr; i++)
            for (int j = 0; j < nc; j++) {
                cin >> hex >> temp;
                grid[i][j] = Cell(temp);
            }
        // Generate all the Conflicts
        for (int i = 0; i < nr; i++)
            for (int j = 0; j < nc; j++) {
                if (i > 0 && grid[i][j].top != grid[i - 1][j].bottom)
                    grid[i][j].top = CONFLICT;
                if (j > 0 && grid[i][j].left != grid[i][j - 1].right)
                    grid[i][j].left = CONFLICT;
                if (i < nr - 1 && grid[i][j].bottom != grid[i + 1][j].top)
                    grid[i][j].bottom = CONFLICT;
                if (j < nc - 1 && grid[i][j].right != grid[i][j + 1].left)
                    grid[i][j].right = CONFLICT;
            }
        // Printing out the results
        for (int i = 0; i < nr; i++) {
            for (int j = 0; j < nc; j++)
                grid[i][j].pTop();
            cout << "+" << endl;
            for (int j = 0; j < nc; j++) {
                grid[i][j].pLeft();
                cout << "   ";
            }
            grid[i][nc - 1].pRight();
            cout << endl;
        }
        for (int j = 0; j < nc; j++)
            grid[nr - 1][j].pBottom();
        cout << "+" << endl;
    }
}
