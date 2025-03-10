#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10 // 11 for fgets?

#define OFFSET_BITS 12 // 32-20 (counter-intuitive)
#define OFFSET_MASK 0x00000FFF
// (0b?)000000000000111111111111111111111111 or 0x000FFF
#define PAGES 8

// TEST THIS ALL INCREMENTALLY, ONE LINE AT A TIME - ONLY DOING THIS CUZ YOU HAVE NO WIFI ON PLANE


int main(){
    printf("Hello world!\n");

    FILE *fptr = fopen("labaddr.txt" , "r");
    char buff[BUFFER_SIZE];
    u_int32_t page_table[PAGES] = {6,4,3,7,0,1,2,5};

    while(fgets(buff, BUFFER_SIZE, fptr) != NULL){
        // printf("Virtual address is %s :", buff);
        int buff_1 = atoi(buff);
        u_int32_t logical_address = (u_int32_t)buff_1;
        printf("Logical address is %u: ", logical_address);
        u_int32_t page_number = logical_address >> OFFSET_BITS;
        printf("Page# = %u & ", page_number);
        u_int32_t offset = logical_address & OFFSET_MASK;
        printf("Offset = %u. ", offset);
        printf("Physical address is %u\n", (page_table[page_number] << OFFSET_BITS) | (logical_address & OFFSET_MASK));        
    }

    fclose(fptr);
    return 0;
}