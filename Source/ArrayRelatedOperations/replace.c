void replace(int* arr, int len, int oldValue, int newValue) {
    for(int i = 0; i < len; i++) {
        if(arr[i] == oldValue) {
            arr[i] = newValue;
        }
    }
}
