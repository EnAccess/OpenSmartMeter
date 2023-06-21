<?php


    $db = mysqli_connect("localhost",  "id17130795_byker",  "Danielkomolafe00%",  "id17130795_energymeter");

    if (isset($_POST['meter_no'])) {
    $meter_no  =  $_POST['meter_no'];


    $sql = "SELECT * FROM meter WHERE meterid='$meter_no'";
    $res = mysqli_query($db, $sql);

    if (mysqli_num_rows($res) > 0) {
    if (isset($_POST['topup'])) {
    my_custom_redirect();
    }
    if (isset($_POST['history'])) {
    fetch_meterparameters();
    }
    }
    else{
    echo $meter_no. ' does not exist';
    }
    }



    function my_custom_redirect() {
    ob_start();
    header( 'Location: http://paygotesting.000webhostapp.com/topup.html');
    exit;
    }

    function fetch_meterparameters() {
    $meter_no  =  $_POST['meter_no'];
    $servername = "localhost";
    $dbname = "id17130795_energymeter";
    $username = "id17130795_byker";
    $password = "Danielkomolafe00%";

    $con=mysqli_connect($servername, $username,  $password, $dbname) or die("Database Error");
    if (!$con) {
    echo "Unable to connect to DB: " . mysql_error();
    exit;
    }

    $sql = "SELECT * from meter WHERE meterid='$meter_no'";
    $result = mysqli_query( $con ,$sql);
    $output= (mysqli_fetch_assoc($result));
    $link = $output['thingsboardlink'];
    mysqli_close($con);
    if($link != ""){
    header('Location: '.$link);
    exit;
    }
    if($link == ""){
    echo "sorry, thingsboard history page is yet to be setup.";
    }
    }

?>

