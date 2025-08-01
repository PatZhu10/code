// server_side/index.js

const express = require('express');
const fileUpload = require('express-fileupload');
const path = require('path');
const runSWMM_routes = require('./routes/runSWMM');
const runGA_routes = require('./routes/runGA');
const file_process_routes = require('./routes/file_process');
const runSWMM_results_routes = require('./routes/runSWMM_results');
const runGA_results_routes = require('./routes/runGA_results');

const app = express();
const PORT = process.env.PORT || 5500;

// Serve static files
app.use('/js', express.static(__dirname + '/web/js'));
app.use('/css', express.static(__dirname + '/web/css'));
app.use('/img', express.static(__dirname + '/web/img'));
app.use('/user_data', express.static(path.join(__dirname, '../user_data')));
app.use('/node_modules', express.static(__dirname + '/node_modules'));

// Middleware for serving static files
app.use('/file_process', file_process_routes);
app.use('/runSWMM', runSWMM_routes);
app.use('/runGA', runGA_routes);
app.use(fileUpload());
app.use('/runSWMM_results', runSWMM_results_routes);
app.use('/runGA_results', runGA_results_routes);

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
