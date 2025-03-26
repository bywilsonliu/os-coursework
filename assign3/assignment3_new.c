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


// frames brought in order???

struct PhysMem {
    signed char arr[128*256]; //128 frames, each holding 256 bytes (signed char)
    int front; //initialize to -1
    int back; //initialize to -1
};

u_int16_t pageFault(u_int16_t pageNum, struct PhysMem *phys, signed char* mmapfptr);

int main(){
    printf("Hello world!\n");

    // Address requests setup
    FILE *fptr = fopen("addresses.txt" , "r");
    char buff[BUFFER_SIZE];

    // Backing store setup
    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY); 
    signed char* mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0); 

    // Physical memory setup
    struct PhysMem phys = {{0}, -1, -1};


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
        if (page_table[page_number] == 999){ // Page fault occurs
            printf("Page fault occurs\n");
            int frame = pageFault(page_number, &phys, mmapfptr);
            printf("Frame %d", frame);
            u_int16_t physical_address = ((frame << OFFSET_BITS) | offset);
            printf("Physical address is %u\n", physical_address);        
            //printf("Value=%d", *(phys.arr + ))
            
            //signed char* test = phys->arr + 20;
            //printf("%d, %d, %d", *(phys.arr + 20), *(phys.arr + 758), *(phys.arr + 947));
        }
        else printf("No page fault\n");

        








        
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

u_int16_t pageFault(u_int16_t pageNum, struct PhysMem *phys, signed char* mmapfptr) {
    if (phys->back == -1) { // Counter logic
        phys->front = 0;
        phys->back = 0;
    } else if ((phys->front == phys->back + 1) || (phys->front == 0 && phys->back == 127)) { // FIFO replacement policy?
        phys->back = phys->front;
        if(++phys->front == 128) phys->front = 0;
    } else {
        ++phys->back;
    }
    memcpy(phys->arr + PAGE_SIZE * phys->back, mmapfptr + PAGE_SIZE * pageNum, PAGE_SIZE);
    
    return (u_int16_t)phys->back;
}


// There should be (a maximum of?) 2^8 entries in the (single-level) page table.
// This program has (in its logical address space) 256 = 2^8 pages.
// Now given a logical address, you're TRYING TO LOOK UP the value in that physical address (requires address translation from you).

// This requires pure demand paging, so if the page in which that logical address resides is not in memory,
// you must bring it in memory through a page fault. 
// (Unlike normally, where you just look up the frame in memory the page is (already) stored in, using the page table.)

// mmapfptr to mmapfptr + 65536 -> this space represents BACKING STORE, which is organized by PAGES!!