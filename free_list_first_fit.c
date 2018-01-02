#include <string.h>
#include <stdlib.h>

#include "free_list.h"

/* Implement the first fit algorithm to find free space for the
   simulated file data.

   This function finds the first freeblock with enough space and removes it
   to be used.
 */

int get_free_block(FS *fs, int size) {
    Freeblock *curr = fs->freelist;
    Freeblock *previous_curr=NULL;
    int found =0;
    while(curr!=NULL){
        if (curr->length >=size){
            found =1;
            break;
        }
        previous_curr = curr;
        curr = curr->next;
    }
    if (found==0){
        fprintf(stderr, "Cannot find free block\n");
        exit(1);
    }
    if (curr==NULL){
        fprintf(stderr, "Out of space. Cannot create file.\n");
        exit(1);
    }
    int location = curr->offset;
    if (curr->length==size){
        if (previous_curr!=NULL){
            previous_curr->next = curr->next;
            free(curr);
            return location;
        }else{
            free(curr);
            fs->freelist = curr->next;
            return location;
        }
    }else{
        curr->offset += size;
        curr->length -= size;
        return location;
    }
    return -1;
}
