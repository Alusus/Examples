function char_exists(s, c) {
    for (let i = 0; i < s.length; i++) {
        if (s[i] === c) {
            return true;
        }
    }
    return false;
}
