function is_string_alphanumeric(s) {
    for (let i = 0; i < s.length; i++) {
        console.log(s[i]);
        const c = s[i];
        if ((c < 'a' || c > 'z') && (c < 'A' || c > 'Z') && (c < '0' || c > '9')) {
            return false;
        }
    }
    return true;
}
