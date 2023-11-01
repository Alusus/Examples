int rfind_char(char s[], int n, char c) {
    for (int i = n-1; i != 0; i--) {
        if (s[i] == c) {
            return i;
        }
    }
    return -1;
}
