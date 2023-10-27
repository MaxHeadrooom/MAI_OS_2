#include <bits/stdc++.h>
#include <chrono>

using namespace std;

using ll = long long;

void quickSort(vector<ll>& arr, ll low, ll high) 
{
    if (low < high) 
    {
        ll pivot = arr[low]; // Выбираем опорный элемент (pivot)
        ll left = low + 1;
        ll right = high;

        while (true) 
        {
            while (left <= right and arr[left] <= pivot) 
                left++;
            while (arr[right] >= pivot and right >= left) 
                right--;
            if (right < left) 
                break;
            else 
                swap(arr[left], arr[right]);
        }

        swap(arr[low], arr[right]);

        quickSort(arr, low, right - 1);
        quickSort(arr, right + 1, high);
    }
}

int main() 
{
    ll n;

    cin >> n;

    vector<ll> m(n);

    for (int i = 0; i < n; i++)
    {
        cin >> m[i];
    }

    auto start = chrono::high_resolution_clock::now();

    quickSort(m, 0, n - 1);

    auto end = chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) 
    {
        cout << m[i] << " ";
    }

    cout << '\n';

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << duration.count();

    return 0;
}
