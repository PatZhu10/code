// client_side/web/js/map_marker.js

let markers;

function map_marker(coordinates) {
    markers = [];
    let mapStyles = [
        {
          featureType: "poi",
          elementType: "labels",
          stylers: [
            { visibility: "off" } // Hide Points of Interest labels
          ]
        }
    ];
    const map = new google.maps.Map(document.getElementById("map"), {
        styles: mapStyles
    });

    let infoWindow = new google.maps.InfoWindow(); // Create a single info window
    let bounds = new google.maps.LatLngBounds(); // Initialize bounds to fit all coordinates
    
    // Add new markers based on coordinates
    coordinates.forEach(coord => {
        const marker = new google.maps.Marker ({
        position: {lat: coord[1], lng: coord[2]},
        map,
        icon: {
            path: google.maps.SymbolPath.CIRCLE,
            scale: 5,
            strokeWeight: 0, // Set strokeWeight to 0 to remove the stroke
            fillColor: "#3C3C3C", // Set the fill color here
            fillOpacity: 1 // Set fillOpacity to 1 for full opacity
        }
    });

    const infowindow = new google.maps.InfoWindow({
        content: coord[0] // Title of the marker
    });

    bounds.extend(new google.maps.LatLng(coord[1], coord[2])); // Extend bounds with each coordinate


    // Add event listener for click event to show the title in the info window
    marker.addListener('click', function() {
        infoWindow.setContent('<div style="font-size: 16px;">' + coord[0] + '</div>'); // Set content of the info window to the title of the marker
        infoWindow.open(map, marker); // Open the info window at the marker's position
    });

    markers.push(marker);

    });

    // Fit the map to the calculated bounds to show all markers
    map.fitBounds(bounds);

    // Close info window when the map is clicked
    map.addListener('click', function() {
        infoWindow.close();
    });

};

window.map_marker = map_marker;