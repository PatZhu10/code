//  client_side/html/js/initMap.js

function initMap() {
  var Latitude;
  var Longitude;

  function getLocation() {
    if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition(showPosition);
    }
  }

  function showPosition(position) {
    const user_location = {
      lat: Latitude = position.coords.latitude,
      lng: Longitude = position.coords.longitude
    }

    // Call initMap with the position
    createMap(user_location);
  }

  function createMap(user_location) {
    // Customize the map styles
    var mapStyles = [
      {
        featureType: "poi",
        elementType: "labels",
        stylers: [
          { visibility: "off" } // Hide Points of Interest labels
        ]
      }
    ];

    var map = new google.maps.Map(document.getElementById("map"), {
      zoom: 16,
      center: user_location,
      styles: mapStyles
    });
  }

  // Call getLocation to start the process
  getLocation();

}

initMap();