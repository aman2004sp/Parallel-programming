#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;

int main() {
    int n, num_threads;

    cout << "Enter number of elements: ";
    cin >> n;

    cout << "Enter number of threads (buckets): ";
    cin >> num_threads;

    vector<int> data(n);
    cout << "Enter the elements:" << endl;
    for (int i = 0; i < n; ++i)
        cin >> data[i];

    // Find the min and max to determine the range
    int min_val = *min_element(data.begin(), data.end());
    int max_val = *max_element(data.begin(), data.end());

    int range = max_val - min_val + 1;
    int bucket_size = (range + num_threads - 1) / num_threads; // ceiling division

    vector<vector<int>> buckets(num_threads);

    // Distribute elements into buckets (parallelized)
    #pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();
        vector<int> local_bucket;

        #pragma omp for nowait
        for (int i = 0; i < n; ++i) {
            int b = (data[i] - min_val) / bucket_size;
            if (b >= num_threads) b = num_threads - 1; // safety bound
            if (b == tid)
                local_bucket.push_back(data[i]);
        }

        #pragma omp critical
        buckets[tid].insert(buckets[tid].end(), local_bucket.begin(), local_bucket.end());
    }

    // Sort each bucket in parallel
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < num_threads; ++i)
        sort(buckets[i].begin(), buckets[i].end());

    // Merge buckets
    vector<int> sorted_data;
    for (int i = 0; i < num_threads; ++i++)
        sorted_data.insert(sorted_data.end(), buckets[i].begin(), buckets[i].end());

    // Output the result
    cout << "Sorted data:" << endl;
    for (int num : sorted_data)
        cout << num << " ";
    cout << endl;

    return 0;
}
