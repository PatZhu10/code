// client_side/web/js/file_process.js

// import { map_marker } from './map_marker.js';

async function sendFiles() {
    const form = document.getElementById('uploadForm');

    const sendFiles = async () => {
        // Object
        const myFiles = document.getElementById('myFiles').files;
        const formData = new FormData();
      
        Object.keys(myFiles).forEach(key => {
            formData.append(myFiles.item(key).name, myFiles.item(key));
        });
    
        const response = await fetch('/file_process', {
            method: 'POST',
            body: formData
    
        });

        const coordinates = await response.json(); // Parse the JSON response
        console.log(coordinates);    
        map_marker(coordinates);
    }

    form.addEventListener('submit', (e) => {
        e.preventDefault();
        sendFiles();

    });
};

function updateFileCount() {
    const fileInput = document.getElementById('myFiles');
    const fileCountSpan = document.getElementById('fileCount');

    if (fileInput.files.length === 0)
    {
        fileCountSpan.textContent = 'No file chosen';
    }
    else if (fileInput.files.length === 1)
    {
        fileCountSpan.textContent = '1 file chosen';
    }
    else
    {
        fileCountSpan.textContent = fileInput.files.length + ' files chosen';
    }
}

// Export the sendFiles function so it can be accessed globally
window.sendFiles = sendFiles;
window.updateFileCount = updateFileCount;