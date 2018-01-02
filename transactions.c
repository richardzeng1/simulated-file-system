#include <stdlib.h>
#include <string.h>
#include "transactions.h"
#include "file_ops.h"
#include "free_list.h"

/* Each line of a transaction file represents a single operation on the
 * simulated file system.  A line is divided into several fields and the fields
 * are separated by a single space.
 * 
 * The first field of a transaction is a single character.
 * c = create_file, d = delete_file, 
 * s = show_freelist, p = print_fs
 * The remaining fields (if any) are the arguments of the operation in order
 */

static void split(char **args, char *line) {
    char *ptr = line;
    args[0] = line;
    int i = 1;
    while((ptr = strchr(ptr, ' ')) != NULL) {
        *ptr = '\0';
        ptr++;
        args[i] = ptr;
        i++;
    }
}

void process_transactions(char *transfile) {
    char line[MAXLINE];
    FS *fs = NULL;

    FILE *tf = fopen(transfile, "r");
    if (tf == NULL ) {
        perror("fopen");
        exit(1);
    }

    // first transaction must be init or open

    if((fgets(line, MAXLINE, tf)) != NULL) {
        char *args[5];
        // First remove newline character
        line[strlen(line) - 1] = '\0';
        split(args, line);

        if(line[0] == 'i') {
            fs = init_fs(args[1]);
        } else if(line[0] == 'o') {
            fs = open_fs(args[1]);
        } else {
            fprintf(stderr, "First transaction must be init_fs or open_fs\n");
            exit(1);
        }
    }

    while((fgets(line, MAXLINE, tf)) != NULL) {
        char *args[5];
        // First remove newline character
        line[strlen(line) - 1] = '\0';

        if(line[0] == '#') {
            continue;
        }

        split(args, line);

        switch(line[0]) {
        case 'd':
            delete_file(fs, args[1]);
            break;
        case 'c':
            create_file(fs, args[1], atoi(args[2]), args[3]);
            break;
        case 's':
            show_freelist(fs);
            break;
        case 'p':
            print_fs(fs);
            break;
        case 'x':  // close
            close_fs(fs);
            break;
        case '#':  // just do nothing on comment line
            break;
        default:
            fprintf(stderr, "Error: bad operation, %c\n", line[0]);
            exit(1);
        }
    }
    fclose(tf);
}
