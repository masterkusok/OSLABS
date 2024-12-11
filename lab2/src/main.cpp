#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include "iolib/std.h"

struct ThreadData
{
    std::vector<int> *sequence;
    int n;
    int p;
    int k;
    int threadId;
    int numThreads;
};

sem_t semaphore;

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
            sem_wait(&semaphore);
            if (std::floor((i + j) / (p * 2)) == std::floor((i + j + k) / (p * 2)))
            {
                compare_and_exchange(sequence[i + j], sequence[i + j + k]);
            }
            sem_post(&semaphore);
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
    if (argc != 2)
    {
        std_out("invalid numer of args. enter max number of threads\n");
        return 1;
    }
    int max_threads = atoi(argv[1]);
    std::string buffer;
    std_in(&buffer);
    int len = atoi(buffer.c_str());

    std::vector<int> unsorted_vector(len);
    for (int i = 0; i < len; i++)
    {
        buffer.clear();
        std_in(&buffer);
        unsorted_vector[i] = atoi(buffer.c_str());
    }
    sem_init(&semaphore, 0, 4);
    betcher_sort(unsorted_vector, 4);
    for (int i = 0; i < len; i++)
    {
        std_out(std::to_string(unsorted_vector[i]) + " ");
    }
    sem_destroy(&semaphore);
    return 0;
}
