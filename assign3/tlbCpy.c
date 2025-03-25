//#include <pthread.h>
#include <stdlib.h>
#include <stdio.h> //testing


#define TLB_SIZE 10

struct TLBentry {
    char pageNum;
    unsigned short frameNum;
};

struct Queue {
    struct TLBentry arr[TLB_SIZE];
    int front; //initialize to -1
    int back; //initialize to -1
};

/*
struct arg_struct {
    const char *arg1;
    struct TLBentry arg2;
};

void *checkEntry(const char *pageReq, struct TLBentry entry) {
    if (*pageReq == entry.pageNum) return (void *) (char*) (entry.frameNum + 1); //hit
    return (void *) (unsigned short *) 0; //miss
}
*/

short search_TLB(const char *pageReq, struct Queue *tlb) {
    int k;
    if ((tlb->front == tlb->back + 1) || (tlb->front == 0 && tlb->back == TLB_SIZE - 1)) k = TLB_SIZE;
    else k = tlb->back - tlb->front + 1;

    //testing
    short frame = -1;
    for (int i = 0; i < k; i++) {
        if (*pageReq == tlb->arr[i].pageNum) frame += tlb->arr[i].frameNum + 1;
    }

    /*
    pthread_t threads[TLB_SIZE];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    struct arg_struct args;
    args.arg1 = pageReq;
    for (int i = 0; i < k; i++) {
        args.arg2 = tlb->arr[i];
        pthread_create(&threads[i], &attr, checkEntry, (void *)&args); //parallized to emulate checking all entries simultaniously
    }

    void *result;
    unsigned short frame = -1;
    for (int i = 0; i < k; i++) {
        pthread_join(threads[i], &result);
        frame += (unsigned short) (unsigned short*) result;
    }
    */
    return frame;
}

void TLB_Add(struct TLBentry entry, struct Queue *tlb) {
    if (tlb->back == -1) {
        tlb->front = 0;
        tlb->back = 0;
    } else if ((tlb->front == tlb->back + 1) || (tlb->front == 0 && tlb->back == TLB_SIZE - 1)) {
        tlb->back = tlb->front;
        if(++tlb->front == TLB_SIZE) tlb->front = 0;
    } else {
        ++tlb->back;
    }
    tlb->arr[tlb->back] = entry;
}

void TLB_Update(struct TLBentry entry, struct Queue *tlb) { //assuming known existence
    int i;
    for (i = tlb->front; (i % TLB_SIZE) < tlb->back; i++) {
        if (tlb->arr[i % TLB_SIZE].pageNum == entry.pageNum) break;
    }
    for (i; (i % TLB_SIZE) < tlb->back; i++) {
        tlb->arr[i % TLB_SIZE] = tlb->arr[(i + 1) % TLB_SIZE];
    }
    tlb->arr[tlb->back] = entry;
}

//testing
int main() {
    struct Queue tlb;
    struct Queue *tlbPtr = &tlb;
    tlbPtr->back = -1;
    tlbPtr->front = -1;
    struct TLBentry entry;
    entry.pageNum = 1;
    entry.frameNum = 3;
    TLB_Add(entry, tlbPtr);
    entry.pageNum = 4;
    entry.frameNum = 7;
    TLB_Add(entry, tlbPtr);
    entry.pageNum = 2;
    entry.frameNum = 5;
    TLB_Add(entry, tlbPtr);

    char req = 1;
    printf("1: %d\n",search_TLB(&req, tlbPtr));
    req = 4;
    printf("4: %d\n",search_TLB(&req, tlbPtr));
    req = 2;
    printf("2: %d\n",search_TLB(&req, tlbPtr));

    entry.pageNum = 2;
    entry.frameNum = 9;
    TLB_Update(entry, tlbPtr);

    printf("2 (updated): %d\n",search_TLB(&req, tlbPtr));
}