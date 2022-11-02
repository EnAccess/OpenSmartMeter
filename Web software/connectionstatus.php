<?php

    $api_key_value = "tPmAT5Ab3j7F9";
    $api_key = $meter =$unit  =$stat = "";
    $postedmeterno = "";
   
   
   header("refresh: 20");
   echo " Establishing connecion with meter, response will be available in 20sec ";
   echo "<br>";
   echo " RESPONSE : "; 
   //echo date('H:i:s Y-m-d');

   if ($_SERVER["REQUEST_METHOD"] == "POST") {
   $api_key = test_input($_POST["api_key"]);
    
   if($api_key == $api_key_value) {
   $meter = test_input($_POST["meter"]);
   if($meter = $postedmeterno){
   echo $postedmeterno." is active and online";   
   }
   }
}
    
    
else{
   echo "unfortunately,".$postedmeterno." did not respond"; 
}
  
   
 


function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
 

?>