 <?php
    

$servername = "localhost";

// REPLACE with your Database name
$dbname = "id17130795_byker";
// REPLACE with Database user
$username = "id17130795_energymeter";
// REPLACE with Database user password
$password = "Danielkomolafe00%";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. If you change this value, the ESP32 sketch needs to match
$api_key_value = "tPmAT5Ab3j7F9";

$api_key = $value1   = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
    //$value1 = test_input($_POST["value1"]);
    //echo $value1."received" ;
    $meter = test_input($_POST["meter"]);
    DB_connect();
    
    }   
    }
    else {
    echo "Wrong API Key provided.";
    }


function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}


function DB_connect() {
     
}