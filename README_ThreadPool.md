For this task, we were instructed to design and implement a thread pool, which
is when a process creates and allocates a FIXED number of threads during its 
startup. This is done so that threads aren't created on DEMAND, which increases
dispatch latency and overall kernel overhead. 

The client API for the thread pool can be seen in Berde_threadpool.h; these
functions are all accessible to a client thread. The following functions are 
available to the client: pool_init(), pool_submit(void (*somefunction)(void *p
), void *p), and pool_shutdown().The source repository Professor Chen provided
us with defined worker(void *param) and execute(void (*somefunction)(void *p), 
void *p) within the client API as well, but I chose to omit them; those should
be private to Berde_threadpool.c. worker() is the function that's called by
the threads within the pool, and the function execute() actually calls the 
function passed by the user. These internal details should be abstract to the
user. 

In a program, if the user wishes to initialize a thread pool, they must call 
pool_init(). To submit a request to the thread pool, the function pool_submit() must be called, with the function to run and its parameters passed with the 
request. The program can submit requests at will, but the thread pool's waiting
queue has a LIMIT (maximum of 10). If the queue is full, the request will NOT
be successfully submitted. To close the thread pool, the user must call pool_shutdown(). 

To run Berde_client.c, simply type 'make' into the local terminal; using gcc 
would work as well, but make automatically does all of this for us. Afterwards,
run the command './example', which executes Berde_client.c. NOTE: the makefile 
was provided in the source repository given to us by Professor Chen in the 
assignment. I simply tweaked it to match my file names. 