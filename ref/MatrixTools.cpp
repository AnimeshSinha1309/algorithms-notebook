// solving systems of linear equations(AX = B)
// (2) inverting matrices(AX = I)
// (3) computing determinants of square matrices
// O(nˆ3)
// INPUT : a[][] = an nxn matrix; b[][] = an nxm matrix
// OUTPUT:
//  X = an nxm matrix(stored in b[][])
//  Aˆ{-1} = an nxn matrix(stored in a[][])
// returns determinant of a[][]

const double EPS = 1e-10;
// T is data type of matrix elements
T GaussJordan(VVT &a, VVT &b) {
    const int n = a.size();
    const int m = b[0].size();
    VI irow(n), icol(n), ipiv(n);
    T det = 1;
    for (int i = 0; i < n; i++) {
        int pj = -1, pk = -1;
        for (int j = 0; j < n; j++)
            if (!ipiv[j])
                for (int k = 0; k < n; k++)
                    if (!ipiv[k])
                        if (pj == -1 || fabs(a[j][k]) > fabs(a[pj][pk])) {
                            pj = j;
                            pk = k;
                        }
        if (fabs(a[pj][pk]) < EPS) {
            cerr << "Matrix is singular." << endl;
            exit(0);
        }
        ipiv[pk]++;
        swap(a[pj], a[pk]);

        swap(b[pj], b[pk]);
        if (pj != pk)
            det *= -1;
        irow[i] = pj;
        icol[i] = pk;
        T c = 1.0 / a[pk][pk];
        det *= a[pk][pk];
        a[pk][pk] = 1.0;
        for (int p = 0; p < n; p++)
            a[pk][p] *= c;
        for (int p = 0; p < m; p++)
            b[pk][p] *= c;
        for (int p = 0; p < n; p++)
            if (p != pk) {
                c = a[p][pk];
                a[p][pk] = 0;
                for (int q = 0; q < n; q++)
                    a[p][q] -= a[pk][q] * c;
                for (int q = 0; q < m; q++)
                    b[p][q] -= b[pk][q] * c;
            }
    }
    for (int p = n - 1; p >= 0; p--)
        if (irow[p] != icol[p]) {
            for (int k = 0; k < n; k++)
                swap(a[k][irow[p]], a[k][icol[p]]);
        }
    return det;
}// gets the rank of a matrix.
// Running time: O(nˆ3)
// INPUT: a[][] = an nxm matrix
// OUTPUT: rref[][] = an nxm matrix (stored in a[][])
// returns rank of a[][]

int rref(VV &a) {
    int n = a.size(), m = a[0].size(), r = 0;

    for (int c = 0; c < m && r < n; c++) {
        int j = r;
        for (int i = r + 1; i < n; i++)
            if (fabs(a[i][c]) > fabs(a[j][c]))
                j = i;

        if (fabs(a[j][c]) < EPS)
            continue;
        swap(a[j], a[r]);

        T s = 1.0 / a[r][c];

        for (int j = 0; j < m; j++)
            a[r][j] *= s;

        for (int i = 0; i < n; i++) {
            if (i != r) {
                T t = a[i][c];
                for (int j = 0; j < m; j++) {
                    a[i][j] -= t * a[r][j];
                }
            }
        }

        r++;
    }

    return r;
}// Solves LP with constraints cˆT x, Ax <= b, x >= 0
// A: m x n matrix
// b: m-dimensional vector
// c: n-dimensional vector
// x: a vector where the optimal solution will be stored
// OUTPUT: value of the optimal solution (infinity if unbounded
// above, nan if infeasible)
// To use this code, create an LPSolver object with A, b, and c as
// arguments. Then, call Solve(x)

struct LPSolver {
    int m, n;
    VI B, N;
    VVD D;
    LPSolver(const VVD &A, const VD &b, const VD &c)
        : m(b.size()), n(c.size()), N(n + 1), B(m), D(m + 2, VD(n + 2)) {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                D[i][j] = A[i][j];
        for (int i = 0; i < m; i++) {
            B[i] = n + i;
            D[i][n] = -1;
            D[i][n + 1] = b[i];
        }
        for (int j = 0; j < n; j++) {
            N[j] = j;
            D[m][j] = -c[j];
        }
        N[n] = -1;
        D[m + 1][n] = 1;
    }
    void Pivot(int r, int s) {
        double inv = 1.0 / D[r][s];
        for (int i = 0; i < m + 2; i++)
            if (i != r)
                for (int j = 0; j < n + 2; j++)
                    if (j != s)
                        D[i][j] -= D[r][j] * D[i][s] * inv;
        for (int j = 0; j < n + 2; j++)
            if (j != s)
                D[r][j] *= inv;
        for (int i = 0; i < m + 2; i++)
            if (i != r)
                D[i][s] *= -inv;
        D[r][s] = inv;
        swap(B[r], N[s]);
    }
    bool Simplex(int phase) {
        int x = phase == 1 ? m + 1 : m;
        while (true) {
            int s = -1;
            for (int j = 0; j <= n; j++) {
                if (phase == 2 && N[j] == -1)
                    continue;
                if (s == -1 || D[x][j] < D[x][s] ||
                    D[x][j] == D[x][s] && N[j] < N[s])
                    s = j;
            }
            if (D[x][s] > -EPS)
                return true;
            int r = -1;
            for (int i = 0; i < m; i++) {
                if (D[i][s] < EPS)
                    continue;
                if (r == -1 || D[i][n + 1] / D[i][s] < D[r][n + 1] / D[r][s] ||
                    (D[i][n + 1] / D[i][s]) == (D[r][n + 1] / D[r][s]) &&
                        B[i] < B[r])
                    r = i;
            }
            if (r == -1)
                return false;
            Pivot(r, s);
        }
    }
    DOUBLE Solve(VD &x) {
        int r = 0;
        for (int i = 1; i < m; i++)
            if (D[i][n + 1] < D[r][n + 1])
                r = i;
        if (D[r][n + 1] < -EPS) {
            Pivot(r, n);
            if (!Simplex(1) || D[m + 1][n + 1] < -EPS)
                return -numeric_limits<DOUBLE>::infinity();
            for (int i = 0; i < m; i++)
                if (B[i] == -1) {
                    int s = -1;
                    for (int j = 0; j <= n; j++)
                        if (s == -1 || D[i][j] < D[i][s] ||
                            D[i][j] == D[i][s] && N[j] < N[s])
                            s = j;
                    Pivot(i, s);
                }
        }
        if (!Simplex(2))
            return numeric_limits<DOUBLE>::infinity();
        x = VD(n);
        for (int i = 0; i < m; i++)
            if (B[i] < n)
                x[B[i]] = D[i][n + 1];
        return D[m][n + 1];
    }
};