const readline = require('readline');
const fs = require('fs');
const path = require('path');

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

rl.question('Enter root directory [./]: ', (rootDirectory) => {
  rootDirectory = rootDirectory || './';

  rl.question('Enter file extensions (comma-separated) [.save]: ', (fileExtensions) => {
    fileExtensions = fileExtensions || '.save';
    const extensions = fileExtensions.split(',');

    console.log(`\nPerforming recursive file deletion in '${rootDirectory}' with file extensions: ${extensions}\n`);

    deleteFiles(rootDirectory, extensions);

    rl.close();
  });
});

function deleteFiles(directory, extensions) {
  fs.readdirSync(directory).forEach((file) => {
    const filePath = path.join(directory, file);
    const stats = fs.statSync(filePath);

    if (stats.isFile()) {
      for (const extension of extensions) {
        if (file.endsWith(extension.trim())) {
          console.log(`Deleting file: ${filePath}`);
          fs.unlinkSync(filePath);
          break;
        }
      }
    } else if (stats.isDirectory()) {
      deleteFiles(filePath, extensions);
    }
  });
}
