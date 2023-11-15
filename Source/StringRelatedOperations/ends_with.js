function endsWith(str, suffix) {
    let j = suffix.length - 1;
    for(let i = str.length - 1; i >= 0 && j >= 0; i--, j--) {
        if(str[i] !== suffix[j])
            return false;
    }
    return j < 0;
}
