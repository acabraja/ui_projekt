<?php
set_time_limit(0);
$address = '127.0.0.1';
$port = '5000';

$fp = fsockopen($address, $port, $errno, $errstr, 300);
if(! $fp)
{
  echo "$errstr ($errno)\n";
}
else
{	
	var_dump($fp);
  // Send 10 test message to the server
  for($i=0; $i<= 10; $i++)
  {
   // Send message to server
   $out = "Test #$i\r\n";
   fwrite($fp, $out);
  
   // Read the response from the server
   $str = fread($fp, 100000);
   echo $str;
   
	 sleep(5);
  }
	
	fclose($fp);
}
?>
