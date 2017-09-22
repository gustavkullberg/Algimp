#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <math.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

int i = 0;
int j = 0;
typedef struct {
    char n; //numerator
    char d; //denominator
}fraction;




int fm_stages(size_t rows, size_t cols, fraction matrix[rows][cols], fraction solutions[rows]){

    return 0;
}

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{
    fraction matrix[rows][cols];
    fraction solutions[rows];

    for(i=0; i<rows;i++) {
        for(j=0; j<cols; j++) {
            printf("%d ", a[i][j]);
            matrix[i][j].n = a[i][j];
            matrix[i][j].d = 1;
        }
        printf("\t%d \n", c[i]);
        solutions[i].n = c[i];
        solutions[i].d = 1;
    }
    fm_stages(rows, cols, matrix, solutions);
    return 0;
}

int main(int argc, char** argv){
    size_t rows = 4;
    size_t cols = 2;
    signed char a[rows][cols];
    signed char c[rows];

    a[0][0] = 2;
    a[0][1] = -11;
    a[1][0] = -3;
    a[1][1] = 2;
    a[2][0] = 1;
    a[2][1] = 3;
    a[3][0] = -2;
    a[3][1] = 0;

    c[0] = 3;
    c[1] = -5;
    c[2] = 4;
    c[3] = -3;
    return(fm(rows,cols,a,c));
}
