#include <vector>
#include <pthread.h>
#include <mutex>
#include <cmath>
#include <sys/time.h>
#include "iolib/std.h"

pthread_mutex_t mutex;

struct ThreadData
{
    std::vector<int> *sequence;
    int n;
    int p;
    int k;
    int threadId;
    int numThreads;
};

void compare_and_exchange(int &a, int &b)
{
    if (b < a)
    {
        std::swap(a, b);
    }
}

void *thread_function(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    std::vector<int> &sequence = *data->sequence;
    int n = data->n;
    int p = data->p;
    int k = data->k;
    int threadId = data->threadId;
    int numThreads = data->numThreads;

    for (int j = k % p + threadId * 2 * k; j <= n - 1 - k; j += 2 * k * numThreads)
    {
        for (int i = 0; i <= std::min(k - 1, n - j - k - 1); ++i)
        {
            if (std::floor((i + j) / (p * 2)) == std::floor((i + j + k) / (p * 2)))
            {
                pthread_mutex_lock(&mutex);
                compare_and_exchange(sequence[i + j], sequence[i + j + k]);
                pthread_mutex_unlock(&mutex);
            }
        }
    }

    pthread_exit(NULL);
}

void betcher_sort(std::vector<int> &v, int max_threads)
{
    int n = v.size();
    for (int p = 1; p < n; p *= 2)
    {
        for (int k = p; k >= 1; k /= 2)
        {
            pthread_t threads[max_threads];
            ThreadData threadData[max_threads];

            for (int t = 0; t < max_threads; ++t)
            {
                threadData[t] = {&v, n, p, k, t, max_threads};
                pthread_create(&threads[t], NULL, thread_function, (void *)&threadData[t]);
            }

            for (int t = 0; t < max_threads; ++t)
            {
                pthread_join(threads[t], NULL);
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std_out("invalid numer of args. enter max number of threads and array len\n");
        return 1;
    }
    int max_threads = atoi(argv[1]);
    int len = atoi(argv[2]);

    std::srand(std::time(0));

    std::string buffer;
    std::vector<int> unsorted_vector(len);

    for (int i = 0; i < len; i++)
    {
        unsorted_vector[i] = std::rand() % len;
    }
    std_out("Unsorted array:\n");
    for (int i = 0; i < len; i++)
    {
        std_out(std::to_string(unsorted_vector[i]) + " ");
    }
    std_out("\nSorted array:\n");

    struct timeval start, end;
    pthread_mutex_init(&mutex, NULL);
    gettimeofday(&start, NULL);
    betcher_sort(unsorted_vector, max_threads);
    gettimeofday(&end, NULL);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    for (int i = 0; i < len; i++)
    {
        std_out(std::to_string(unsorted_vector[i]) + " ");
    }
    std_out("\n");
    std_out("Time elapsed:" + std::to_string(time_spent) + " s\n");
    pthread_mutex_destroy(&mutex);
    return 0;
}
