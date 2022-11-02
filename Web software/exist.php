<?php
    
      
    $db = mysqli_connect("localhost",  "id17130795_byker",  "Danielkomolafe00%",  "id17130795_energymeter");
  
    if (isset($_POST['meter_no'])) {
  	 
  	$meter_no  =  $_POST['meter_no'];

  	$sql = "SELECT * FROM meter WHERE meterid='$meter_no'";
  	$res = mysqli_query($db, $sql);

  	if (mysqli_num_rows($res) > 0) {
  	my_custom_redirect();
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
  
?>

  