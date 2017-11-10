#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <float.h>
#include <alloca.h>

#define value_of(a) ((a).p / (double) (a).q)

static unsigned long long	fm_count;
static volatile bool		proceed = false;

typedef struct {
    int p;
    int q;
} rational;

int gcd(int a, int b)
{
    return a == 0 ? b : gcd(b % a, a);
}

inline rational add(rational a, rational b)
{
    rational result = {a.p * b.q + b.p * a.q, a.q * b.q};
    return result;
}

inline rational diff(rational a, rational b)
{
    rational result = {a.p * b.q - b.p * a.q, a.q * b.q};
    return result;
}

inline rational divide(rational a, rational b)
{
    rational result = {a.p * b.q, a.q * b.p};
    return result;
}

inline rational multiply(rational a, rational b)
{
    rational result = {a.p * b.p, a.q * b.q};
    return result;
}

void swap_rows(int cols, rational (*matrix)[cols], rational *c, int first_row, int second_row)
{
    for (int i = 0; i < cols; ++i) {
        rational temp = matrix[first_row][i];
        matrix[first_row][i] = matrix[second_row][i];
        matrix[second_row][i] = temp;
    }

    rational temp = c[first_row];
    c[first_row] = c[second_row];
    c[second_row] = temp;
}

inline void reorder_rows(int rows, int cols, rational (*matrix)[cols], rational *c, int r, int* n1, int* n2) {
    int index = 0;

    for (int i = 0; i < rows; ++i) {
        rational a = matrix[i][r];
        if (a.p != 0 && ((a.p < 0 && a.q < 0) || (a.p > 0 && a.q > 0)))
            swap_rows(cols, matrix, c, index++, i);
    }
    *n1 = index;

    for (int i = index; i < rows; ++i) {
        rational a = matrix[i][r];
        if (a.p != 0 && ((a.p > 0 && a.q < 0) || (a.p < 0 && a.q > 0)))
            swap_rows(cols, matrix, c, index++, i);
    }

    *n2 = index;
}

void step_3(int cols, rational (*matrix)[cols], rational *c, int r, int n2)
{
    for (int i = 0; i < n2; ++i) {
        for (int j = 0; j < cols-1; ++j) {
            matrix[i][j] = divide(matrix[i][j], matrix[i][r-1]);
        }
        c[i] = divide(c[i], matrix[i][r-1]);
        matrix[i][r-1] = (rational){1, 1};
    }
}

void step_4(rational *c, int n1, int n2, rational* br, rational* Br)
{
    *br = (rational){INT_MIN, 1};

    if (n2 > n1) {
        double max_value = -DBL_MAX;
        rational max_rational;
        bool max_set = false;
        for (int j = n1+1; j <= n2; ++j) {
            if (value_of(c[j-1]) > max_value) {
                max_value = value_of(c[j-1]);
                max_rational = c[j-1];
                max_set = true;
            }
        }
        if (max_set)
            *br = max_rational;
    }

    *Br = (rational){INT_MAX, 1};
    if (n1 > 0) {

        double min_value = DBL_MAX;
        rational min_rational;
        bool min_set = false;

        for (int j = 1; j <= n1; ++j) {
            if (value_of(c[j-1]) < min_value) {
                min_value = value_of(c[j-1]);
                min_rational = c[j-1];
                min_set = true;
            }
        }
        if (min_set)
            *Br = min_rational;
    }
}

inline int get_solution(int s, rational c[s], int n2, rational b, rational B) {
    if (value_of(b) > value_of(B))
        return false;

    for (int j = n2+1; j <= s; ++j)
        if (value_of(c[j-1]) < 0)
            return false;
    return true;
}

bool fm_elim(int rows, int cols, int a[rows][cols], int c[])
{
    int s = rows;
    int r = cols;

    rational (*start_matrix)[r] = alloca(s * r * sizeof(rational));

    rational *q = alloca(s * sizeof(rational));

    for (int i = 0; i < s; ++i) {
        for (int j = 0; j < r; ++j)
            start_matrix[i][j] = (rational){a[i][j], 1};
        q[i] = (rational){c[i], 1};
    }

    void *next_matrix_ptr = (void *)start_matrix;
    while (1) {		int n1;
        int n2;
        rational (*T)[r] = next_matrix_ptr;

        reorder_rows(s, r, T, q, r - 1, &n1, &n2);
        step_3(r, T, q, r, n2);


        if (r == 1) {
            rational br, Br;
            step_4(q, n1, n2, &br, &Br);
            if (get_solution(s, q, n2, br, Br))
                return true;
            return false;
        }


        int s_prime = s - n2 + n1*(n2 - n1);
        if (s_prime == 0)
            return true;

        rational (*U)[r-1] = alloca(s_prime * (r-1) * sizeof(rational));
        rational *q_next = alloca(s_prime * sizeof(rational));


        int current_row = 0;

        for (int k = 1; k <= n1; k++) {
            for (int l = n1 + 1; l <= n2; l++) {
                for (int i = 1; i < r; i++) {
                    U[current_row][i-1] = diff(T[k-1][i-1], T[l-1][i-1]);
                }
                q_next[current_row] = diff(q[k-1], q[l-1]);
                current_row++;
            }
        }

        for (int j = n2 + 1; j <= s; j++) {
            for (int i = 1; i < r; i++) {
                U[current_row][i-1] = T[j-1][i-1];

            }
            q_next[current_row++] = q[j-1];
        }

        next_matrix_ptr = (void *)U;
        q = q_next;
        r--;
        s = s_prime;
    }
}

static void done(int unused)
{
    proceed = false;
    unused = unused;
}

unsigned long long tpi14lho(char* aname, char* cname, int seconds)
{
    FILE*		afile = fopen(aname, "r");
    FILE*		cfile = fopen(cname, "r");

    fm_count = 0;

    if (afile == NULL) {
        fprintf(stderr, "could not open file A\n");
        exit(1);
    }

    if (cfile == NULL) {
        fprintf(stderr, "could not open file c\n");
        exit(1);
    }

    /* read A and c files. */
    int rows;
    int cols;
    int i, j;

    assert(fscanf(afile, "%d %d\n", &rows, &cols) == 2);
    int a[rows][cols];

    for (i = 0; i < rows; ++i)
        for (j = 0; j < cols; ++j)
            assert(fscanf(afile, "%d", &a[i][j]) == 1);

    assert(fscanf(cfile, "%d", &rows) == 1);
    int c[rows];
    for (i = 0; i < rows; ++i)
        assert(fscanf(cfile, "%d", &c[i]) == 1);

    /* Done reading */

    fclose(afile);
    fclose(cfile);

    if (seconds == 0) {
        /* Just run once for validation. */
        return fm_elim(rows, cols, a, c);
    }

    /* Tell operating system to call function DONE when an ALARM comes. */
    signal(SIGALRM, done);
    alarm(seconds);

    /* Now loop until the alarm comes... */
    proceed = true;
    while (proceed) {
        fm_elim(rows, cols, a, c);
        fm_count++;
    }

    return fm_count;
}
