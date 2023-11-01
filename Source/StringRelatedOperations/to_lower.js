function toLowerCase(s) {
    let result = '';
    for(let i = 0; i < s.length; i++){
        let code = s.charCodeAt(i);
        if(code >= 65 && code <= 90){
            result += String.fromCharCode(code + 32);
        } else {
            result += s[i];
        }
    }
    return result;
}

let s = "Hello, World!";
s = toLowerCase(s);
console.log(s);  // Outputs: hello, world!
