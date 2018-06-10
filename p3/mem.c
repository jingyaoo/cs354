#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"

/*
 * This structure serves as the header for each allocated and free block
 * It also serves as the footer for each free block
 * The blocks are ordered in the increasing order of addresses 
 */
typedef struct block_tag{

	int size_status;

	/*
	 * Size of the block is always a multiple of 4
	 * => last two bits are always zero - can be used to store other information
	 *
	 * LSB -> Least Significant Bit (Last Bit)
	 * SLB -> Second Last Bit 
	 * LSB = 0 => free block
	 * LSB = 1 => allocated/busy block
	 * SLB = 0 => previous block is free
	 * SLB = 1 => previous block is allocated/busy
	 * 
	 * When used as the footer the last two bits should be zero
	 */

	/*
	 * Examples:
	 * 
	 * For a busy block with a payload of 24 bytes (i.e. 24 bytes data + an additional 4 bytes for header)
	 * Header:
	 * If the previous block is allocated, size_status should be set to 31
	 * If the previous block is free, size_status should be set to 29
	 * 
	 * For a free block of size 28 bytes (including 4 bytes for header + 4 bytes for footer)
	 * Header:
	 * If the previous block is allocated, size_status should be set to 30
	 * If the previous block is free, size_status should be set to 28
	 * Footer:
	 * size_status should be 28
	 * 
	 */

} block_tag;

/* Global variable - This will always point to the first block
 * i.e. the block with the lowest address */
block_tag *first_block = NULL;

/* Global variable - Total available memory */
int total_mem_size = 0;

/* 
 * Function for allocating 'size' bytes
 * Returns address of the payload in the allocated block on success 
 * Returns NULL on failure 
 * Here is what this function should accomplish 
 * - If size is less than equal to 0 - Return NULL
 * - Round up size to a multiple of 4 
 * - Traverse the list of blocks and allocate the best free block which can accommodate the requested size 
 * - Also, when allocating a block - split it into two blocks when possible 
 * Tips: Be careful with pointer arithmetic 
 */
void* Mem_Alloc(int size){
        //If size is less then equal to 0, return NULL
	if (size <= 0) {
                //Returns NULL on failure
		return NULL;
	}
        //get totalSize that include header, payload and pad
        //and round up the size to a multiple of 4
	int const header = 4;
	int const pad = (4*(size/4) + 4)- size;
	int payload = size;
	int totalSize = header + pad + payload;
        
        //Traverse the list of blocks and allocate and best free block which can accommodate the requested size
	block_tag* curr = first_block;
	block_tag* best_block = NULL;
	while (1) {
		// if curr block is free
		if ((curr->size_status & 1) == 0) {
			int gap = (curr->size_status & ~3) - totalSize;
                        //if totalSize equal to curr block size, it is the bestfit one          
			if (gap == 0){
				curr->size_status =curr->size_status |1;
				block_tag* next_header = (block_tag*)((char*)curr + (curr->size_status & (~3)));
				if (next_header < (block_tag*)((char*)first_block + total_mem_size)) {
					next_header->size_status = next_header->size_status | 2;
				}
                                //Returns address of the payload in the allocated block on success
				return (block_tag*)((char*)curr + 4);
                        //if the size of curr block is greater than totalSize, we can firstly to store it as the bestfit one
			} else if (gap > 0){  
				if (best_block == NULL)
					best_block = curr;
				else if (best_block->size_status > curr->size_status) {
					best_block = curr;
				}
			}
		}
                //to make the curr pointer to the next pointer
		curr = (block_tag*)((char*)curr + (curr->size_status & (~3)));
                //if curr exceed the max range, then break it
		if (curr >= (block_tag*)((char*)first_block + total_mem_size)) {
			if (best_block == NULL) {
                                //Returns NULL on failure
				return NULL;
			}  else {
				int gap2 = (best_block->size_status & ~3) - totalSize;
                                // if the size of best block is enough larger, then split it
                                //if the size of best block is not enought larger, do not split it
				if (gap2 < 8) {
					best_block->size_status = best_block->size_status |1;
					block_tag*next_header = (block_tag*)((char*)best_block + (best_block->size_status &~3));
					if (next_header < (block_tag*)((char*)first_block + total_mem_size)) {
						next_header->size_status = next_header->size_status |2;
					}
				} else {
					block_tag* header =(block_tag*)((char*)best_block+ totalSize);
					header->size_status = gap2 |2;
					block_tag *footer = (block_tag*)((char*)best_block + totalSize + gap2 -4);
					footer->size_status = gap2;
					best_block->size_status = totalSize |3;
				}
                                //Returns address of the payload in the allocated block on success
				return (block_tag*)((char*)best_block + 4);
			}
		}
	}
}

/* 
 * Function for freeing up a previously allocated block 
 * Argument - ptr: Address of the payload of the allocated block to be freed up 
 * Returns 0 on success 
 * Returns -1 on failure 
 * Here is what this function should accomplish 
 * - Return -1 if ptr is NULL
 * - Return -1 if ptr is not within the range of memory allocated by Mem_Init()
 * - Return -1 if ptr is not 4 byte aligned
 * - Mark the block as free 
 * - Coalesce if one or both of the immediate neighbours are free 
 */
