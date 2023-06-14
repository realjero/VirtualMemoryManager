#include "vmm.h"
#include "output_utility.h"
#include "fifo.h"
#include "tlb.h"

#include <stdio.h>

struct {
    unsigned char memory[PHYSICAL_MEMORY_SIZE];
} PhysicalMemory;

struct {
    int frame_number[PAGE_TABLE_SIZE];
    int frame_count;
    Queue frame_queue;
} PageTable;

/**
 * Initialized a statistics object to zero.
 * @param stats A pointer to an uninitialized statistics object.
 */
static void statistics_initialize(Statistics *stats) {
    stats->tlb_hits = 0;
    stats->pagetable_hits = 0;
    stats->total_memory_accesses = 0;
}

Statistics simulate_virtual_memory_accesses(FILE *fd_addresses, FILE *fd_backing) {
    int page_number, offset, address;
    // Initialize statistics
    Statistics stats;
    statistics_initialize(&stats);
    initialize(&PageTable.frame_queue);
    initializeTLB();

    // Initialize physical memory
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        PhysicalMemory.memory[i] = 0;
    }

    // Initialize page table
    PageTable.frame_count = 0;
    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
        PageTable.frame_number[i] = -1;
    }

    for (;;) {
        if (fscanf(fd_addresses, "%d", &address) == EOF) break;

        // Get page number and offset
        page_number = GET_PAGE_NUMBER(address);
        offset = GET_OFFSET(address);

        if (32956 == address) {
            printf("address: %d\n", address);
        }

        stats.total_memory_accesses++;

        // TLB HIT
        int tlbFrameNumber = getFrameNumber(page_number);
        if (tlbFrameNumber != -1) {
            print_access_results(address, tlbFrameNumber * FRAME_SIZE + offset,
                                 PhysicalMemory.memory[tlbFrameNumber * FRAME_SIZE + offset],
                                 true, false);
            stats.tlb_hits++;
            stats.pagetable_hits++; // Why tho?!?!?
        } else {

            // PAGE HIT
            if (PageTable.frame_number[page_number] != -1) {
                insert(page_number, PageTable.frame_number[page_number]);
                print_access_results(address, PageTable.frame_number[page_number] * FRAME_SIZE + offset,
                                     PhysicalMemory.memory[PageTable.frame_number[page_number] * FRAME_SIZE + offset],
                                     false, true);
                stats.pagetable_hits++;
            } else {
                // PAGE FAULT
                int frame_number;

                if (PageTable.frame_count < FRAME_COUNT) {
                    frame_number = PageTable.frame_count;
                    PageTable.frame_count++;
                } else {
                    frame_number = dequeue(&PageTable.frame_queue);
                    for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
                        if (PageTable.frame_number[i] == frame_number) {
                            PageTable.frame_number[i] = -1;
                            break;
                        }
                    }
                }

                fseek(fd_backing, page_number * PAGE_SIZE, SEEK_SET);
                fread(&PhysicalMemory.memory[frame_number * PAGE_SIZE], sizeof(char), PAGE_SIZE, fd_backing);

                PageTable.frame_number[page_number] = frame_number;
                enqueue(&PageTable.frame_queue, frame_number);

                insert(page_number, frame_number);

                print_access_results(address,
                                     frame_number * PAGE_SIZE + offset,
                                     PhysicalMemory.memory[frame_number * PAGE_SIZE + offset],
                                     false,
                                     false);
            }
        }
    }

    return stats;
}
