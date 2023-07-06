<!DOCTYPE html>
<html>
<head>
    <title>Sensor Dashboard</title>
    <link rel="stylesheet" href="styles.css">
</head>
<body>
    <div class="dashboard">
        <div class="sidebar">
            <!-- Navigation tabs -->
            <ul>
                <li><a href="#" onclick="loadChart('humidity')">Humidity & Temperature</a></li>
                <li><a href="#" onclick="loadChart('lights')">Light Sensor & Light Sources</a></li>
                <li><a href="#" onclick="loadChart('traffic-lights')">Traffic Lights & Timers</a></li>
                <li><a href="#" onclick="loadChart('pollution')">Pollution Levels</a></li>
            </ul>
        </div>
        <div class="content">
            <!-- Content for each tab -->
            <div id="humidity" class="chart-container">
                <h2>Humidity & Temperature</h2>
                <!-- Chart for humidity and temperature data -->
                <canvas id="humidity-chart" width="400" height="200"></canvas>
            </div>
            <div id="lights" class="chart-container">
                <h2>Light Sensor & Light Sources</h2>
                <!-- Chart for light sensor data -->
                <canvas id="light-chart" width="400" height="200"></canvas>
                <!-- Light source switches -->
                <div class="light-sources">
                    <div class="switch-container">
                        <label class="switch">
                            <input type="checkbox" id="light-source-1" disabled> <!-- Add "disabled" attribute -->
                            <span class="slider round"></span>
                        </label>
                        <span>Light Source 1</span>
                    </div>
                    <div class="switch-container">
                        <label class="switch">
                            <input type="checkbox" id="light-source-2" disabled> <!-- Add "disabled" attribute -->
                            <span class="slider round"></span>
                        </label>
                        <span>Light Source 2</span>
                    </div>
                    <div class="switch-container">
                        <label class="switch">
                            <input type="checkbox" id="light-source-3" disabled> <!-- Add "disabled" attribute -->
                            <span class="slider round"></span>
                        </label>
                        <span>Light Source 3</span>
                    </div>
                    <div class="switch-container">
                        <label class="switch">
                            <input type="checkbox" id="light-source-4" disabled> <!-- Add "disabled" attribute -->
                            <span class="slider round"></span>
                        </label>
                        <span>Light Source 4</span>
                    </div>
                </div>
            </div>
            <div id="traffic-lights" class="chart-container">
                <h2>Traffic Lights & Timers</h2>
                <!-- Chart for traffic light timers -->
                <canvas id="traffic-light-timers-chart" width="400" height="200"></canvas>
            </div>
            <div id="pollution" class="chart-container">
                <h2>Pollution Levels</h2>
                <!-- Chart for pollution levels -->
                <canvas id="pollution-chart" width="400" height="200"></canvas>
            </div>
        </div>
    </div>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script>

        function loadChart(tabId) {
        // Remove the active class from all tabs
        const tabs = document.querySelectorAll('.sidebar li');
        tabs.forEach(tab => {
            tab.querySelector('a').classList.remove('active');
        });

        // Add the active class to the selected tab
        const selectedTab = document.querySelector(`#${tabId}`);
        selectedTab.querySelector('a').classList.add('active');
        }
    
        // Retrieve data from H_T_Sensor table
        <?php
        $servername = "localhost";
        $dbname = "id20898035_database_sensor";
        $username = "id20898035_admin";
        $password = "h3awJ41xJ_9kG?b";
         
         
        $conn = new mysqli($servername, $username, $password, $dbname);
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 

        // Retrieve latest 5 values from H_T_Sensor table
        $sql = "SELECT Temperature, Humidity, reading_time FROM H_T_Sensor ORDER BY reading_time DESC LIMIT 5";
        $result = $conn->query($sql);

        $labels = array();
        $temperatureData = array();
        $humidityData = array();

        if ($result->num_rows > 0) {
            // Output data of each row
            while($row = $result->fetch_assoc()) {
                $labels[] = $row["reading_time"];
                $temperatureData[] = $row["Temperature"];
                $humidityData[] = $row["Humidity"];
            }
        }

        // Retrieve latest 5 values from Light_Sensor table
        $lightSql = "SELECT Light, Light_sensor_1, Light_sensor_2, Light_sensor_3, Light_sensor_4, reading_time FROM Light_Sensor ORDER BY reading_time DESC LIMIT 5";
        $lightResult = $conn->query($lightSql);

        $lightLabels = array();
        $lightSensor = array();
        $lightData = array();

        if ($lightResult->num_rows > 0) {
            while ($row = $lightResult->fetch_assoc()) {
                $lightLabels[] = $row["reading_time"];
                $lightData[] = $row["Light"];
                $lightSensor[] = array(
                    $row["Light_sensor_1"],
                    $row["Light_sensor_2"],
                    $row["Light_sensor_3"],
                    $row["Light_sensor_4"]
                );
            }
        }

        // Retrieve latest 5 values from TrafficLights table
        $trafficLightsSql = "SELECT Lane1_red, Lane2_red, Lane1_green, Lane2_green, reading_time FROM TrafficLights ORDER BY reading_time DESC LIMIT 5";
        $trafficLightsResult = $conn->query($trafficLightsSql);

        $trafficLightsLabels = array();
        $trafficLightsData = array();

        if ($trafficLightsResult->num_rows > 0) {
            while ($row = $trafficLightsResult->fetch_assoc()) {
                $trafficLightsLabels[] = $row["reading_time"];
                $trafficLightsData[] = array(
                    $row["Lane1_red"],
                    $row["Lane2_red"],
                    $row["Lane1_green"],
                    $row["Lane2_green"]
                );
            }
        }

        // Retrieve latest 5 values from Pollution_sensor table
        $pollutionSql = "SELECT Pollution, reading_time FROM Pollution_sensor ORDER BY reading_time DESC LIMIT 5";
        $pollutionResult = $conn->query($pollutionSql);

        $pollutionLabels = array();
        $pollutionData = array();

        if ($pollutionResult->num_rows > 0) {
            while ($row = $pollutionResult->fetch_assoc()) {
                $pollutionLabels[] = $row["reading_time"];
                $pollutionData[] = $row["Pollution"];
            }
        }

        $conn->close();

        // Reverse the order of the arrays
        $labels = array_reverse($labels);
        $temperatureData = array_reverse($temperatureData);
        $humidityData = array_reverse($humidityData);

        $lightLabels = array_reverse($lightLabels);
        $lightData = array_reverse($lightData);

        $trafficLightsLabels = array_reverse($trafficLightsLabels);
        $trafficLightsData = array_reverse($trafficLightsData);

        $pollutionLabels = array_reverse($pollutionLabels);
        $pollutionData = array_reverse($pollutionData);

        // Convert PHP arrays to JavaScript arrays
        $labelsJS = json_encode($labels);
        $temperatureDataJS = json_encode($temperatureData);
        $humidityDataJS = json_encode($humidityData);

        $lightLabelsJS = json_encode($lightLabels);
        $lightDataJS = json_encode($lightData);

        $lightSensorJS = json_encode($lightSensor);

        // Extract the first set of values from the array
        $firstValues = $lightSensor[0];

        // Encode the extracted values back into JSON
        $lightSensorJS = json_encode($firstValues);

        $trafficLightsLabelsJS = json_encode($trafficLightsLabels);
        $trafficLightsDataJS = json_encode($trafficLightsData);

        $pollutionLabelsJS = json_encode($pollutionLabels);
        $pollutionDataJS = json_encode($pollutionData);


        ?>
    
        // Declare variables for chart instances
        var humidityChart, lightChart, trafficLightTimersChart, pollutionChart;

        // Function to load the chart based on the selected tab
        function loadChart(tabId) {
            // Hide all chart containers
            var chartContainers = document.querySelectorAll('.chart-container');
            chartContainers.forEach(function (container) {
                container.style.display = 'none';
            });

            // Show the selected chart container
            var selectedContainer = document.getElementById(tabId);
            selectedContainer.style.display = 'block';

            // Update the chart data if needed
            if (tabId === 'humidity') {
                if (humidityChart) {
                    humidityChart.destroy();
                }
                humidityChart = updateHumidityChart();
            } else if (tabId === 'lights') {
                if (lightChart) {
                    lightChart.destroy();
                }
                lightChart = updateLightChart();
            } else if (tabId === 'traffic-lights') {
                if (trafficLightTimersChart) {
                    trafficLightTimersChart.destroy();
                }
                trafficLightTimersChart = updateTrafficLightsChart();
            } else if (tabId === 'pollution') {
                if (pollutionChart) {
                    pollutionChart.destroy();
                }
                pollutionChart = updatePollutionChart();
            }

            var lightSourceStates = <?php echo $lightSensorJS; ?>.map(function(value) {
            return value === "1" ? true : false;
            });

            // Update the light sources based on the received data
            lightSourceStates.forEach(function (state, index) {
                updateLightSource(index + 1, state);
            });
        }

        // Function to update the humidity chart data
        function updateHumidityChart() {
            var chart = new Chart(document.getElementById('humidity-chart'), {
                type: 'line',
                data: {
                labels: <?php echo $labelsJS; ?>,
                datasets: [
                    {
                        label: "Humidity",
                        data: <?php echo $temperatureDataJS; ?>,
                        backgroundColor: 'rgba(75, 192, 192, 0.2)',
                        borderColor: 'rgba(75, 192, 192, 1)',
                        borderWidth: 1
                    },
                    {
                        label: "Temperature",
                        data: <?php echo $humidityDataJS; ?>,
                        backgroundColor: 'rgba(255, 99, 132, 0.2)',
                        borderColor: 'rgba(255, 99, 132, 1)',
                        borderWidth: 1
                    }
                ]
            },

                options: {
                    responsive: true,
                    scales: {
                        y: {
                            beginAtZero: true
                        }
                    }
                }
            });

            return chart;
        }


