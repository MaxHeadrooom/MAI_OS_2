#include <iostream>
#include <vector>
#include <stack>

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quicksort(std::vector<int>& arr) {
    std::stack<std::pair<int, int>> stack;
    stack.push(std::make_pair(0, 100 - 1));

    while (!stack.empty()) {
        int low = stack.top().first;
        int high = stack.top().second;
        stack.pop();

        if (low < high) {
            int pivot_index = partition(arr, low, high);
            if (pivot_index > low)
                stack.push(std::make_pair(low, pivot_index - 1));
            if (pivot_index < high)
                stack.push(std::make_pair(pivot_index + 1, high));
        }
    }
}

int main() {
    long long n;

    n = 100;

    std::vector<int> arr(100);
    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }
    quicksort(arr);

    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
