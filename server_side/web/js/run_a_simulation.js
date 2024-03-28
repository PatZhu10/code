//  server_side/html/js/run_a_simulation.js

function run_a_simulation() {
    const form = document.getElementById('uploadForm');
    const formData = new FormData(form); // Use the form element to create FormData

    fetch('/run_a_simulation', {
        method: 'POST',
        body: formData,
    })
    .then(response => {
        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }
        return response.json();
    })
    .then(data => {
        console.log('Data received:', data);
        alert(data.message);

        // Check if the simulation completed successfully and navigate to the results page
        if (data.status === 'success') {
            console.log('Simulation completed successfully. Navigating to results page.');
            window.location.href = '/runSWMM_results';
        } else {
            console.log('Simulation failed or encountered an error.');
        }
    })
    .catch(error => {
        console.error('Fetch error:', error);
        alert('Error running the simulation. Please try again.');
    });
}
