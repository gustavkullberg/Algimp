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
#include <limits.h>

int i = 0;
int j = 0;
int k = 0;
int l = 0;
bool cont = false;
typedef struct {
    int n; //numerator
    int d; //denominator
}fraction;
fraction br;
fraction Br;
fraction max;
fraction min;
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
void subtract(const fraction* f1, const fraction* f2, fraction* f3) {
    f3->d = f1->d * f2->d;
    f3->n = (f1->n*f2->d - f2->n*f1->d);
}

int gcd(int u, int v)
{
    if(u<0)
	u=-u;
    if(v<0)
	v=-v;
    if (u == v)
        return u;

    if (u == 0)
        return v;

    if (v == 0)
        return u;

    // look for factors of 2
    if (~u & 1) // u is even
    {
        if (v & 1) // v is odd
            return gcd(u >> 1, v);
        else // both u and v are even
            return gcd(u >> 1, v >> 1) << 1;
    }

    if (~v & 1) // u is odd, v is even
        return gcd(u, v >> 1);

    // reduce larger argument
    if (u > v)
        return gcd((u - v) >> 1, v);

    return gcd((v - u) >> 1, u);

}

fraction shorten(fraction f)
{
    int div = gcd(f.n, f.d);
    f.n = f.n/div;
    f.d = f.d/div;
    return(f);
}

fraction cmpMax(fraction f1, fraction f2) {
    if((f1.d <0 && f2.d < 0) ||(f1.d>=0 && f2.d >=0)  ){
        if(f1.n*f2.d > f2.n*f1.d){
            return f1;
        }
        return f2;
    }else{
        if(f1.n*f2.d < f2.n*f1.d){
            return f1;
        }
        return f2;
    }
}

fraction cmpMin(fraction f1, fraction f2) {
    if((f1.d <0 && f2.d < 0) ||(f1.d>=0 && f2.d >=0)  ){
        if(f1.n*f2.d < f2.n*f1.d){
            return f1;
        }
        return f2;
    }else{
        if(f1.n*f2.d > f2.n*f1.d){
            return f1;
        }
        return f2;
    }
}

fraction getMax(size_t size, fraction fracs[size]) {
    max.n = fracs[0].n;
    max .d = fracs[0].d;
    for(j = 1; j<size; j++) {
        max = cmpMax(max, fracs[j]);
    }
    return max;
}

fraction getMin(size_t size, fraction fracs[size]) {
    min.n = fracs[0].n;
    min.d = fracs[0].d;
    for(j = 1; j<size; j++) {
        min = cmpMin(min, fracs[j]);
    }
    return min;
}
bool makeSolution(fraction Br, fraction br, int size, fraction solutions[size], int n2 ) {
    if(size == n2) {
        if((cmpMax(Br, br).n == Br.n) && (cmpMax(Br, br).d == Br.d))
            return 1;
        else
            return 0;
    }else {
        min = solutions[n2];
        for (j = n2+1; j < size; j++) {
            min = cmpMin(min, solutions[j]);
        }
        if(min.n>=0 && (cmpMax(Br, br).n == Br.n) && (cmpMax(Br, br).d == Br.d))
            return 1;
        else
            return 0;
    }
}



