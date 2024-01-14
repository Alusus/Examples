function findFrequency(a) {
    let freq = {};
    for(let i = 0; i < a.length; i++) {
        const key = a[i].toString();
        if(freq[key] === undefined) {
            freq[key] = 1;
        } else {
            freq[key]++;
        }
    }

    return freq;
}
