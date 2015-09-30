#include "clog.h"
#include <pthread.h>


void test(clog_t *clog)
{
    int i;

    for (i = 0; i < 10000000; ++i)
        clog_logging(CLOG_ALERT, clog, 123, "finyren %d", i);
}

void *fun(void *arg)
{
    clog_t *clog = (clog_t *)arg;

    test(clog);

    return NULL;
}


int main(int argc, char const *argv[])
{
    clog_t *clog = clog_init("log_file", CLOG_DEBUG, 0);
    pthread_t tid_1, tid_2, tid_3, tid_4;

    pthread_create(&tid_1, NULL, fun, clog);
    pthread_create(&tid_2, NULL, fun, clog);
    pthread_create(&tid_3, NULL, fun, clog);
    pthread_create(&tid_4, NULL, fun, clog);

    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);
    pthread_join(tid_3, NULL);
    pthread_join(tid_4, NULL);

    return 0;
}

