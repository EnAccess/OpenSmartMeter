<?php

    function recheckDB() {
    $meter =  ($_POST["meter_no"]);
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
    $status = $output['conn_status'];
    if($status == '' ){
    echo $meter." has never established a connection" ;
    }

    if($status != ""){
    date_default_timezone_set("Africa/Lagos");
    $currentDate = date('Y-m-d-(h:i:sa)', $status);
    echo $meter." was last active". " " .$currentDate;
    }

    mysqli_close($con);
    }

   ?>
  <script type="text/JavaScript">
   setTimeout(function(){
   alert("<?php   recheckDB(); ?>")
   },0);
   </script>
