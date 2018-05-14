
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

#define SIZE 100
typedef struct {
    int64_t *list;
    int64_t *clone;
    int64_t low;
    int64_t high;
    int64_t level;
} thread_data;
 
//merge the elements from the sorted sublist in [low, mid] and (mid, high]
void merge(int64_t* list, int64_t* clone, int64_t low, int64_t mid, int64_t high) {
    int64_t i = low;
    int64_t j = mid + 1;
    int64_t k = low;

    while (i <= mid && j <= high) {
        if (list[i] <= list[j]) {
            clone[k] = list[i];
            ++i;
        } else {
            clone[k] = list[j];
            ++j;
        }
        ++k;
    }

    if (i > mid) {
        memcpy(&clone[k], &list[j], (high - j + 1) * sizeof(int64_t));
    } else {
        memcpy(&clone[k], &list[i], (mid - i + 1) * sizeof(int64_t));
    }

    memcpy(&list[low], &clone[low], (high-low+1) * sizeof(int64_t));
}

void merge_worker(int64_t list[], int64_t clone[], int64_t low, int64_t high)
{
    if (low == high)
        return;

    int64_t mid = (int64_t)((low + high) * 0.5f);

    merge_worker(list, clone, low, mid);
    merge_worker(list, clone, mid+1, high);

    merge(list, clone, low, mid, high);
}


void *sort_worker(void *arg) {
    int64_t t;
    int64_t mid;

    thread_data td = *(thread_data*) arg;
    
    if (td.level <= 0 || td.low == td.high)
    {
        // enough threads were spawned (2^td.level)
        merge_worker (
            td.list,
            td.clone, 
            td.low,
            td.high
        );

        pthread_exit(NULL);
    }

    // create L and R threads
    mid = (int64_t)((td.low + td.high) * 0.5f);

    thread_data dataX = {
        td.list,
        td.clone,
        td.low,
        mid,
        td.level - 1
    };

    thread_data dataY = {
        td.list,
        td.clone,
        mid + 1,
        td.high,
        td.level - 1
    };

    //Now, instantiate the threads.
    pthread_t threads[2];
    pthread_create(&threads[0], NULL, sort_worker, (void*)&dataX);
    pthread_create(&threads[1], NULL, sort_worker, (void*)&dataY);
    
    void *pv;
    pthread_join(threads[0], &pv);
    pthread_join(threads[1], &pv);

    merge(td.list, td.clone, td.low, mid, td.high);
    pthread_exit(NULL);
}

void sort(int64_t* list, int64_t size, int64_t tlevel)
{
    int64_t *clone = (int64_t*)malloc(size * sizeof(int64_t));

    thread_data td;
    td.list = list;
    td.clone = clone;
    td.low = 0;
    td.high = size - 1;
    td.level = tlevel;

    // create the workers
    pthread_t t;
    pthread_create(&t, NULL, sort_worker, (void *) &td);

    void *pv;
    pthread_join(t, &pv);

    free(clone);
}

int main() {

    srand(time(NULL));
    
    int64_t *a = (int64_t*)malloc(sizeof(int64_t) * SIZE);
    for (int64_t i = 0; i < SIZE; i++) {
        a[i] = rand();
    }

    sort(a, SIZE, 3);

    for (int64_t i = 0; i < SIZE - 1; i++) {
        printf("%d\n", a[i]);

        if (a[i] > a[i + 1]) {
            puts("failed");
        }
    }

    puts("done");
}
