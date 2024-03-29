const fs = require('fs');
const readline = require('readline');

function read_nth_line(file_path, n) {
    return new Promise((resolve, reject) => {
        const fileStream = fs.createReadStream(file_path);

        const rl = readline.createInterface({
            input: fileStream,
            crlfDelay: Infinity
        });

        let line_no = 0;

        rl.on('line', (line) => {
            line_no++;
            if(line_no === n) {
                resolve(line);
                rl.close();
            }
        });

        rl.on('close', () => {
            if(line_no < n) {
                reject(new Error(`The file has less than ${n} lines.`));
            }
        });
    });
}
