void toUpperCase(char *s, char *result) {
    for(int i = 0; s[i]; i++){
        if(s[i] >= 'a' && s[i] <= 'z'){
            result[i] = s[i] - 'a' + 'A';
        }
        else {
            result[i] = s[i];
        }
    }
}