int Mem_Free(void *ptr){
        //Return -1 if ptr is NULL
	if (ptr == NULL)
		return -1;
        //Return -1 if ptr is not within the range of memory allocated by Mem_Init()
	if ((block_tag*)ptr > (block_tag*)((char*)first_block + total_mem_size)|| (block_tag*)ptr < first_block)
		return  -1;
        //Return -1 if ptr is not 4 byte aligned
	if ((int)ptr % 4 != 0)
		return -1;
        //make the curr pointer to the ptr pointer and creat header and footer to meet the requirement
        //mark this block is free
	block_tag* curr_header = (block_tag*)((char*)ptr - 4);
	block_tag* curr_footer = (block_tag*)((char*)curr_header + (curr_header->size_status & ~3) - 4);
	curr_header ->size_status = curr_header->size_status & ~1;
	curr_footer->size_status = curr_header->size_status & ~3;
        // use the next header to identify the curr pointer is free
	block_tag*next_header =(block_tag*)((char*)curr_footer + 4);
	if (next_header < (block_tag*)((char*)first_block + total_mem_size)) {
		next_header->size_status = next_header->size_status & ~2;
	}
        //to coalesce the previous block if it is free
        //and update previous block situation and curr block situation
	if ((curr_header->size_status & 2) == 0) {
		block_tag*previous_footer = (block_tag*)((char*) curr_header - 4);
		block_tag*previous_header = (block_tag*)((char*)previous_footer - previous_footer->size_status + 4);
		previous_header->size_status = ((previous_header->size_status & ~3) + (curr_header->size_status & ~3)) | 2;
		curr_header = previous_header;
		curr_footer->size_status = curr_header->size_status & ~3;
	}
        //to coalesce the back block if it is free
       // and update later block situation and curr block situation
	block_tag*later_header = (block_tag*)((char*)curr_header + (curr_header->size_status &~3));
	if ((later_header < (block_tag*)((char*)first_block + total_mem_size)) && (later_header->size_status & 1) == 0) {
		block_tag*later_footer = (block_tag*)((char*)later_header + (later_header->size_status &~3) -4);
		curr_footer = later_footer;
		curr_header->size_status =((later_header->size_status &~3) + (curr_header->size_status &~3)) | 2;
		curr_footer->size_status = curr_header->size_status & ~3;
	}
	return 0;
}

/*
 * Function used to initialize the memory allocator
 * Not intended to be called more than once by a program
 * Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated
 * Returns 0 on success and -1 on failure 
 */
int Mem_Init(int sizeOfRegion){
	int pagesize;
	int padsize;
	int fd;
	int alloc_size;
	void* space_ptr;
	static int allocated_once = 0;

	if(0 != allocated_once){
		fprintf(stderr,"Error:mem.c: Mem_Init has allocated space during a previous call\n");
		return -1;
	}
	if(sizeOfRegion <= 0){
		fprintf(stderr,"Error:mem.c: Requested block size is not positive\n");
		return -1;
	}

	// Get the pagesize
	pagesize = getpagesize();

	// Calculate padsize as the padding required to round up sizeOfRegion to a multiple of pagesize
	padsize = sizeOfRegion % pagesize;
	padsize = (pagesize - padsize) % pagesize;

	alloc_size = sizeOfRegion + padsize;

	// Using mmap to allocate memory
	fd = open("/dev/zero", O_RDWR);
	if(-1 == fd){
		fprintf(stderr,"Error:mem.c: Cannot open /dev/zero\n");
		return -1;
	}
	space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (MAP_FAILED == space_ptr){
		fprintf(stderr,"Error:mem.c: mmap cannot allocate space\n");
		allocated_once = 0;
		return -1;
	}

	allocated_once = 1;

	// Intialising total available memory size
	total_mem_size = alloc_size;

	// To begin with there is only one big free block
	first_block = (block_tag*) space_ptr;

	// Setting up the header
	first_block->size_status = alloc_size;
	// Marking the previous block as busy
	first_block->size_status += 2;

	// Setting up the footer
	block_tag *footer = (block_tag*)((char*)first_block + alloc_size - 4);
	footer->size_status = alloc_size;

	return 0;
}

/* 
 * Function to be used for debugging 
 * Prints out a list of all the blocks along with the following information for each block 
 * No.      : serial number of the block 
 * Status   : free/busy 
 * Prev     : status of previous block free/busy
 * t_Begin  : address of the first byte in the block (this is where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block (as stored in the block header)(including the header/footer)
 */ 
void Mem_Dump() {
	int counter;
	char status[5];
	char p_status[5];
	char *t_begin = NULL;
	char *t_end = NULL;
	int t_size;

	block_tag *current = first_block;
	counter = 1;

	int busy_size = 0;
	int free_size = 0;
	int is_busy = -1;

	fprintf(stdout,"************************************Block list***********************************\n");
	fprintf(stdout,"No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
	fprintf(stdout,"---------------------------------------------------------------------------------\n");

	while(current < (block_tag*)((char*)first_block + total_mem_size)){

		t_begin = (char*)current;

		t_size = current->size_status;

		if(t_size & 1){
			// LSB = 1 => busy block
			strcpy(status,"Busy");
			is_busy = 1;
			t_size = t_size - 1;
		}
		else{
			strcpy(status,"Free");
			is_busy = 0;
		}

		if(t_size & 2){
			strcpy(p_status,"Busy");
			t_size = t_size - 2;
		}
		else strcpy(p_status,"Free");

		if (is_busy) busy_size += t_size;
		else free_size += t_size;

		t_end = t_begin + t_size - 1;

		fprintf(stdout,"%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n",counter,status,p_status,
				(unsigned long int)t_begin,(unsigned long int)t_end,t_size);

		current = (block_tag*)((char*)current + t_size);
		counter = counter + 1;
	}
	fprintf(stdout,"---------------------------------------------------------------------------------\n");
	fprintf(stdout,"*********************************************************************************\n");

	fprintf(stdout,"Total busy size = %d\n",busy_size);
	fprintf(stdout,"Total free size = %d\n",free_size);
	fprintf(stdout,"Total size = %d\n",busy_size+free_size);
	fprintf(stdout,"*********************************************************************************\n");
	fflush(stdout);
	return;
}
