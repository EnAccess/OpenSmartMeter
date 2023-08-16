  <?php

    $tarrif = "70";
    $api_key_value = "tPmAT5Ab3j7F9";
    $api_key = $meter =$unit  =$stat = "";
    $connection_status = "";



if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);

    if($api_key == $api_key_value) {
    $meter = test_input($_POST["meter"]);
    $servername = "localhost";
    $dbname = "id17130795_energymeter";
    $username = "id17130795_byker";
    $password = "Danielkomolafe00%";

    $con=mysqli_connect($servername, $username,  $password, $dbname) or die("Database Error");
    if (!$con) {
    echo "Unable to connect to DB: " . mysql_error();
    exit;
    }

    $sql = "SELECT * from meter WHERE meterid='$meter'";
    $result = mysqli_query( $con ,$sql);
    $output= (mysqli_fetch_assoc($result));
    $real = $output['amount'];
    $unit = ($real/ $tarrif) ;
    echo $real."/".$tarrif."/";
    $connection_status = "sucess";
    mysqli_close($con);


    if($connection_status == 'sucess'){
    $servername = "localhost";
    $dbname = "id17130795_energymeter";
    $username = "id17130795_byker";
    $password = "Danielkomolafe00%";
    $meter = test_input($_POST["meter"]);

    $conn = mysqli_connect ($servername, $username, $password, $dbname);
    //Check connection
    if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
    }
    $t= time();
    $sql = "UPDATE meter SET  conn_status=$t  WHERE meterid='$meter'";
    if ($conn->query($sql) === TRUE) {
    //echo "sucess";
    }
    else {
    echo "Error " . $conn->error;
    }
    $conn->close();
    $connection_status = '';
    }


    $meter = test_input($_POST["meter"]);
    $stat = test_input($_POST["stat"]);
    if($stat == 'sucess'){

    $servername = "localhost";
    $dbname = "id17130795_energymeter";
    $username = "id17130795_byker";
    $password = "Danielkomolafe00%";

    $conn = mysqli_connect ($servername, $username, $password, $dbname);
    //Check connection
    if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
    }
    $sql = "UPDATE meter SET amount='0', status='sucess', transaction_id='0' WHERE meterid='$meter'";
    if ($conn->query($sql) === TRUE) {
    echo "sucess";
    }
    else {
    echo "Error " . $conn->error;
    }
    $conn->close();
    }
    }
}


 else{
     echo "wrong api key";
 }


function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}


?>
