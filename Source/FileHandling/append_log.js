const fs = require('fs');

function appendLogEntry(filename, logEntry) {
    let stream = fs.createWriteStream(filename, {flags:'a'});
    stream.write(logEntry + '\n');
    stream.end();
}

appendLogEntry('log.txt', 'This is a log entry.');
