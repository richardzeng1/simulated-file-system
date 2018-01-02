/* The functions operate on a linked list of free blocks.  Each node of the
 * list contains the starting location and the length of a free block.
 *
 */

#include <string.h>
#include <stdlib.h>
#include "file_ops.h"
#include "free_list.h"

/* Give free space back to the free list.  Since the list is ordered by
 * location, this function is the same for both algorithms.
 */
void add_free_block(FS *fs, int location, int size) {

	// create the free block
	Freeblock *new_block = malloc(sizeof(Freeblock));
	new_block->offset = location;
	new_block->length = size;
	new_block->next = NULL;
	Freeblock *curr = fs->freelist;
	// If there are no freeblocks, we initialize the freelist with a freeblock
	// at the location and specified size.
	if (curr ==NULL){
		fs->freelist = new_block;
	}else{
		Freeblock *previous_curr = NULL;
		// Iterate to find the correct position in the freelist
		if (fs->freelist->offset > location){
			new_block->next = fs->freelist;
			fs->freelist = new_block;
			if (fs->freelist->offset + fs->freelist->length == fs->freelist->next->offset){
				// Merge 2 first blocks
				Freeblock *merge_two = malloc(sizeof(Freeblock));
				merge_two->offset = fs->freelist->offset;
				merge_two->length = fs->freelist->length + fs->freelist->next->length;
				merge_two->next = fs->freelist->next->next;
				Freeblock *old = fs->freelist;
				Freeblock *old2 = fs->freelist->next;
				fs->freelist = merge_two;
				free(old);
				free(old2);
			}
		}else{
			
			while(curr->next->offset<location){
				previous_curr = curr;
				curr = curr->next;
				if (curr->next == NULL){
					break;
				}
			}

			new_block->next = curr->next;
			curr->next = new_block;

			// Handling cases to merge blocks

			// Checking if the previous block and the new block can be merged
			if (curr->offset + curr->length == new_block->offset){
				// Creating a new block with offset of curr and size of
				// the two blocks
				Freeblock *larger_block = malloc(sizeof(Freeblock));
				larger_block->offset = curr->offset;
				larger_block->length= new_block->length + curr->length;
				larger_block->next = new_block->next;
				if (previous_curr!=NULL){
					previous_curr->next = larger_block;
				}else{
					fs->freelist = larger_block;
				}
				free(curr);
				free(new_block);
				new_block = larger_block;
			}

			if (new_block->next !=NULL){
				// Checking if the new block and the next block can be merged
				if (new_block->offset + new_block->length == new_block->next->offset){
					// Creating a new block with offset of the new block and a length of
					// the new block and the next block
					Freeblock *merged_block = malloc(sizeof(Freeblock));
					merged_block->offset = new_block->offset;
					merged_block->length= new_block->length+ new_block->next->length;
					merged_block->next = new_block->next->next;
					free(new_block->next);
					free(new_block);
					if (previous_curr!=NULL){
						previous_curr->next = merged_block;
					}else{
						fs->freelist = merged_block;
					}
				}
			}
		}

	}
	return;
}

/* Print the contents of the free list */

void show_freelist(FS *fs) {
	Freeblock *curr = fs->freelist;
	printf("Free List\n");
	if (curr!=NULL){
		while (curr!=NULL){
			printf("(offset: %d, length:%d)\n", curr->offset, curr->length);
			curr = curr->next;
		}
	}
    return;
}


/* To be used after the metadata has been read from a file, to rebuild the
 * free list.
 */
void rebuild_freelist(FS *fs) {
	Freeblock *starter = malloc(sizeof(Freeblock));
	starter->offset = METADATA_ENDS;
	starter->length = MAX_FS_SIZE;
	starter->next =NULL;
	fs->freelist = starter;
	for (int index = 0; index<MAXFILES; index++){
		if (fs->metadata[index].length!=-1){
			Freeblock *curr = fs->freelist;
			Freeblock *previous_curr = NULL;
			while(fs->metadata[index].offset>curr->length+curr->offset){
				previous_curr = curr;
				curr = curr->next;
			}
			if (curr->length != fs->metadata[index].length){
				if (fs->metadata[index].offset==curr->offset){
					curr->offset += fs->metadata[index].length;
					curr->length -= fs->metadata[index].length;
				}else{
					Freeblock *first_block = malloc(sizeof(Freeblock));
					Freeblock *second_block = malloc(sizeof(Freeblock));

					first_block->offset = curr->offset;
					first_block->length = fs->metadata[index].offset - curr->offset;
					first_block->next = second_block;

					second_block->offset = fs->metadata[index].offset + fs->metadata[index].length;
					second_block->length = curr->offset + curr->length - (fs->metadata[index].offset + fs->metadata[index].length);
					second_block->next = curr->next;
					if (previous_curr!=NULL){
						previous_curr->next = first_block;
						free(curr);
					}else{
						fs->freelist = first_block;
						free(curr);
					}
				}
			}else{
				if (previous_curr!=NULL){
					previous_curr->next = curr->next;
					free(curr);
				}else{
					fs->freelist = curr->next;
					free(curr);
				}
			}
		}
	}
}
