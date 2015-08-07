#include <stdio.h>

int main(){
    FILE *ifp = fopen("C:\\COP3502C\\Assignment 1\\bigIntTest.txt", "w");
    int i, j;
    for(i = 0; i < 2; i++){
        for(j = 0; j < 10000; j++){
            fprintf(ifp, "9");
        }
        fprintf(ifp, "\n\n");
    }
    fclose(ifp);
    return 0;
}
