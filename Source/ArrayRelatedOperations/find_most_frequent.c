int find_most_frequent(int arr[], int n) {
    int max_count = 0, res = -1;

    for (int i = 0; i < n; i++) {
        int count = 1;
        for (int j = i + 1; j < n; j++)
            if (arr[i] == arr[j])
                count++;
        if (count > max_count) {
            max_count = count;
            res = arr[i];
        }
    }

    return res;
}
