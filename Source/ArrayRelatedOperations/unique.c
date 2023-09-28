void findUniqueElements(int arr[], int n) {
    // الجرد لحفظ العناصر الفريدة
    int uniqueElements[n];
    int uniqueCount = 0;

    // الجرد للتحقق من تكرار العناصر
    int isUnique;

    // تمرير عبر المصفوفة
    for (int i = 0; i < n; i++) {
        isUnique = 1;

        // التحقق من تكرار العنصر في الجزء السابق من المصفوفة
        for (int j = 0; j < i; j++) {
            if (arr[i] == arr[j]) {
                isUnique = 0;
                break;
            }
        }

        // إذا كان العنصر فريدًا ، قم بإضافته إلى الجرد
        if (isUnique) {
            uniqueElements[uniqueCount++] = arr[i];
        }
    }

    // طباعة العناصر الفريدة
    printf("العناصر الفريدة في المصفوفة:\n");
    for (int i = 0; i < uniqueCount; i++) {
        printf("%d ", uniqueElements[i]);
    }
    printf("\n");
}
