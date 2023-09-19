#include <stdio.h>
#include <limits.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

int maxCrossingSum(int arr[], int low, int mid, int high) {
    int sum = 0;
    int left_sum = INT_MIN;

    for (int i = mid; i >= low; i--) {
        sum += arr[i];
        if (sum > left_sum)
            left_sum = sum;
    }

    sum = 0;
    int right_sum = INT_MIN;

    for (int i = mid + 1; i <= high; i++) {
        sum += arr[i];
        if (sum > right_sum)
            right_sum = sum;
    }

    return max(max(left_sum, right_sum), left_sum + right_sum);
}

int maxSubarraySum(int arr[], int low, int high) {
    if (low == high)
        return arr[low];

    int mid = (low + high) / 2;

    return max(max(maxSubarraySum(arr, low, mid),
                   maxSubarraySum(arr, mid + 1, high)),
               maxCrossingSum(arr, low, mid, high));
}

int main() {
    int arr[] = {-2, -3, 4, -1, -2, 1, 5, -3};
    int n = sizeof(arr) / sizeof(arr[0]);

    int max_sum = maxSubarraySum(arr, 0, n - 1);
    printf("Maximum subarray sum is %d\n", max_sum);

    return 0;
}
