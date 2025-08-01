// client_side/web/js/map_marker_results.js

let markers;
let infowindow;

function map_marker_results(node_info, node_head) {
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
    node_info.forEach((info, index) => {
      const marker = new google.maps.Marker ({
      position: {lat: info[1], lng: info[2]},
      map,
      icon: {
          path: google.maps.SymbolPath.CIRCLE,
          scale: 5,
          strokeWeight: 0, // Set strokeWeight to 0 to remove the stroke
          fillColor: "#3C3C3C", // Set the fill color here
          fillOpacity: 1 // Set fillOpacity to 1 for full opacity
      }
    });

    infowindow = new google.maps.InfoWindow();

    bounds.extend(new google.maps.LatLng(info[1], info[2])); // Extend bounds with each coordinate


    // Add event listener for click event to show the title in the info window
    marker.addListener('click', function() {
      // Create a div element for the info window content
      const content = document.createElement('div');
      
      // Define the translation amount
      const xOffset = 85;
      const yOffset = 30.4;

      // Create an SVG element for the line chart
      const svg = d3.select(content).append('svg')
          .attr('width', 500)
          .attr('height', 300); // Increased height to accommodate the title

      // Append a group element and apply the translation
      const g = svg.append('g')
          .attr('transform', `translate(${xOffset}, ${yOffset})`); // Translate horizontally
      
      // Add D3.js code to create the line chart here
      const data = node_head[index].map((value, index) => ({ x: (index), y: value }));
      console.log(data);
      const margin = { top: 30, right: 20, bottom: 30, left: 85 };
      const width = 500 - margin.left - margin.right;
      const height = 300 - margin.top - margin.bottom - 10;
      const x = d3.scaleLinear().domain([0, data.length - 1]).range([0, width]);
      const y = d3.scaleLinear().domain([d3.min(data, d => d.y), d3.max(data, d => d.y)]).range([height, 0]);
      console.log(d3.min(data, d => d.y));
      const line = d3.line().x(d => x(d.x)).y(d => y(d.y));
      const xAxis = d3.axisBottom(x);
      const yAxis = d3.axisLeft(y);
      svg.append('g')
         .attr('transform', `translate(${margin.left},${height + margin.top})`)
         .call(xAxis)
         .selectAll('text')
         .style('font-size', '14px'); // Set font size for x-axis labels

      svg.append('g').attr('transform', `translate(${margin.left},${margin.top})`)
         .call(yAxis)
         .selectAll('text')
         .style('font-size', '14px'); // Set font size for x-axis labels
      // Append x-axis label
      svg.append('text')
        .attr('class', 'x-axis-label')
        .attr('x', width / 2 + xOffset)
        .attr('y', height + margin.top + margin.bottom + 10) // Adjust y-coordinate for positioning
        .attr('text-anchor', 'middle')
        .style('font-size', '14px')
        .text('Time (hr)');

      // Append y-axis label
      svg.append('text')
        .attr('class', 'y-axis-label')
        .attr('transform', 'rotate(-90)')
        .attr('x', -height / 2 - 25)
        .attr('y', 28) // Adjust y-coordinate for positioning
        .attr('text-anchor', 'middle')
        .style('font-size', '14px')
        .text('Head (m)');
      // Append chart title with font size
      svg.append('text')
         .attr('class', 'chart-title')
         .attr('x', width / 2 + xOffset)
         .attr('y', margin.top / 2)
         .attr('text-anchor', 'middle')
         .style('font-size', '18px') // Set font size for chart title
         .style('font-weight', 'bold') // Set font size for chart title
         .text(`${'Hydrograph of node ' + info[0]}`);
      
      g.append('path')
       .datum(data)
       .attr('fill', 'none')
       .attr('stroke', 'steelblue')
       .attr('stroke-width', 2.5)
       .attr('d', line);

      // Append circles for data points
      g.selectAll('circle')
       .data(data)
       .enter()
       .append('circle')
       .attr('cx', d => x(d.x)) // Set x-coordinate of the circle
       .attr('cy', d => y(d.y)) // Set y-coordinate of the circle
       .attr('r', 4) // Set radius of the circle
       .attr('fill', 'steelblue'); // Set fill color of the circle
        
      // Set content of the info window to the SVG element
      infowindow.setContent(content);
      // Open the info window at the marker's position
      infowindow.open(map, marker);

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

window.map_marker_results = map_marker_results;