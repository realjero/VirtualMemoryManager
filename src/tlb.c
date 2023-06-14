//
// Created by student on 14.06.23.
//

#include "tlb.h"

struct {
    int page_number[TLB_SIZE];
    int frame_number[TLB_SIZE];
    int count;
} TLB;

void initializeTLB() {
    TLB.count = 0;
    for (int i = 0; i < TLB_SIZE; i++) {
        TLB.page_number[i] = -1;
        TLB.frame_number[i] = -1;
    }
}

int getFrameNumber(int page_number) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (TLB.page_number[i] == page_number) {
            return TLB.frame_number[i];
        }
    }
    return -1;
}

void insert(int page_number, int frame_number) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (TLB.frame_number[i] == frame_number) {
            TLB.page_number[i] = -1;
            TLB.frame_number[i] = -1;
        }
    }
    TLB.page_number[TLB.count % TLB_SIZE] = page_number;
    TLB.frame_number[TLB.count % TLB_SIZE] = frame_number;
    TLB.count = (TLB.count + 1) % TLB_SIZE;
}