int find_char(char s[], int n, char c) {
    for (int i = 0; i < n; i++) {
        if (s[i] == c) {
            return i;
        }
    }
    return -1;
}
