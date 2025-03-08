#include <stdio.h>

//#define BUFFER_SIZE 10; // 11 for fgets?
//Macros don't seem to work

int main(){
    printf("Hello world!\n");

    FILE *fptr = fopen("labaddr.txt" , "r");
    char buff[10];

    while(fgets(buff, 10, fptr) != NULL){
        printf("Virtual address is %s :", buff);
    }

    fclose(fptr);
    return 0;
}