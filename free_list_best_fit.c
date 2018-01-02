#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Implement the best fit algorithm to find free space for the
   simulated file data.

   This function finds the smallest block that is big enough to store the data
   and removes it to be used.
 */

int get_free_block(FS *fs, int size) {
        Freeblock *smallest = fs->freelist;
        Freeblock *before_smallest = NULL;
        Freeblock *curr = fs->freelist;
        Freeblock *previous_curr = NULL;
        int found = 0;
        if (fs->freelist == NULL){
            fprintf(stderr, "No space.\n");
            exit(1);
        }
        while(curr!=NULL){
            if (curr->length>=size){
                found =1;
                if (smallest->length < size){
                    smallest = curr;
                    before_smallest = previous_curr;
                }else{
                    if(smallest->length > curr->length){
                        smallest=curr;
                        before_smallest = previous_curr;
                    }
                }
            }
            previous_curr = curr;
            curr = curr->next;
        }
        if (found ==0){
            fprintf(stderr, "Cannot find empty block\n");
            exit(1);
        }
        int location = smallest->offset;
        if (smallest->length == size){
            if (before_smallest!=NULL){
                before_smallest->next = smallest->next;
            }else{
                fs->freelist = smallest->next;
            }
            free(smallest);
        }else{
            smallest->offset += size;
            smallest->length -= size;
        }
        return location;
}
