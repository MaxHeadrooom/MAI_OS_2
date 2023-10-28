#include <bits/stdc++.h>
#include <chrono>
#include <pthread.h>

using namespace std;

using ll = long long;

const int MAX_THREADS = 4;

typedef struct args_ins 
{
    ll* array;
    ll start;
    ll end;
} args_ins_t;

typedef struct args_left_right 
{
    args_ins_t left_args;
    args_ins_t right_args;
} args_left_right_t;

void* quickSort(void* input) 
{
    args_left_right_t* args = (args_left_right_t*)input;
    ll* arr = args->left_args.array;
    ll left = args->left_args.start;
    ll right = args->left_args.end;

    if (left < right) 
    {
        ll pivot = arr[left];
        ll low = left + 1;
        ll high = right;

        while (true) 
        {
            while (low <= high and arr[low] <= pivot)
                low++;
            while (arr[high] >= pivot && high >= low)
                high--;
            if (high < low)
                break;
            else
                swap(arr[low], arr[high]);
        }

        swap(arr[left], arr[high]);

        args_ins_t left_args = args->left_args;
        args_ins_t right_args = args->right_args;

        left_args.end = high - 1;
        right_args.start = high + 1;

        args_left_right_t new_args;
        new_args.left_args = left_args;
        new_args.right_args = right_args;

        pthread_t left_thread, right_thread;
        pthread_create(&left_thread, NULL, quickSort, &new_args);

        // Запуск сортировки правой части в текущем потоке
        quickSort(&right_args);

        pthread_join(left_thread, NULL); // Ожидание завершения левого потока
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
            data->array = &(arr[partitions[i]]); // Передача указателя на массив
            data->start = new_low;
            data->end = new_high;
            pthread_create(&threads[i], NULL, quickSort, data);
        }
        // Вывод для отладки (может быть убран)
        for (int j = new_low; j <= new_high; j++) 
            cout << arr[j] << " ";
        cout << '\n';
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL); // Ожидание завершения потоков
    }
}


int main() 
{
    ll n;

    cin >> n;

    ll* arr = new ll[n];
    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }

    //auto start = chrono::high_resolution_clock::now();

    pthread_t threads[MAX_THREADS];
    args_ins_t* data = (args_ins_t*)malloc(sizeof(args_ins_t));
    data->array = arr; // Передача указателя на массив
    data->start = 0;
    data->end = 32;


    //quickSort(arr, 0, n - 1);

    pthread_create(&threads[0], NULL, quickSort, data);

    //potok(m, 0, n - 1);

    //auto end = chrono::high_resolution_clock::now();

    for (int i = 0; i < n; i++) 
    {
        cout << arr[i] << " ";
    }

    cout << '\n';

    //auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

    //cout << "microsec: " <<  duration.count() << '\n';

    return 0;
}