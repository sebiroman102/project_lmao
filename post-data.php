<?php
$servername = "localhost";
$dbname = "id20898035_database_sensor";
$username = "id20898035_admin";
$password = "h3awJ41xJ_9kG?b";

$api_key_value = "#54321";
$api_key = "";

if ($_SERVER["REQUEST_METHOD"] == "GET") {
    $api_key = test_input($_GET["api_key"]);
    if ($api_key == $api_key_value) {

        $conn = new mysqli($servername, $username, $password, $dbname);
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        }

        // Insert data into H_T_Sensor table
        if (isset($_GET["humidity"]) && isset($_GET["temperature"])) {
            $htSql = "INSERT INTO H_T_Sensor (Humidity, Temperature)
            VALUES ('" . test_input($_GET["humidity"]) . "', '" . test_input($_GET["temperature"]) . "')";
            if ($conn->query($htSql) === TRUE) {
                echo "H_T_Sensor: New record created successfully<br>";
            } else {
                echo "H_T_Sensor: Error: " . $htSql . "<br>" . $conn->error;
            }
        }

        // Insert data into Light_Sensor table
        if (isset($_GET["light"]) && isset($_GET["light_sensor_1"]) && isset($_GET["light_sensor_2"]) && isset($_GET["light_sensor_3"]) && isset($_GET["light_sensor_4"])) {
            $lightSql = "INSERT INTO Light_Sensor (Light, Light_sensor_1, Light_sensor_2, Light_sensor_3, Light_sensor_4)
            VALUES ('" . test_input($_GET["light"]) . "', '" . test_input($_GET["light_sensor_1"]) . "', '" . test_input($_GET["light_sensor_2"]) . "', '" . test_input($_GET["light_sensor_3"]) . "', '" . test_input($_GET["light_sensor_4"]) . "')";
            if ($conn->query($lightSql) === TRUE) {
                echo "Light_Sensor: New record created successfully<br>";
            } else {
                echo "Light_Sensor: Error: " . $lightSql . "<br>" . $conn->error;
            }
        }

        // Insert data into TrafficLights table
        if (isset($_GET["lane1_red"]) && isset($_GET["lane2_red"]) && isset($_GET["lane1_green"]) && isset($_GET["lane2_green"])) {
            $trafficSql = "INSERT INTO TrafficLights (Lane1_red, Lane2_red, Lane1_green, Lane2_green)
            VALUES ('" . test_input($_GET["lane1_red"]) . "', '" . test_input($_GET["lane2_red"]) . "', '" . test_input($_GET["lane1_green"]) . "', '" . test_input($_GET["lane2_green"]) . "')";
            if ($conn->query($trafficSql) === TRUE) {
                echo "TrafficLights: New record created successfully<br>";
            } else {
                echo "TrafficLights: Error: " . $trafficSql . "<br>" . $conn->error;
            }
        }

        // Insert data into Pollution_sensor table
        if (isset($_GET["pollution"])) {
            $pollutionSql = "INSERT INTO Pollution_sensor (Pollution)
            VALUES ('" . test_input($_GET["pollution"]) . "')";
            if ($conn->query($pollutionSql) === TRUE) {
                echo "Pollution_sensor: New record created successfully<br>";
            } else {
                echo "Pollution_sensor: Error: " . $pollutionSql . "<br>" . $conn->error;
            }
        }

        $conn->close();
    } else {
        echo "Wrong API Key";
    }
} else {
    echo "No data received. HTTP GET method expected.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>
