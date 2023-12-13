function trim(str) {
    let start = 0, end = str.length - 1;

    // Trim leading white spaces
    while(str[start] === ' ' || str[start] === '\t' || str[start] === '\n') start++;

    // Trim trailing white spaces
    while(str[end] === ' ' || str[end] === '\t' || str[end] === '\n') end--;

    // Return the trimmed string
    return str.substring(start, end + 1);
}

let str = "   Hello, World!   ";
console.log("Original string: '" + str + "'");
str = trim(str);
console.log("Trimmed string: '" + str + "'");
