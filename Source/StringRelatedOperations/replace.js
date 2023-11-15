function replace(str, oldChar, newChar) {
    var result = "";
    for(var i = 0; i < str.length; i++) {
        if(str[i] == oldChar) {
            result += newChar;
        } else {
            result += str[i];
        }
    }
    return result;
}

var str = "Hello, World!";
var replaced = replace(str, 'o', 'a');

console.log(replaced);
