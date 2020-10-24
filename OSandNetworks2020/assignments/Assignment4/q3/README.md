# Q3

## Main logic

The logic I used here was to use semaphores as a shared resource to keep a counter
of the number of resources available in a global variable kind of thing. The
resources here are stages and coordinators.

## Major solutions to problems

One of the problems was on how can a musician wait for both type of stages.
To counter this, the musician thread created another thread which then checked
if another type of stage was available. A global flag for these two threads was
set which check if the other was done so as to not have a big problem.

## Compiling

Do:

```
make
./q3
```

To clean:

```
make clean
```
