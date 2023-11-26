function split(str, token) {
    let result = [];
    let temp = "";

    for(let i = 0; i < str.length; i++) {
        if(str[i] !== token) {
            temp += str[i];
        } else {
            result.push(temp);
            temp = "";
        }
    }

    // Push the last substring if it's not empty
    if(temp !== "") {
        result.push(temp);
    }

    return result;
}

let str = "Hello,World,This,Is,A,Test";
let result = split(str, ',');

for(let i = 0; i < result.length; i++) {
    console.log(result[i]);
}
