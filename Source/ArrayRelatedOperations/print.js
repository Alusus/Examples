function printArray(a, size) {
    var output = '';
    for(var i = 0; i < size; i++) {
        output += a[i] + ' ';
    }
    console.log(output);
}

var a = [1, 2, 3, 4, 5];
var size = a.length;

printArray(a, size);
