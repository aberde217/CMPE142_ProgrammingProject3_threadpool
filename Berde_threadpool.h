void pool_init(); 
int pool_submit(void (*somefunction)(void *p), void *p);
void pool_shutdown();