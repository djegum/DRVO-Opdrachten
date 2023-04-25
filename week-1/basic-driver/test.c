#include <stdio.h>
// #include <stdlib.h>

int main(){
    printf("C tests running\n");
    char val[10];
    FILE *fptr;
    fptr = fopen("/dev/hello_dev", "w+");
    
    if(fptr == NULL){
        printf("Error!\n");
        return -1;
    }

    fscanf(fptr,"%s", &val);
    printf("Value of file: %s\n", val);

    fprintf(fptr,"Hello World");   

    fclose(fptr);
    return 0;
}