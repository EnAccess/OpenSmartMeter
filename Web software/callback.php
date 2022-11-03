<?php

  //$trans_id = [];
  session_start();
  $curl = curl_init();
  $reference = isset($_GET['reference']) ? $_GET['reference'] : '';


if(!$reference){
  die('No reference supplied');
}

  curl_setopt_array($curl, array(
  CURLOPT_URL => "https://api.paystack.co/transaction/verify/" . rawurlencode($reference),
  CURLOPT_RETURNTRANSFER => true,
  CURLOPT_HTTPHEADER => [
  "accept: application/json",
  "authorization: Bearer sk_test_5a679ebeb2769a355091ac05f843d7187b0e7b0e",
  "cache-control: no-cache"
  ],
));

  $response = curl_exec($curl);
  $err = curl_error($curl);

if($err){
  // there was an error contacting the Paystack API
  die('Curl returned error: ' . $err);
}

  $tranx = json_decode($response);

if(!$tranx->status){
  // there was an error from the API
  die('API returned error: ' . $tranx->message);
}

if('success' == $tranx->data->status){
  // transaction was successful...
  // please check other things like whether you already gave value for this ref
  // if the email matches the customer who owns the product etc
  // Give value
  
  $ID = $reference;
   
  //$ID = $reference;
  //$meternumber = $_SESSION ["meter_no"];
  //$_SESSION ["amount"];
  //$temp_amount = $_SESSION ["amount"];
  //$topup = substr($temp_amount,0,-2);
  //echo $topup;
  //echo $ID;
  //echo $meternumber;
  //DB();
  //session_destroy();
  
     
   
  $meternumber = $_SESSION ["meter_no"];
  $_SESSION ["amount"];
  $temp_amount = $_SESSION ["amount"];
  $topup = substr($temp_amount,0,-2);
 
  session_destroy();   
 
  $servername = "localhost";
  $dbname = "id17130795_energymeter";
  $username = "id17130795_byker";
  $password = "Danielkomolafe00%";
 
  $conn = mysqli_connect ($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
  }

  //$sql = "INSERT INTO meter (meterid, amount, status, transaction_id)
  //VALUES ('$meternumber', '$topup', 'pending', '$ID' )";
  $sql = "UPDATE meter SET amount='$topup', status='pending', transaction_id='$ID' WHERE meterid='$meternumber'";
  
 
 if ($conn->query($sql) === TRUE) {
  echo "Record updated successfully";
} else {
  echo "Error updating record: " . $conn->error;
}
  
   

$conn->close();    
 echo "<h2>Thank you for making a purchase. Your file has been sent to your email.</h2>"; 
  
}




function DB(){
    
   
  $meternumber = $_SESSION ["meter_no"];
  $_SESSION ["amount"];
  $temp_amount = $_SESSION ["amount"];
  $topup = substr($temp_amount,0,-2);
 
  session_destroy();   
 
  $servername = "localhost";
  $dbname = "id17130795_energymeter";
  $username = "id17130795_byker";
  $password = "Danielkomolafe00%";
 
  $conn = mysqli_connect ($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
  }

  $sql = "INSERT INTO meter (meterid, amount, status, transaction_id)
  VALUES ('$meternumber', '$topup', 'pending', '$trans_id' )";

  if (mysqli_query($conn, $sql)) {
  echo "New record replaced successfully";
  }
   

$conn->close();    
}

?>