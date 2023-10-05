function std(data, n, mean) {
    let variance = 0.0;

    for(let i=0; i<n; ++i)
        variance += (data[i] - mean) * (data[i] - mean);

    return Math.sqrt(variance/n);
}
