function calculateMean(arr) {
    var sum = 0;
    for (var i = 0; i < arr.length; i++) {
        sum += arr[i];
    }
    return sum / arr.length;
}

var arr = [1, 2, 3, 4, 5];
console.log("Mean = " + calculateMean(arr));
