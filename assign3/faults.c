#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define MEMORY_SIZE 65536
#define PAGE_SIZE 256

signed char *mmapfptr;

struct PhysMem {
    signed char arr[128]; //128 pages in physical space  //Only 1D array?
    int front; //initialize to -1
    int back; //initialize to -1
};

void pageFault(const char* pageNum, struct PhysMem *phys) {
    if (phys->back == -1) { // Counter logic
        phys->front = 0;
        phys->back = 0;
    } else if ((phys->front == phys->back + 1) || (phys->front == 0 && phys->back == 127)) { // FIFO replacement policy?
        phys->back = phys->front;
        if(++phys->front == 128) phys->front = 0;
    } else {
        ++phys->back;
    }
    memcpy(phys->arr + phys->back, mmapfptr + PAGE_SIZE * *pageNum, PAGE_SIZE);
}

int main() {
    int mmapfile_fd = open("BACKING_STORE.bin", O_RDONLY);
    mmapfptr = mmap(0, MEMORY_SIZE, PROT_READ, MAP_PRIVATE, mmapfile_fd, 0);
    
    //do at end of program:
    munmap(mmapfptr, MEMORY_SIZE);
}