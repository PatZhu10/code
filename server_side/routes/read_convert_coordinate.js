//  server_side/routes/read_convert_coordinate.js  (only funciton of read file and convert coordinate)

const fs = require('fs');
const readline = require('readline');
const proj4 = require('proj4');

const twd67_tm2 = '+proj=tmerc +towgs84=-752,-358,-179,-0.0000011698,0.0000018398,0.0000009822,0.00002329 +lat_0=0 +lon_0=121 +x_0=250000 +y_0=0 +k=0.9999 +ellps=aust_SA +units=m +no_defs';
const wgs84 = '+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs';

function read_convert_coordinate(filepath) {
    return new Promise((resolve, reject) => {
        const original_coordinates = [];
        const converted_coordinates = [];
        let section = false;
        const fileStream = fs.createReadStream(`${filepath}`);
        const rl = readline.createInterface({ input: fileStream });
                
        rl.on('line', (line) => {
            if (line == '[COORDINATES]'){
                section = true;
                return;
            }
            if (section){
                if (line[0] == ';' || line[0] == '[') return;
                else if (line.length < 2){
                    section = false;
                }
                else {
                    // If line is not empty and does not start with ';' or '['
                    const [node, x, y] = line.trim().split(/\s+/);
                    original_coordinates.push({node, x, y});
                }
            }

        });
        
        rl.on('close', () => {
            original_coordinates.forEach(coordinates => {
                const x = parseFloat(coordinates.x);
                const y = parseFloat(coordinates.y);
                if (!isNaN(x) && !isNaN(y)) {
                    const temp_converted_coordinates = proj4(twd67_tm2, wgs84, [x, y]).map(coord => parseFloat(coord.toFixed(6)));;
                    converted_coordinates.push([coordinates.node, temp_converted_coordinates[1], temp_converted_coordinates[0]]);
                }
            });
            resolve(converted_coordinates);
        });

        rl.on('error', (error) => {
            reject(error);
        });



    });
}

module.exports = read_convert_coordinate;