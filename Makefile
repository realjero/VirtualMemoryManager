CC = gcc
CFLAGS = -Wall -Wextra

# Source files
SRC = src/vmm.c src/main.c src/output_utility.c src/fifo.c src/tlb.c
HEADERS = src/vmm.h src/output_utility.h src/fifo.h src/tlb.h
OBJ = $(SRC:.c=.o)

# Test command
TEST_CMD = ./vmm --backing BACKING_STORE.bin addresses.txt > output1.txt && diff output1.txt correct-64.txt && rm output1.txt

# Default target
all: vmm

# Build the vmm executable
vmm: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile the source files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up generated files
clean:
	rm -f vmm $(OBJ) output1.txt

# Test the vmm program
test: vmm
	$(TEST_CMD)