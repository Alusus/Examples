int find_diff_index(int arr1[], int arr2[], int n) {
    int i;
    for(i = 0; i < n; i++) {
        if(arr1[i] != arr2[i]) {
            return i;
        }
    }
    return -1; // return -1 if no difference is found
}
