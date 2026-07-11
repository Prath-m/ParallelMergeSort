#include "parallelMergeSort.hpp"
#include <cmath>

ParallelMergeSort::ParallelMergeSort(std::vector<int> *nums) 
    : nums(nums) {
}

ParallelMergeSort::~ParallelMergeSort() {}

void ParallelMergeSort::recursiveSort(int left, int right, int depth) {

    const int THRESHOLD = 5000;
    static const int MAX_DEPTH = (int)std::log2(std::thread::hardware_concurrency());

    if (right - left < THRESHOLD) {
        std::sort(nums->begin() + left, nums->begin() + right + 1);
        return;
    }
    int mid = left + (right - left) / 2;

    if (depth >= MAX_DEPTH) {
        recursiveSort(left, mid, depth);
        recursiveSort(mid + 1, right, depth);
    } else {
        std::thread thread_1([this, left, mid, depth] { this->recursiveSort(left, mid, depth + 1); });
        std::thread thread_2([this, mid, right, depth] { this->recursiveSort(mid + 1, right, depth + 1); });
        thread_1.join();
        thread_2.join();
    }

    std::vector<int> result;
    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        if ((*nums)[i] <= (*nums)[j]) {
            result.push_back((*nums)[i]);
            i++;
        } else {
            result.push_back((*nums)[j]);
            j++;
        }
    }

    while (i <= mid) {
        result.push_back((*nums)[i]);
        i++;
    }

    while (j <= right) {
        result.push_back((*nums)[j]);
        j++;
    }

    for (size_t k = 0; k < result.size(); k++) {
        (*nums)[left + k] = result[k];
    }
}

void ParallelMergeSort::sort() {
    recursiveSort(0, (*nums).size() - 1, 0);
}