# Makefile for the simulated file system program

FLAGS= -Wall -g -std=gnu99

all : bfsim ffsim


bfsim : simfile.o file_ops.o transactions.o free_list_best_fit.o free_list_common.o
	gcc ${FLAGS} -o $@ $^
	
ffsim : simfile.o file_ops.o transactions.o free_list_first_fit.o free_list_common.o
	gcc ${FLAGS} -o $@ $^

# Separately compile each C file

# Ensure that the 
simfile.o : file_ops.h transactions.h
transactions.o : file_ops.h transactions.h free_list.h
file_ops.o : file_ops.h free_list.h
free_list_best_fit.o : free_list.h
free_list_first_fit.o : free_list.h
free_list_common.o : free_list.h


%.o : %.c 
	gcc ${FLAGS} -c $<


clean :
	-rm *.o bfsim ffsim

