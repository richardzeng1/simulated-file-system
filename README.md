# simulated-file-system
A simulated file system that contains files


![University of Toronto](https://upload.wikimedia.org/wikipedia/en/thumb/9/9a/UofT_Logo.svg/1280px-UofT_Logo.svg.png)


#### **Table of Contents**
1. Description
2. Setup


### **Description**
This is an assignment from the University of Toronto's Computer Science program. It is the second assignment from the course
CSC209. This assignment was to create a simulated command line interface file system. The following operations were supported:
1. Initialize a file system
2. Create a file
3. Delete a file
4. Open a file system
5. Display the free list
6. Print the file system
7. Ensure all data has been written and close the file system

![](https://github.com/richardzeng1/simulated-file-system/blob/master/images/Screen%20Shot%202018-01-02%20at%202.13.56%20AM.png)

Image from the University of Toronto

We represent the size of the file system with a freelist which is a linked list. When we add a file, the size of the file
is removed from the freelist and when a file is deleted, the size of the deleted file is added to the freelist. We have two algorithms for determining a free slot to store the file.
1. The first is a first fit algorithm. When we encounter a block large enough for the file, the file is inserted at the
location.
2. The second is a best fit algorithm. We determine the smallest block large enough to store the file.

The algorithm is very extensive. Please see http://www.teach.cs.toronto.edu/~csc209h/fall/assignments/a2.html for more detail regarding the algorithm and project.
                 
Final Mark: 96.88%. <br />
Note: Some code was starter code written by the staff of the University of Toronto.

### **Setup**
1. Clone this repo
2. $make
