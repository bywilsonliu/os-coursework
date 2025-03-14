#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_SIZE 10 // 11 for fgets?
#define OFFSET_BITS 8
#define OFFSET_MASK 0x00FF
#define PAGES 8

#define MEMORY_SIZE 65536

// TEST THIS ALL INCREMENTALLY, ONE LINE AT A TIME - ONLY DOING THIS CUZ YOU HAVE NO WIFI ON PLANE


// frames brought in order???

int main(){
    printf("Hello world!\n");

    FILE *fptr = fopen("addresses.txt" , "r");
    char buff[BUFFER_SIZE];

    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY); 
    signed char* mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0); 



    //CHECK  u_int16_t page_table[256] = {-1};

    while(fgets(buff, BUFFER_SIZE, fptr) != NULL){
        // printf("Logical address is %s :", buff);
        int buff_1 = atoi(buff);
        u_int16_t logical_address = (u_int16_t)buff_1;
        printf("Logical address is %u: ", logical_address);
        u_int16_t page_number = logical_address >> OFFSET_BITS;
        printf("Page# = %u & ", page_number);
        u_int16_t offset = logical_address & OFFSET_MASK;
        printf("Offset = %u. \n", offset);
        
        // printf("Physical address is %u\n", (page_table[page_number] << OFFSET_BITS) | (logical_address & OFFSET_MASK));        
    }

    munmap(mmapfptr, MEMORY_SIZE);
    fclose(fptr);
    return 0;
}

// There should be (a maximum of?) 2^8 entries in the (single-level) page table.
// This program has (in its logical address space) 256 = 2^8 pages.
// Now given a logical address, you're TRYING TO LOOK UP the value in that physical address (requires address translation from you).

// This requires pure demand paging, so if the page in which that logical address resides is not in memory,
// you must bring it in memory through a page fault. 
// (Unlike normally, where you just look up the frame in memory the page is (already) stored in, using the page table.)

