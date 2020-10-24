# Q1

Normal merge sort works better than the others. 
Output:

Normal > Threaded > Multiprocess

## Reasons

Reason for this happening is that while Multi process merge sort and threaded
merge sort use all cores, they create so many processes that the context swtiches
take a lot of time and the benifet of using multiple cores is wasted. To counter
this, I also have a `parasort.c` which uses all cores.

## Further testing

Suprisingly, my `parasort.c` and `mergesort.c` work better than C++ sort!

## Compiling

Although the guiidelines mention one q1.c, I created different files so that
each sort has equal conditions since we are benchmarking them. As the process
goes on, its priority decreases and then the sorts do not have same environment

To compile, do `gcc <file_name>`
