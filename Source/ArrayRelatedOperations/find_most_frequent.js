function findMostFrequent(arr) {
    var maxCount = 0, res = -1;

    for (var i = 0; i < arr.length; i++) {
        var count = 1;
        for (var j = i + 1; j < arr.length; j++)
            if (arr[i] == arr[j])
                count++;
        if (count > maxCount) {
            maxCount = count;
            res = arr[i];
        }
    }

    return res;
}
