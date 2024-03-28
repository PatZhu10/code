//  server_side/routes/runGA.js

const { Router } = require('express');
const router = Router();
const { exec } = require('child_process');
const fileUpload = require('express-fileupload');
const path = require('path');

router.use(fileUpload());

router.post('/', (req, res) => {
    const build = 'cd CSHPOM/ga;make;cd ..;\
                   cd ./swmm522_engine/build;\
                   cmake ..;cmake --build .;\
                   cd ../../;\
                   g++ -I. cshpom.cpp -o cshpom -L./ga -L./swmm522_engine/build -lga -lswmm5;cd ..;\
                   export LD_LIBRARY_PATH=CSHPOM/ga:CSHPOM/swmm522_engine/build;';
    const executablePath = 'CSHPOM/cshpom -o';
    const WLS_File = req.files && req.files.myFiles[0]; // Assuming you want the first file
    const inputFile = req.files && req.files.myFiles[1];  // Assuming you want the second file
    const outputFile = req.files && req.files.myFiles[2]; // Assuming you want the third file
    const reportFile = req.files && req.files.myFiles[3]; // Assuming you want the fourth file

    if (!inputFile || !reportFile || !outputFile || !WLS_File) {
        return res.status(400).json({ error: 'Missing required file paths.' });
    }

    const inputFilePath = path.resolve('../user_data', inputFile.name);
    const reportFilePath = path.resolve('../user_data', reportFile.name);
    const outputFilePath = path.resolve('../user_data', outputFile.name);
    const WLS_FilePath = path.resolve('../user_data', WLS_File.name);

    const command = `${build} ${executablePath} ${inputFilePath} ${reportFilePath} ${outputFilePath} ${WLS_FilePath}`;

    const childProcess = exec(command, (error, stdout, stderr) => {
        if (error) {
            console.error(`Error executing the C++ program: ${error.message}`);
            return res.status(500).json({ error: 'Error running the simulation.', details: error.message });
        }

        console.log(`C++ program output:\n${stdout}`);

        if (!stdout.includes('error')) {
            const data = { status: 'success', message: 'Simulation completed successfully.' };

            console.log('Data received:', data);
            res.json(data);
        } else {
            res.status(500).json({ error: 'Simulation failed.', details: 'Output file not found or simulation unsuccessful.' });
        }
    });

    // Clean the buffer by consuming the data and handling the 'end' event
    childProcess.stdout.on('data', (data) => {
        // Process the data or simply ignore it
    });

    childProcess.stdout.on('end', () => {
        console.log('stdout stream ended. Buffer cleaned.');
    });

    childProcess.stderr.on('data', (data) => {
        // Process the data or simply ignore it
    });

    childProcess.stderr.on('end', () => {
        console.log('stderr stream ended. Buffer cleaned.');
    });
    
});

module.exports = router;