# CSC360 Assignment2: Syncronization by Kanato Sato (V00948631)
## Task1: Reader/Writer Problem with semaphores
Task1 is done in **rw.c**

In *Initialize_readers_writer()*, two semaphores are declared, named R and W corresponding to reader and writer respectively.
Also resource_t data is declared to store written value.

*void rw_read(char \*value, int len)* function checks whether there is some thread that is writing. If such thread exists, 
all threads that are reading get paused due to *sem_wait(&R)*. If such threads don't exist, *resource_read()* function is called from resource.c 
and reads data (resource_t data).
Also, this function counts the number of reader threads so writer threads can be resumed by *sem_post(&W)* when the count hits 0.

*void rw_write(char \*value, int len)* function does the opposite. The function checks whether reader threads exist. If so, paused by *sem_wait(&W)*. 
Otherwise, start writing and when number of writer threads hits 0, *sem_post(&R)* lets reader threads resume. 

## Task2: Reusable Barrier with mutex and condition value
Task2 is done in **meetup.c**

In *initialize_meetup(int n, int mf)*, capacity of a group (int n) is stored in global variable (int cap) and codeword teller (int mf) is stored in global variable (int teller). Also, resource_t codeword is declared to store codeword of a group. When int n is not >0, error occurs since there is no such thing as group of negative number of people.

*join_meetup(char \*value, int len)* locks the critical part so the first (int cap) threads to arrive can shre the codeword.
By comparing my_generation to generation, threads get to know which group they belong to.
By checking global variables (int cap) and (int teller), the function sets the codeword for a group. 
After unlocking mutex, the threads prints out the shared codeword by calling *read_resource(&codeword, value, len)*.

## References
Dr. Michael Zastre: Uvic csc360 slide and sample codes
