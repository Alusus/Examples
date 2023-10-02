void findUniqueElements(int arr[], int n, int uniqueElements[], int *uniqueCount) {
    // a flag used to indicate whether the element is unique or not
    int isUnique;

    // loop through the array
    for (int i = 0; i < n; i++) {
        isUnique = 1;

        // check if the element is occurred before
        for (int j = 0; j < i; j++) {
            if (arr[i] == arr[j]) {
                isUnique = 0;
                break;
            }
        }

        // if the element is unique add it to array
        if (isUnique) {
            uniqueElements[(*uniqueCount)++] = arr[i];
        }
    }
}
