BoundedBuffer-HW
================
Implment a producer-consumer program using a bounded buffer. Use fulland emptysemaphores to
keep track of the number of full and empty slots available to the consumers and producers, respectively.
Use a mutex to coordinate access to the buffer once it is determined that there is an available slot via
the appropriate semaphore. Command-line parameters should be used to set the number of slots in the
buffer, the number of producers, the number of consumers, and the number of items each producer
producesbefore exiting. The consumers need to consume all items produced. (The number of items
each consumer will need to consume can be calculated from these parameters.) The main thread should
parse all of these command-line parameters, print them in a message,initialize the buffer and
synchronization objects, spawn all of the producer and consumerthreads, wait for the threads to
complete, and then print a final message. The producer and consumer threads should print each item
they produce or consume and update the buffer accordingly. (Printing is the only required
"consumption" of the items.) The items produced should be integers obtained using the following
expression:

thread_number * 1000000 + counter

where counteris a local variable in each thread that gets initialized to 0and incremented every time
a new item is produced. (Notice this limits the number of items produced by each producer to
1,000,000.)


Use the same functions listed in homework 4 for thread creation and management. Use
pthread_mutex_init, pthread_mutex_lock, pthread_mutex_unlock, sem_init,
sem_waitand sem_postfor synchronization in POSIX. Use CreateSemaphore,
WaitForSingleObjectand ReleaseSemaphorefor synchronization in Win32.
