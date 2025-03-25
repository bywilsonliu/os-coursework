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
#define PAGE_SIZE 256
#define FRAME_SIZE 128
#define NUM_PAGES 256
#define NUM_FRAMES 128

// TEST THIS ALL INCREMENTALLY, ONE LINE AT A TIME - ONLY DOING THIS CUZ YOU HAVE NO WIFI ON PLANE


// frames brought in order???

int main(){
    printf("Hello world!\n");

    // Address requests setup
    FILE *fptr = fopen("addresses.txt" , "r");
    char buff[BUFFER_SIZE];

    // Backing store setup
    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY); 
    signed char* mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0); 

    /*
    // Physical memory setup
    signed char phys_mem[NUM_FRAMES][FRAME_SIZE] = {69};
    // Question: How to implement physical memory (32768 bits) - double array?
    */

    u_int16_t page_table[256] = { [0 ... 255] = 999}; // Frame numbers can be between 0 and 127, or -1 -> therefore (signed) int_8_t
    // But for integration with below loop, make it u_int16_t and "not in memory number" 999 instead of -1

    while(fgets(buff, BUFFER_SIZE, fptr) != NULL){
        int buff_1 = atoi(buff); // Just buffer (for type conversion)

        u_int16_t logical_address = (u_int16_t)buff_1;
        printf("Logical address is %u: ", logical_address);
        u_int16_t page_number = logical_address >> OFFSET_BITS;
        printf("Page# = %u & ", page_number);
        u_int16_t offset = logical_address & OFFSET_MASK;
        printf("Offset = %u. \n", offset);

        // CHECK TLB
        // CHECK PAGE TABLE // valid/invalid bit?
        if (page_table[page_number] == 999){ // page fault occurs
            printf("Page fault occurs\n");
        }






        
        // IF PAGE FAULT OCCURS (TLB miss and not in page table)

        // memcpy(&phys_mem + , mmapfptr + page_number*256, 256); //At least starting with frame 0
        // printf("Physical address is %u\n", (page_table[page_number] << OFFSET_BITS) | (logical_address & OFFSET_MASK));        
    }

    /* memcpy(&phys_mem, mmapfptr + 66*256, 256); //At least starting with frame 0
    printf("------------\n");
    printf("Test value is: %hhn\n", *(phys_mem + 20));
    // ^Worked before this line? Not printing properly (Or at least before this commit?)
    */

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

// mmapfptr to mmapfptr + 65536 (32768?) represents physical address space?
// ^Not sure?

// mmapfptr to mmapfptr + 65536 -> this space represents BACKING STORE, which is organized by PAGES!!