# Q2

## Main logic 

The logic I used was to busy-wait instead of using conditions or semaphores
as that wasn't allowed. 

## More details

I treated the zones as a shared resources which the student threads and the
company threads can access. The reason for doing this was there are two type of
interactions happening: companies <--> zones AND zones <--> students. So, to reduce
the amount of code to be written, I treated the zones as a common resource and had
the companies and students query over them multiple times and do work if required.

## Compiling

Do:

```
make
./q2
```

To clean all files:

```
make clean
```
