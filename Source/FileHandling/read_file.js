const fs = require('fs');

// Function to read the content of a file into a string one character at a time
function read_file(file_path) {
    const fileDescriptor = fs.openSync(file_path, 'r');
    let file_content = '';
    let buffer = Buffer.alloc(1);

    // Read the file one character at a time
    while(fs.readSync(fileDescriptor, buffer, 0, 1)) {
        file_content += buffer.toString();
    }

    fs.closeSync(fileDescriptor);
    return file_content;
}
