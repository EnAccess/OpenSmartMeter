 
 <?php
 $privatekey = 109;  // can be a number between 100 and 111
 $sts = 0;
 $status_check=0;
  $token = 70;  // unit in naira
  $db = mysqli_connect("localhost",  "id17130795_byker",  "Danielkomolafe00%",  "id17130795_energymeter");
  //$meter_no = "id17130795_energymeter";
  if (isset($_POST['check'])) {
  $meter_no = $_POST['meter_no'];
  $sql = "SELECT * FROM meter WHERE meterid='$meter_no'";
  $res = mysqli_query($db, $sql);
  if (mysqli_num_rows($res) > 0) {
  echo "found"; 	
  } 
  
  else{
  echo $meter_no.' does not exist';
  exit();
  }
  }
  
  // to generate token
  if (isset($_POST['STS'])) {
  $sts =1;
  $meter_no = $_POST['meter_no'];
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
  
  $MT = $output['meterid'];
  $topup = $output['amount'];
  $transaction = $output['transaction_id'];
  $t=time();
  $status = $output['status'];
  //echo(date("Y-m-d",$t)); 
  
  mysqli_close($con);
  }
  
  
  // to check meter status
  if (isset($_POST['status'])) {
  $status_check=1;
  $meter_no = $_POST['meter_no'];
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

  
  mysqli_close($con);
  }
  
?>



  <!DOCTYPE html>
  <html>
  <head>
  <style>
  /* Container holding the image and the text */
  .container {
  position: relative;
  text-align: center;
  //color: white;
  }

  /* Bottom left text */
  .bottom-left {
  position: absolute;
  bottom: 8px;
  left: 16px;
  }

  /* Top left text */
  .top-left {
  position: absolute;
  top: 8px;
  left: 16px;
  }

  /* Top right text */
  .top-right {
  position: absolute;
  top: 8px;
  right: 16px;
  }

  /* Bottom right text */
  .bottom-right {
  position: absolute;
  bottom: 8px;
  right: 16px;
  }

  /* Centered text */
  .centered {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  font-size: 250%;
  color: white;
  font-family: "Times New Roman", Times, serif;
  //font-family: Arial, Helvetica, sans-serif;
  }
  </style>
  </head>
  <body>
  <div class="container">
  <img src="img/sts2.jpg" alt="Snow" style="width:100%;">
  <div class="centered"> <?php 
   
  
  
  
  //sts token generation code starts here //
   if($status=='pending' && $sts ==1){
  
  $keymultiplicationfactor =(rand(1,9));
  $first = $privatekey*$keymultiplicationfactor;
  $first = $first.$keymultiplicationfactor;
  //$time_base = (date("md",$t)); 
  $time_base = (date("d",$t)); 
  
  // for 3 digit topup
  if($topup>99 && $topup<1000){
  $digit=(rand(1,3)); // random number from 1 to 3  
  $rand1=(rand(10,99));
  $indentifier='A';
 
  if($digit == 1){
  $topup = $topup - 25;
  }
  if($digit == 2){
  $topup = $topup - 50;
  }
  if($digit == 3){
  $topup = $topup - 45;
  }
  $topuplength = strlen($topup);
  if ($topuplength < 3){
  $adder =0;      
  $topup = $adder.$topup;    
  }
  $third = $digit.$topup.$rand1.$indentifier; 
  }
  
  // for 4 digit topup
  if($topup>999 && $topup<10000){
  $digit=(rand(1,3)); // random number from 1 to 3  
  $rand1=(rand(1,9));
  $indentifier='C';
   
  if($digit == 1){
  $topup = $topup - 15;
  }
  if($digit == 2){
  $topup = $topup - 36;
  }
  if($digit == 3){
  $topup = $topup - 39;
  }
  $topuplength = strlen($topup);
  if ($topuplength < 4){
  $adder =0;      
  $topup = $adder.$topup;    
  }
  $third = $digit.$topup.$rand1.$indentifier; 
  }
  
  // for 5 digit topup
  if($topup>9999 && $topup<100000){
  $digit=(rand(1,3)); // random number from 1 to 3  
  $indentifier='B';
   
  if($digit == 1){
  $topup = $topup - 99;
  }
  if($digit == 2){
  $topup = $topup - 70;
  }
  if($digit == 3){
  $topup = $topup - 60;
  }
  $topuplength = strlen($topup);
  if ($topuplength < 5){
  $adder =0;      
  $topup = $adder.$topup;    
  }
  $third = $digit.$topup.$indentifier; 
  }
   
  
  $mt_no = substr($MT,2, 10); //to extract the digit out of the meter code number
  $length = strlen($mt_no);
  if($length==1){
  $meter_no=  $mt_no*10000; 
  }
  if($length==2){
  $meter_no=  $mt_no*1000;   
  }
  if($length==3){
  //$meter_no=  $mt_no*100;   
  $meter_no=  $mt_no*100;   
  }
  if($length==4){
  $meter_no=  $mt_no*10;   
  }
  if($length==5){
  $meter_no=  $mt_no;   
  }
  $location=(rand(1,9)); // random letter a to c
  $encode= $meter_no + $token ;
  $STS_token = $first.$length.$location.$time_base.$third.$encode; 
  
  // sts token genaration code ends here //
  $sts ==0;
  
  
  
    // to clear database of genearated topup value //
    $meter_no = $_POST['meter_no'];
    $servername = "localhost";
    $dbname = "id17130795_energymeter";
    $username = "id17130795_byker";
    $password = "Danielkomolafe00%";
 
    $conn = mysqli_connect ($servername, $username, $password, $dbname);
    //Check connection
    if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
    }
    $sql = "UPDATE meter SET amount='0', status='sucess', transaction_id='0' WHERE meterid='$meter_no'";
    if ($conn->query($sql) === TRUE) {
    echo "<script>
    alert('Note: Token is generated once and automatically cleared upon manual generating') </script>";
    } 
    else {
    echo "Error " . $conn->error;
    }
    $conn->close();    
    
    
  }
  
  if($sts ==1){
  if($status=='sucess' ){
  echo "No pending topup transaction.";
  }
  if($status==''){
  echo "zero transaction done on this meter ";
  }
  if($status=='pending'){
  echo $STS_token;
  }
  else if( $MT == ""){
  echo "Sorry Meter does not exist";    
  }
  $sts ==0;
  }
  if($status_check==1){
  if($status!= ""){
  echo "Last Meter Top-up ".$status;    
  }
  if($status== ""){
  echo "Zero Transaction done";    
  }
  $status_check=0;
  }
  ?>
  </div></div>
  </body>
  </html>