void clip(int arr[], int n, int lower, int upper) {
    for(int i = 0; i < n; i++) {
        if(arr[i] < lower) {
            arr[i] = lower;
        } else if(arr[i] > upper) {
            arr[i] = upper;
        }
    }
}
