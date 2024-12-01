#include <iostream>
#include <vector>
#include <pthread.h>

#include "iolib/std.h"


struct thread_data {
    std::vector<int> *arr;
    int p;
    int k;
    int thread_id;
    int threads_num;
};

void *thread_sort(void *arg) {
    auto *data = (thread_data *) arg;
    std::vector<int> &arr = *(data->arr);
    int p = data->p;
    int k = data->k;
    int n = arr.size();
    int num_threads = data->threads_num;
    int thread_id = data->thread_id;

    for (int i = thread_id; i < n - k; i += num_threads) {
        int j = i + k;
        if ((i & p) == 0 && arr[i] > arr[j]) {
            std::swap(arr[i], arr[j]);
        }
    }

    return nullptr;
}

void batcher_sort(std::vector<int> &arr, int max_threads) {
    int n = arr.size();
    int numThreads = std::min(max_threads, n);

    for (int p = 1; p < n; p *= 2) {
        for (int k = p; k > 0; k /= 2) {
            std::vector<pthread_t> threads(numThreads);
            std::vector<thread_data> threadData(numThreads);

            for (int i = 0; i < numThreads; ++i) {
                threadData[i] = {&arr, p, k, i, numThreads};
                pthread_create(&threads[i], nullptr, thread_sort, &threadData[i]);
            }
            for (int i = 0; i < numThreads; ++i) {
                pthread_join(threads[i], nullptr);
            }
        }
    }
}

int main(int argc, char **argv) {
    std::string prog_name(argv[0]);
    if (argc != 2) {
        log_stderr("usage: " + prog_name + " <max_threads>");
        return 1;
    }
    int maxThreads, arraySize;
    std::string buffer;
    maxThreads = atoi(argv[1]);
    std_out("enter array size:\n");
    std_in(&buffer);
    arraySize = atoi(buffer.c_str());

    std::vector<int> arr(arraySize);
    std::cout << "enter array elements:\n";
    for (int i = 0; i < arraySize; ++i) {
        std_in(&buffer);
        arr[i] = atoi(buffer.c_str());
    }

    std::cout << "initial array: ";
    for (int i = 0; i < arraySize; i++) {
        std::cout << arr[i] << " ";
    }

    std::cout << std::endl;

    batcher_sort(arr, maxThreads);

    std::cout << "sorted array: ";
    for (int i = 0; i < arraySize; i++) {
        std::cout << arr[i] << " ";
    }

    return 0;
}