int fm_stages(size_t rows, size_t cols, fraction matrix[rows][cols], fraction solutions[rows]){
    int n1 = 0;
    int n2 = 0;
    int rowChange;
    int g;
    printf("\n\n\nStarting new fm_stages \n");
    for(i = 0; i <rows; i++) {
        if(getSign(matrix[i][cols-1])>0) {
            //printf("%d/%d was positive! \n", matrix[i][cols-1].n, matrix[i][cols-1].d);
            //printf("matrix[1][0] = %d/%d \n", matrix[1][0].n, matrix[1][0].d);
            //printf("indexes are: i=%d, cols-1=%lu \n", i, cols-1);
            n1++;
        }
        else if(getSign(matrix[i][cols-1])<0) {
            //printf("%d/%d was negative! \n", matrix[i][cols - 1].n, matrix[i][cols - 1].d);
            n2++;
        }
    }
    n2 += n1;
    printf("Rows are: %zu, Cols are: %zu \n", rows, cols);
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
    printf("\nThis is after rearrange: \n");
	printMatrix(rows, cols, matrix, solutions);
    for(i = 0; i < n2; i++) {
        for(j = 0; j < cols-1; j++) {
            divide(&matrix[i][j], &matrix[i][cols-1], &matrix[i][j]);
	    matrix[i][j] = shorten(matrix[i][j]);
        }
        divide(&solutions[i], &matrix[i][cols-1], &solutions[i]);
	solutions[i] = shorten(solutions[i]);
        matrix[i][cols-1].n = matrix[i][cols-1].d = 1;
    }
    if(!(cols > 1)) {
        //Check if lösbart
        if(n2>n1) {
            br = solutions[n1];
            for(j = n1+1; j<n2; j++){
                br = cmpMax(br,solutions[j]);
            }
        }else{
            br.n = INT_MIN;
            br.d = 1;
        }
        if(n1>0) {
            Br = solutions[0];
            for(j = 1; j<n1; j++) {
                Br = cmpMin(Br, solutions[j]);
            }
        }else{
            Br.n = INT_MAX;
            Br.d = 1;

        }
        cont = true;
        return(makeSolution(Br, br, rows, solutions, n2));

    }
    printf("Rows are %d, Cols are %d", rows, cols);
    printf("\nThis is after divide \n");
    printMatrix(rows, cols, matrix, solutions);
    printf("\n");
    int sprime = rows - n2 + n1*(n2-n1);
    printf("%d \n", sprime);
    if(sprime == 0)
        return 1;

    fraction tempMatrix[sprime][cols-1];
	fraction tempSolution[sprime];

    int count  =0;
    for(l = 0; l< n1; l++) {
        for(k =n1; k<n2;k++) {
          for(i=0; i<cols-1; i++){
              subtract(&matrix[l][i],&matrix[k][i], &tempMatrix[count][i]);
          }
          subtract(&solutions[l], &solutions[k], &tempSolution[count]);
          count++;
      }
    }
    for(i = n2; i<rows; i++){
        for(j = 0; j<cols-1;j++) {
            tempMatrix[count][j] = matrix[i][j];
        }
        tempSolution[count] = solutions[i];
        count++;
    }

   // printf("test:  %d/%d \n",tempMatrix[0][0].n, tempMatrix[0][0].d);
   // fraction f = shorten(tempMatrix[0][0]);
   // printf("try:  %d/%d \n", f.n, f.d);


    printf("CHANGNING TO TEMP \n");


    for(i = 0; i< sprime; i++) {
        for(j = 0; j< cols-1; j++) {
            tempMatrix[i][j] = shorten(tempMatrix[i][j]);
            //printf("%d/%d equals %d/%d \n", matrix[i][j].n, matrix[i][j].d, tempMatrix[i][j].n, tempMatrix[i][j].d);
        }
        tempSolution[i] = shorten(tempSolution[i]);
    }

    printf("Sending this matrix to stage 2: \n");
    printMatrix(rows, cols, matrix, solutions);
    printf("\n");
	printMatrix(sprime, cols-1, tempMatrix, tempSolution);
    g = fm_stages(sprime, cols-1, tempMatrix, tempSolution);
    printf("\n");
//    free(tempMatrix);
//    free(tempSolution);
    return(g);

    cont = true;
    return 0;
}

bool fm(size_t rows, size_t cols, signed char a[rows][cols], signed char c[rows])
{

    long maxAllocRows = rows;
    int g;
    /*for(g = 0; g<cols; g++){
        printf("%ld \n" ,maxAllocRows);
        maxAllocRows += (ceil((double)maxAllocRows/2))*(ceil((double)maxAllocRows/2));
    }*/
    //fraction matrix[rows + (int)(ceil(rows/2)*ceil(rows/2))][cols];
    //fraction solutions[rows+ (int)(ceil(rows/2)*ceil(rows/2))];
    fraction matrix[rows][cols];
    fraction solutions[rows];

    //fraction matrix[maxAllocRows][cols];
    //fraction solutions[maxAllocRows];

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
	g = fm_stages(rows, cols, matrix, solutions);
//	free(matrix);
//	free(solutions);
	return(g);
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

    
   /* fraction f;
    f.d = 33;
    f.n = -54;

    f = shorten(f);
    printf("shorten is : %d/%d \n", f.n, f.d);*/


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

    /*fraction g;
    fraction h;
    fraction k;

    g.n = 1;
    g.d = 2;

    h.n = 3;
    h.d = -1;

    k.n = 0;
    k.d = 4;

    fraction lis[3];

    lis[0] = g;
    lis[1] = h;
    lis[2] = k;

    fraction result;
    result = getMin(3,lis);
*/
    //printf(" %d  \n \n",INT_MIN);

    int svar;
    svar = fm(rows,cols,a,c);
    if(svar ==1)
        printf("System solved \n");
    else
        printf("not solved \n");
    return(0);

}
