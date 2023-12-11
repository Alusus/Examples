function slice_string(str, start, end) {
    var result = "";
    for(var i = start; i <= end; i++) {
        result += str[i];
    }
    return result;
}

var str = "Hello, World!";
var sliced = slice_string(str, 7, 12);

console.log(sliced);
