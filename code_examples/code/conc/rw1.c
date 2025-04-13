/*
 * Readers-writers solution with weak reader priority
 * From Courtois et al, CACM, 1971.
 * 
 *  Readers get in quickly — unless there's already a writer writing.
	Once a writer is done, if new readers keep arriving, the writer might never get a chance, because:
		- Readers can slip in while readcnt is going from 0 → 1.
		- The next writer is blocked at P(&w) because the first reader acquired it.
	So, writers may starve if there are many readers — this is why it’s called a "weak" reader priority solution.
 */
#include "csapp.h"

/* $begin reader1 */
/* Global variables 
readcnt: keeps track of how many readers are currently reading.
mutex: a binary semaphore used to protect updates to readcnt (so multiple threads don't change it at the same time).
w: a binary semaphore controlling access to the shared resource — only one writer or the first reader can acquire it.
*/

int readcnt;	/* Initially = 0 */
sem_t mutex, w; /* Both initially = 1 */

void reader(void)
{
	while (1)
	{	
		// Each reader acquires mutex before modifying readcnt to avoid race conditions.
		P(&mutex);       // Lock to modify readcnt
		readcnt++;
		if (readcnt == 1) /* First in, First reader locks the writer, All other readers just increment readcnt and proceed. */
			P(&w);       // Block writers
		V(&mutex);       // Done modifying readcnt
 
		/* Critical section */
		/* Reading happens  */

		P(&mutex);       // When a reader finishes, It locks mutex, decrements readcnt.
		readcnt--;
		if (readcnt == 0) /* If it is the last reader (i.e., readcnt == 0), it unlocks the writer by calling V(&w). */
			V(&w);      // Let writers in
		V(&mutex);     
		//This allows multiple readers to read simultaneously.
	}
}
/* $end reader1 */

/* $begin writer1 */

void writer(void)
{
	while (1)
	{	
		//So only one writer at a time can write, and no reader can read during writing.
		P(&w); // Writer locks the writer semaphore. This ensures mutual exclusion

		/* Critical section */
		/* Writing happens  */

		V(&w); //After writing, Writer releases the writer semaphore.
	}
}
/* $end writer1 */
