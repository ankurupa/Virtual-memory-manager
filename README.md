Virtual-memory-manager
======================

This project implements the virtual memory manager which is responsible for allocating pages to the memory frames using page 
replacement algorithms like FIFO, LFU, LRU-STACK, LRU-CLOCK and LRU-REF8. The virtual memory server also compares the number 
of page faults and performance of the algorithm (performance is measured in terms of time take by the algorithm to execute) 
to that of OPTIMAL page replacement algorithm. 

Data Structure used for Implementation:
--------------------------------------
The circular doubly linked list has been used for frames structure in physical memory. The virtual memory manager will allocate pages in user input sequence to these frames. 
The FIFO and LFU page replacement algorithms can easily be implemented by using the singly linked list. 
For LRU-STACK algorithm, we need to keep track of the next and previous pages in the list since we need to move the page at the TOP of the STACK (in this case, LAST of the linked list) whenever referenced. Thus, in order to implement LRU-STACK page replacement algorithm, we need doubly linked list.
For LRU-CLOCK algorithm, we need to move the hand of the clock in a circular manner. Thus the last page of the list should point to the head page so that the hand of the clock can start all over again from the head page. Therefore we need circular linked list.
Thus the choice of circular doubly linked list ensures the implementation of all algorithm.

The Source Code consists of following files:
1.	virtualmem.cpp
2.	Server.cpp
3.	PageList.cpp
