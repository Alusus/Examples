void slice(int arr[], int start, int end, int result[]) {
    int i;
    for(i = start; i <= end; i++) {
        result[i - start] = arr[i];
    }
}
