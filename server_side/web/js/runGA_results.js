//  server_side/html/js/runGA_results.js

document.addEventListener('DOMContentLoaded', async () => {
    try {
        const response = await fetch('../user_data/optimized_node_head.json');
        if (!response.ok) {
            throw new Error('Failed to fetch data');
        }
        const node_head = await response.json();
        const node_info = localStorage.getItem('coordinates');
        const parsed_node_info = JSON.parse(node_info);
        map_marker_results(parsed_node_info, node_head); // Pass the data to the map_marker_results function
    } catch (error) {
        console.error('Error fetching data:', error);
    }

});

function download_result() {
    // Get the download button by its ID
    const downloadButton = document.getElementById('download-results');
    
    // Add a click event listener to the download button
    downloadButton.addEventListener('click', () => {
        // Create a FormData object to send data with the POST request
        const formData = new FormData();
        // Add any necessary data to the FormData object (if required by the server)

        // Send a POST request to the server endpoint
        fetch('/runGA_results', {
            method: 'POST',
            body: formData // Send any necessary data with the request
        })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.blob();
        })
        .then(blob => {
            // Create a URL for the blob
            const url = window.URL.createObjectURL(blob);
            // Create an anchor element
            const a = document.createElement('a');
            // Set the href and download attributes
            a.href = url;
            a.download = 'swmm_files.zip'; // Set the filename
            // Append the anchor element to the document body
            document.body.appendChild(a);
            // Programmatically trigger a click event on the anchor element
            a.click();
            // Revoke the object URL to free up memory
            window.URL.revokeObjectURL(url);
        })
        .catch(error => {
            console.error('There was a problem with the download:', error);
        });
    });
}
