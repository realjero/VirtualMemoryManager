//
// Created by student on 14.06.23.
//

#ifndef VMM_TLB_H
#define VMM_TLB_H

#include "vmm.h"

void initializeTLB();

int getFrameNumber(int page_number);

void insert(int page_number, int frame_number);

#endif //VMM_TLB_H
