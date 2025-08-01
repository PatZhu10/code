//  server_side/routes/file_process.js

const { Router } = require('express');
const fileUpload = require('express-fileupload');
const router = Router();
let targetPath;
let inpFileName;

const read_convert_coordinate = require('./read_convert_coordinate');

router.use(
    fileUpload({
        useTempFiles: true,
        tempFileDir: '../user_data/',
        createParentPath: true,
    })
);

router.post('/', async (req, res) => {
    const uploadedFiles = req.files;
    // console.log(uploadedFiles);
    if (!uploadedFiles || Object.keys(uploadedFiles).length === 0) {
        return res.status(400).json({ error: 'No files uploaded.' });
    }
    
    // Iterate over the uploaded files
    for (const [fileName, uploadedFile] of Object.entries(uploadedFiles)) {
        targetPath = `../user_data/${uploadedFile.name}`;

        // Move each file to the target path
        try {
            await new Promise((resolve, reject) => {
                uploadedFile.mv(targetPath, (err) => {
                    if (err) {
                        reject(err);
                    } else {
                        resolve();
                    }
                });
            });
            // Check if the filename has the ".inp" extension
            if (fileName.endsWith('.inp')) {
                inpFileName = fileName;
                break; // Break out of the loop after finding the first ".inp" file
            }
        } catch (error) {
            console.error('Error moving file:', error);
            return res.status(500).json({ error: 'Error moving file.', details: error.message });
        }
    }
    if (inpFileName) {
        const targetPath = `../user_data/${inpFileName}`;
        try {
            const coordinates = await read_convert_coordinate(targetPath);
            // console.log('Coordinates:', coordinates);
            return res.json(coordinates);
        } catch (error) {
            console.error('Error reading coordinates:', error);
        }
        console.log(inpFileName);
    }
    // // Files moved and processed successfully
    // return res.json({
    //     status: 'success',
    //     message: 'Files uploaded and processed successfully.',
    // });
});

/*
function setCurrentTime()
{
        // create a new `Date` object
        const now = new Date();
        var month = (now.getMonth() + 1);
        var date = now.getDate();
        var hour = now.getHours();
        var minute = now.getMinutes();
        var second = now.getSeconds();
        if (month < 10) month = '0' + month;
        if (date < 10) date = '0' + date;
        if (hour < 10) hour = '0' + hour;
        if (minute < 10) minute = '0' + minute;
        if (second < 10) second = '0' + second;
    
        return `${now.getFullYear()}${month}${date}${hour}${minute}${second}`;
}
*/
module.exports = router;