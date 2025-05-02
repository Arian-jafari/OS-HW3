#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int* arr;
    int low;
    int high;
} sort_thread;

typedef struct {
    int* arr;
    int left;
    int mid;
    int right;
} merge_thread;

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void* quickSort_wrapper(void* arg) {
    sort_thread* data = (sort_thread*)arg;
    quickSort(data->arr, data->low, data->high);
    return NULL;
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void* merge_wrapper(void* arg) {
    merge_thread* data = (merge_thread*)arg;
    merge(data->arr, data->left, data->mid, data->right);
    return NULL;
}

int main() {
    // making a test arbirary array
    int arr[] = {6, 7, 5, 1, 3, 9, 8, 4, 0, 2};
    int n = sizeof(arr)/sizeof(arr[0]);
    int mid = n / 2;

    pthread_t sort_thread1, sort_thread2, merge_t;
    
    sort_thread first_half = {arr, 0, mid - 1};
    sort_thread second_half = {arr, mid, n - 1};
    
    pthread_create(&sort_thread1, NULL, quickSort_wrapper, &first_half);
    pthread_create(&sort_thread2, NULL, quickSort_wrapper, &second_half);

    pthread_join(sort_thread1, NULL);
    pthread_join(sort_thread2, NULL);

    merge_thread merge_data = {arr, 0, mid - 1, n - 1};
    
    pthread_create(&merge_t, NULL, merge_wrapper, &merge_data);
    
    pthread_join(merge_t, NULL);

    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");

    return 0;
}