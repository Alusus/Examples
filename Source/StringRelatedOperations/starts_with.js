function startsWith(str, prefix) {
    for(let i = 0; i < prefix.length; i++) {
        if(str[i] !== prefix[i])
            return false;
    }
    return true;
}
