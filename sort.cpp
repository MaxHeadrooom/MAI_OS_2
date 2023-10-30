#include <bits/stdc++.h>
#include <chrono>
#include <pthread.h>

using namespace std;

using ll = long long;

const int MAX_THREADS = 2;

typedef struct args_ins 
{
    ll* array;
    ll start;
    ll end;
} args_ins_t;

ll partition(args_ins_t args) 
{
    ll high = args.end;
    ll low = args.start;

    ll pivot = args.array[high];
    ll i = (low - 1);

    for (int j = low; j <= high - 1; j++) 
    {
        if (args.array[j] <= pivot) 
        {
            i++;
            swap(args.array[i], args.array[j]);
        }
    }
    swap(args.array[i + 1], args.array[high]);
    return (i + 1);
}

void* quickSort(void* input) 
{
    args_ins_t* ar = (args_ins_t*)input;
    ll* arr = ar->array;
    ll start = ar->start;
    ll end = ar->end;
    stack<pair<ll, ll>> stack;
    stack.push(make_pair(start, end));

    if (start == 0 and end == 32)
    {
        true;
    }

    vector<ll> a;

    while (!stack.empty()) 
    {
        for (int i = start; i < end; i++) 
        {
            a.push_back(ar->array[i]); // Копируем элементы из ar в arr
        }

        ll low = stack.top().first; 
        ll high = stack.top().second;
        stack.pop();

        if (low < high) 
        {
            args_ins_t arg;
            arg.array = arr;
            arg.end = high;
            arg.start = low; 
            ll pivot_index = partition(arg);
            if (pivot_index > low)
                stack.push(make_pair(low, pivot_index - 1));
            if (pivot_index < high)
                stack.push(make_pair(pivot_index + 1, high));
        }
    }
}

void potok(vector<ll>& arr, ll low, ll high)
{
    pthread_t threads[MAX_THREADS];
    ll partitions[MAX_THREADS];

    partitions[0] = low;

    for (int i = 1; i < MAX_THREADS; i++) {
        partitions[i] = partitions[i - 1] + (high - partitions[i - 1]) / (MAX_THREADS - i);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        ll new_low = partitions[i];
        ll new_high;

        if (i == MAX_THREADS - 1)
            new_high = high;
        else
            new_high = partitions[i + 1] - 1;

        if (new_low < new_high) 
        {
            args_ins_t* data = (args_ins_t*)malloc(sizeof(args_ins_t));
            data->array = arr.data(); // Передача указателя на массив
            data->start = new_low; // Установка начального индекса
            data->end = new_high; // Установка конечного индекса
            pthread_create(&threads[i], NULL, quickSort, data);
        }
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL); // Ожидание завершения потоков
    }

    // Вывод для отладки (может быть убран)
    // for (int j = 0; j <= 32; j++) 
    //     cout << arr[j] << " ";
    // cout << '\n';

    
}


int main() 
{
    ll n;

    cin >> n;

    vector<ll> arr(n);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    auto start = chrono::high_resolution_clock::now();

    // Создание потоков
    potok(arr, 0, n - 1);

    auto end = chrono::high_resolution_clock::now();

    // Ожидание завершения потоков
    for (int i = 0; i < n; i++) 
    {
        cout << arr[i] << " ";
    }

    cout << '\n';

    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    cout << duration.count() << '\n';

    return 0;
}