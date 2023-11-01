function rfind_char(s, c) {
    for (let i = s.length - 1; i !== 0; i--) {
        if (s[i] === c) {
            return i;
        }
    }
    return -1;
}
