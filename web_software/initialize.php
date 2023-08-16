<?php
$curl = curl_init();

//$email = "your@email.com";
//$amount = 30000;  //the amount in kobo. This value is actually NGN 300

$amount = $_POST["amount"];
$meter_no = $_POST["meter_no"];
$amount =$amount *100;

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
$status = $output['status'];
$email = $output['Email'];
mysqli_close($con);

if($status == 'sucess' || $status == ''){
if($amount > 500){
session_start();
$_SESSION ["meter_no"] =  $meter_no;
$_SESSION ["amount"] =  $amount ;

 // url to go to after payment
$callback_url = 'https://paygotesting.000webhostapp.com/callback.php';

curl_setopt_array($curl, array(
  CURLOPT_URL => "https://api.paystack.co/transaction/initialize ",
  CURLOPT_RETURNTRANSFER => true,
  CURLOPT_CUSTOMREQUEST => "POST",
  CURLOPT_POSTFIELDS => json_encode([
    'amount'=>$amount,
    'email'=>$email,

    'callback_url' => $callback_url
  ]),
  CURLOPT_HTTPHEADER => [
    "authorization: Bearer sk_test_5a679ebeb2769a355091ac05f843d7187b0e7b0e", //replace this with your own test key
    "content-type: application/json",
    "cache-control: no-cache"
  ],
));

$response = curl_exec($curl);
$err = curl_error($curl);

if($err){
  // there was an error contacting the Paystack API
  die('Curl returned error: ' . $err);
}

$tranx = json_decode($response, true);

if(!$tranx['status']){
  // there was an error from the API
  print_r('API returned error: ' . $tranx['message']);
  //echo $amount;
}

// comment out this line if you want to redirect the user to the payment page
//print_r($tranx);
// redirect to page so User can pay
// uncomment this line to allow the user redirect to the payment page
header('Location: ' . $tranx['data'] ['authorization_url']);
}
if($amount < 500){
echo "Topup too low, minimum is 500naira" ;
}
}
if($status == 'pending'){
//echo "Sorry, last Topup is waiting to be confirmed by Meter, kindly use STS-Token generation to Topup manually and retry new Top-up payment.";
header("Location: stspending.html");
exit();

}
?>
