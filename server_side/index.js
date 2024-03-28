// server_side/index.js

const express = require('express');
const fileUpload = require('express-fileupload');
const run_a_simulation_routes = require('./routes/run_a_simulation');
const runGA_routes = require('./routes/runGA');
const file_process_routes = require('./routes/file_process');
const runSWMM_results_routes = require('./routes/runSWMM_results');
const runGA_results_routes = require('./routes/runGA_results');
const path = require('path');
const bodyParser = require('body-parser');

const app = express();
const PORT = process.env.PORT || 5500;

// Serve static files
app.use('/js', express.static(__dirname + '/web/js'));
app.use('/css', express.static(__dirname + '/web/css'));
app.use('/img', express.static(__dirname + '/web/img'));

// Middleware for serving static files
app.use('/file_process', file_process_routes);
app.use('/run_a_simulation', run_a_simulation_routes);
app.use('/runGA', runGA_routes);
app.use(fileUpload());
app.use('/runSWMM_results', runSWMM_results_routes);
app.use('/runGA_results', runGA_results_routes);

// // Use body-parser middleware to parse JSON and URL-encoded bodies
// app.use(bodyParser.json());
// app.use(bodyParser.urlencoded({ extended: true }));

app.get('/', function(req, res) {
  res.sendFile(__dirname + "/web/html/home.html");
});

app.get('/runSWMM', function(req, res) {
  res.sendFile(__dirname + "/web/html/runSWMM.html");
});

app.get('/runGA', function(req, res) {
  res.sendFile(__dirname + "/web/html/runGA.html");
});

app.get('/runSWMM_results', (req, res) => {
  res.sendFile(__dirname + "/web/html/runSWMM_results.html");
});

app.get('/runGA_results', (req, res) => {
  res.sendFile(__dirname + "/web/html/runGA_results.html");
});


// Start the Express server
app.listen(PORT, () => {
  console.log(`Express server listening at http://localhost:${PORT}`);
});
