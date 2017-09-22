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
bool cont = false;
typedef struct {
    char n; //numerator
    char d; //denominator
}fraction;

int getSign(fraction f) {
    if((f.d > 0 && f.n >0) ||(f.d < 0 && f.n <0 ))
        return 1;
    else if((f.d > 0 && f.n < 0) ||(f.d < 0 && f.n >0 ))
        return -1;
    else{
        return 0;
    }
}
void printMatrix(size_t rows, size_t cols, fraction f[rows][cols], fraction s[rows]) {
    for(i = 0; i < rows; i++){
        for(j=0; j<cols;j++){
            printf(" %d/%d ", f[i][j].n, f[i][j].d);
        }
        printf("\t \t %d/%d \n", s[i].n, s[i].d);
    }
}

void divide(const fraction* f1, const fraction* f2, fraction* f3) {
    f3->n = f1->n * f2->d;
    f3->d = f1->d * f2->n;
}


int fm_stages(size_t rows, size_t cols, fraction matrix[rows][cols], fraction solutions[rows]){
    int n1 = 0;
    int n2 = 0;
    int rowChange;
    //int
    for(i = 0; i <rows; i++) {
        if(getSign(matrix[i][cols-1])>0)
            n1++;
        else if(getSign(matrix[i][cols-1])<0)
            n2++;
    }
    n2 += n1;
    printf("n1 is: %d, n2 is: %d\n", n1, n2);
    fraction temp[cols+1];

    for(i = 0; i<n1;i++) {
        rowChange = 0;
        if(getSign(matrix[i][cols-1])!=1) {
            while(rowChange != -1){
                if(getSign(matrix[n1+rowChange][cols-1]) > 0){
                    for(j = 0; j< cols; j++){
                        temp[j] = matrix[i][j];
                    }
                    temp[cols] = solutions[i];

                    for(j = 0; j<cols;j++) {
                        matrix[i][j] = matrix[n1+rowChange][j];
                    }
                    solutions[i] = solutions[n1+rowChange];

                    for(j = 0; j<cols; j++) {
                        matrix[n1+rowChange][j] = temp[j];
                    }
                    solutions[n1+rowChange] = temp[cols];

                    rowChange = -1;
                }else{
                    rowChange++;
                }
            }
        }
    }
    for(i =0; i< rows;i++){
        printf("%d \n", solutions[i].n);
    }

    for(i = n1; i<n2;i++) {
        rowChange = 0;
        if(getSign(matrix[i][cols-1])!=-1) {
            while(rowChange != -1){
                if(getSign(matrix[n2+rowChange][cols-1]) < 0){
                    for(j = 0; j< cols; j++){
                        temp[j] = matrix[i][j];
                    }
                    temp[cols] = solutions[i];

                    for(j = 0; j<cols;j++) {
                        matrix[i][j] = matrix[n2+rowChange][j];
                    }
                    solutions[i] = solutions[n2+rowChange];

                    for(j = 0; j<cols; j++) {
                        matrix[n2+rowChange][j] = temp[j];
                    }
                    solutions[n2+rowChange] = temp[cols];

                    rowChange = -1;
                }else{
                    rowChange++;
                }
            }
        }
    }
    printf("\n \n");
    printMatrix(rows, cols, matrix, solutions);
    printf("\n \n");
    for(i = 0; i < n2; i++) {
        for(j = 0; j < cols-1; j++) {
            divide(&matrix[i][j], &matrix[i][cols-1], &matrix[i][j]);
        }
        divide(&solutions[i], &matrix[i][cols-1], &solutions[i]);
        matrix[i][cols-1].n = matrix[i][cols-1].d = 1;
    }
    printMatrix(rows, cols, matrix, solutions);
    cont = true;
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
        printf("\t \t %d \n", c[i]);
        solutions[i].n = c[i];
        solutions[i].d = 1;
    }
    while(cont == false) {
        fm_stages(rows, cols, matrix, solutions);
    }

    return 0;
}

int main(int argc, char** argv){
    size_t rows = 6;
    size_t cols = 6;
    signed char a[rows][cols];
    signed char c[rows];

    a[0][0] = -1;
    a[0][1] = -2;
    a[0][2] = 3;
    a[0][3] = -4;
    a[0][4] = 5;
    a[0][5] = -6;
    a[1][0] = -2;
    a[1][1] = 3;
    a[1][2] = -4;
    a[1][3] = 5;
    a[1][4] = -6;
    a[1][5] = 7;
    a[2][0] = -3;
    a[2][1] = -4;
    a[2][2] = 5;
    a[2][3] = -6;
    a[2][4] = 7;
    a[2][5] = -8;
    a[3][0] = -4;
    a[3][1] = 5;
    a[3][2] = -6;
    a[3][3] = 7;
    a[3][4] = -8;
    a[3][5] = 9;
    a[4][0] = -5;
    a[4][1] = -6;
    a[4][2] = 7;
    a[4][3] = -8;
    a[4][4] = 9;
    a[4][5] = -10;
    a[5][0] = -6;
    a[5][1] = 7;
    a[5][2] = -8;
    a[5][3] = 9;
    a[5][4] = -10;
    a[5][5] = 11;


    c[0] = -1;
    c[1] = -2;
    c[2] = -3;
    c[3] = -4;
    c[4] = -5;
    c[5] = -6;



 /*   a[0][0] = -1;
    a[0][1] = -2;
    a[0][2] = 3;
    a[0][3] = -4;
    a[0][4] = 5;
    a[0][5] = 0;
    a[1][0] = -2;
    a[1][1] = 3;
    a[1][2] = -4;
    a[1][3] = 5;
    a[1][4] = -6;
    a[1][5] = 7;
    a[2][0] = -3;
    a[2][1] = -4;
    a[2][2] = 5;
    a[2][3] = -6;
    a[2][4] = 7;
    a[2][5] = 0;
    a[3][0] = -4;
    a[3][1] = 5;
    a[3][2] = -6;
    a[3][3] = 7;
    a[3][4] = -8;
    a[3][5] = 9;
    a[4][0] = -5;
    a[4][1] = -6;
    a[4][2] = 7;
    a[4][3] = -8;
    a[4][4] = 9;
    a[4][5] = -10;
    a[5][0] = -6;
    a[5][1] = 7;
    a[5][2] = -8;
    a[5][3] = 9;
    a[5][4] = -10;
    a[5][5] = 11;


    c[0] = -1;
    c[1] = -2;
    c[2] = -3;
    c[3] = -4;
    c[4] = -5;
    c[5] = -6;*/
    return(fm(rows,cols,a,c));
}
