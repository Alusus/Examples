function slice_string(str, start, end) {
    var result = "";
    for(var i = start; i <= end; i++) {
        result += str[i];
    }
    return result;
}
