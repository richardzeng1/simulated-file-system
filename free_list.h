#ifndef FREE_LIST_H_
#define FREE_LIST_H_

#include "file_ops.h"

/* You may implement these functions in any way you see fit.
 * Remember that your Makefile must build two targets:
 * - a file system with best-fit search
 * - and a file system with first-fit search
 */

/* Finds a free block of the given size, and returns its position.
 * Removes the block from the free list. */
int get_free_block(FS *fs, int size);

/* Adds to the free list a block at the given location, of the given size */
void add_free_block(FS *fs, int location, int size);

/* Build the freelist using the metadata read from a file.
 */
void rebuild_freelist(FS *fs);

/* Print the freelist to standard output.
 * Print one block per line usin ghte following format string:
 * "(offset: %d, length: %d)\n"
 */
void show_freelist(FS *fs); 


#endif /* FREE_LIST_H_ */
