This program is a modification of the original `lab3-origin.cpp` program.

To overcome the slowness, I split the data between the two threads and have each 
thread work on its own set of data. 

The new version of the program is using the code sharing technique for the threads, 
by dividing the data between the two threads and having each thread work on its own 
set of data. As a result, the two threads are not competing for the same resources 
and the performance is improved. Because the two threads are working on different 
sets of data, it seems there is little to no risk of race conditions.

My test 
========= 
Original: 9780 milliseconds 

Threaded: 5084 milliseconds
The program has been enhanced by 48% in performance.