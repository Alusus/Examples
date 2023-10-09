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

var a = [1, 2, 2, 3, 3, 3, 4, 4, 4, 4];
freq = findFrequency(a);

for(var key in freq) {
    console.log("Element " + key + " occurs " + freq[key] + " times");
}
