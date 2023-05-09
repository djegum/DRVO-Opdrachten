#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>      // printf()
#include <stdlib.h>     // exit()

int main(){
    printf("C tests running\n");
    char val[100];
    FILE *fptr1;
    FILE *fptr2;
    fptr1 = open("/dev/hello_dev1", O_RDONLY);

    if (fptr1 < 0) exit(1);

    char word[4];

    off_t new_pos = lseek(fptr1, 1, SEEK_SET);
    read(fptr1, word, 3);
    word[4] = 0;

    printf("Word @ pos %d: %s\n", new_pos, word);

    new_pos = lseek(fptr1, 1, SEEK_CUR); // Skip new line characters

    read(fptr1, word, 3);
    printf("Word @ pos %d: %s\n", new_pos, word);

    close(fptr1);

    return;
    // fptr2 = fopen("/dev/hello_dev1", "w+");
    
    if(fptr1 == NULL){
        printf("Error!\n");
        return -1;
    }


    // fprintf(fptr1,"Hello World");

    // fprintf(fptr2,"Bye World"); 

    fscanf(fptr1,"%s", &val);
    printf("Value of file1: %s\n", val);

    // fscanf(fptr2,"%s", &val);
    // printf("Value of file2: %s\n", val);

    fclose(fptr1);
    // fclose(fptr2);

    return 0;
}