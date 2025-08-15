#include <sys/time.h>
#include <errno.h>

int _gettimeofday(struct timeval *tv, void *tzvp) {
    (void)tzvp;
    if (tv) {
        tv->tv_sec  = 0;
        tv->tv_usec = 0;
    }
    return 0; // success
}