// Function to update the light chart data
function updateLightChart() {
    var chart = new Chart(document.getElementById('light-chart'), {
        type: 'line',
        data: {
            labels: <?php echo $lightLabelsJS; ?>,
            datasets: [
                {
                    label: "Light Sensor",
                    data: <?php echo $lightDataJS; ?>,
                    backgroundColor: 'rgba(255, 205, 86, 0.2)',
                    borderColor: 'rgba(255, 205, 86, 1)',
                    borderWidth: 1
                }
            ]
        },
        options: {
            responsive: true,
            scales: {
                y: {
                    beginAtZero: true
                }
            }
        }
    });

    return chart;
}

        function updateTrafficLightsChart() {
          var durations = <?php echo json_encode($trafficLightsData); ?>;
        
          var chartContainer = document.getElementById('traffic-lights');
          
          // Clear the chart container
          chartContainer.innerHTML = '';
          
          // Create the chart canvas for Lane 1
          var chartCanvas1 = document.createElement('canvas');
          chartCanvas1.id = 'traffic-light-timers-chart_1';
          chartCanvas1.width = 400;
          chartCanvas1.height = 200;
          chartContainer.appendChild(chartCanvas1);
        
          // Create the chart canvas for Lane 2
          var chartCanvas2 = document.createElement('canvas');
          chartCanvas2.id = 'traffic-light-timers-chart_2';
          chartCanvas2.width = 400;
          chartCanvas2.height = 200;
          chartContainer.appendChild(chartCanvas2);
        
          var chart1 = new Chart(chartCanvas1.getContext('2d'), {
            type: 'bar',
            data: {
              labels: <?php echo json_encode($trafficLightsLabels); ?>,
              datasets: [
                {
                  label: 'Red Light Duration - Lane 1',
                  data: durations.map(data => data[0]),
                  backgroundColor: 'rgba(255, 99, 132, 0.2)',
                  borderColor: 'rgba(255, 99, 132, 1)',
                  borderWidth: 1
                },
                {
                  label: 'Green Light Duration - Lane 1',
                  data: durations.map(data => data[2]),
                  backgroundColor: 'rgba(75, 192, 192, 0.2)',
                  borderColor: 'rgba(75, 192, 192, 1)',
                  borderWidth: 1
                }
              ]
            },
            options: {
              responsive: true,
              scales: {
                y: {
                  beginAtZero: true
                }
              },
              animation: {
                onComplete: function () {
                  if (this.data.datasets[0].data.length > 5) {
                    this.data.datasets[0].data.shift();
                  }
                  if (this.data.datasets[1].data.length > 5) {
                    this.data.datasets[1].data.shift();
                  }
                  this.update();
                }
              }
            }
          });
        
          var chart2 = new Chart(chartCanvas2.getContext('2d'), {
            type: 'bar',
            data: {
              labels: <?php echo json_encode($trafficLightsLabels); ?>,
              datasets: [
                {
                  label: 'Red Light Duration - Lane 2',
                  data: durations.map(data => data[1]),
                  backgroundColor: 'rgba(255, 99, 132, 0.2)',
                  borderColor: 'rgba(255, 99, 132, 1)',
                  borderWidth: 1
                },
                {
                  label: 'Green Light Duration - Lane 2',
                  data: durations.map(data => data[3]),
                  backgroundColor: 'rgba(75, 192, 192, 0.2)',
                  borderColor: 'rgba(75, 192, 192, 1)',
                  borderWidth: 1
                }
              ]
            },
            options: {
              responsive: true,
              scales: {
                y: {
                  beginAtZero: true
                }
              },
              animation: {
                onComplete: function () {
                  if (this.data.datasets[0].data.length > 5) {
                    this.data.datasets[0].data.shift();
                  }
                  if (this.data.datasets[1].data.length > 5) {
                    this.data.datasets[1].data.shift();
                  }
                  this.update();
                }
              }
            }
          });
        
          return [chart1, chart2];
        }

        // Function to update the pollution chart data and the position of the progress bar
        function updatePollutionChart() {
            var chart = new Chart(document.getElementById('pollution-chart'), {
                type: 'line',
                data: {
                    labels: <?php echo $pollutionLabelsJS; ?>,
                    datasets: [
                        {
                            label: "Pollution Levels",
                            data: <?php echo $pollutionDataJS; ?>, // Initialize as an empty array
                            backgroundColor: 'rgba(153, 102, 255, 0.2)',
                            borderColor: 'rgba(153, 102, 255, 1)',
                            borderWidth: 1
                        }
                    ]
                },
                options: {
                    responsive: true,
                    scales: {
                        y: {
                            beginAtZero: true
                        }
                    },
                }
            });
        
            return chart;
        }

        // Function to update the state of a light source
        function updateLightSource(sourceId, isChecked) {
            var switchElement = document.getElementById('light-source-' + sourceId);
            switchElement.checked = isChecked;
        
            // Perform the necessary actions based on the state
            if (isChecked) {
                // Light source is turned on
                console.log('Light Source ' + sourceId + ' is turned on');
            } else {
                // Light source is turned off
                console.log('Light Source ' + sourceId + ' is turned off');
            }
        }
        // Load the initial chart
        loadChart('humidity');
    </script>
</body>
</html>