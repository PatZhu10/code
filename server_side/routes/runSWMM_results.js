// server_side/web/js/results.js

const express = require('express');
const router = express.Router();
const archiver = require('archiver');
const path = require('path');

router.post('/', (req, res) => {

    // Array of file paths to include in the zip
    const parentDir = path.join(__dirname, '..', '..');
    const files = [
        path.join(parentDir, 'user_data/Zhonggang_HAIMA_2004.out'),
        path.join(parentDir, 'user_data/Zhonggang_HAIMA_2004.rpt')
    ];

    // Create an array of objects specifying the file name and content for each file
    const filesData = files.map(filePath => {
        return { path: filePath, name: filePath.split('/').pop() };
    });

    // Create the zip file using archiver and send it as a response when requested
    const archive = archiver('zip', {
        zlib: { level: 9 } // Sets the compression level
    });

    // Set the filename for the response
    res.attachment('swmm_files.zip');

    // Pipe the archive to the response stream
    archive.pipe(res);

    // Append each file to the archive
    filesData.forEach(file => {
        archive.file(file.path, { name: file.name });
    });

    // Finalize the archive
    archive.finalize();
});

module.exports = router;

