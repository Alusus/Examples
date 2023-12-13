int ends_with(const char *str, const char *suffix) {
    const char *a = str;
    const char *b = suffix;

    // Find the end of both strings
    while(*a) a++;
    while(*b) b++;

    // Walk backwards until we find a mismatch or reach the start
    while(a >= str && b >= suffix) {
        if(*a-- != *b--)
            return 0;
    }

    // If we reached the start of the suffix we found a match
    return b < suffix;
}
