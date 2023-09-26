void reduce(char a[4][10], char result[50], void (*f)(char*, char*)) {
    for(int i = 0; i < 4; i++) {
        f(result, a[i]);
    }
}
