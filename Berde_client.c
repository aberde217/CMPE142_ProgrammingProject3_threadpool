#include "Berde_threadpool.h"
#include <stdio.h>
#include <unistd.h>

struct data{
    int num1;
    int num2;
};

void multiply(void *param) {
    struct data *nums = param;
    int product = (nums->num1) * (nums->num2);
    printf("%d x %d = %d\n", nums->num1, nums->num2, product);
}

int main() {
    struct data d;
    d.num1 = 50;
    d.num2 = 2;

    pool_init();
    int status = pool_submit(&multiply, &d);
    if (status == 0) {
        printf("Request Succeeded.\n");
    }
    sleep(3);
    pool_shutdown();
}