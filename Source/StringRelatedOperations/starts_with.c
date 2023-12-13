int starts_with(const char *str, const char *prefix) {
    while(*prefix) {
        if (!(*str))
            return 0;
        if(*prefix++ != *str++)
            return 0;
    }
    return 1;
}
