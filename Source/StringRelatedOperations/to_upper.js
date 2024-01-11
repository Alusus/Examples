function toUpperCase(s) {
    let result = '';
    for(let i = 0; i < s.length; i++){
        let code = s.charCodeAt(i);
        if(code >= 97 && code <= 122){
            result += String.fromCharCode(code - 97 + 65);
        } else {
            result += s[i];
        }
    }
    return result;
}
