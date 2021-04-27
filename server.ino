//sending GET request to the server
//return value after find response
//serial read using start marker and end marker

//////////////////////////FUNCTION TO VERIFY CARD//////////////////////////
boolean verifyCard(String uid){
  // Create a URL for the request. Modify YOUR_HOST_DIRECTORY so that you're pointing to the PHP file.
  String url = "/ECafe/terminal_verify.php?uid=";
  url += uid;
  String r = (readUntilEndMarker(url));
  Serial.println();
  Serial.print("Response from server: ");
  Serial.println(r);
  if(r.equals("Valid")){
    return true;
  }else{
    return false;
  }
}

//////////////////////////FUNCTION TO REGISTER CARD//////////////////////////
boolean registerUID(String uid){
  // Create a URL for the request. Modify YOUR_HOST_DIRECTORY so that you're pointing to the PHP file.
  String url = "/ECafe/terminal_reg.php?uid=";
  url += uid;
  String p = (readUntilEndMarker(url));
  Serial.println();
  Serial.print("Response from server: ");
  Serial.println(p);
  if(p.equals("UID registered")){
    return true;
  }else{
    return false;  //*
  }
}

//////////////////////////FUNCTION TO PERFORM PAYMENT//////////////////////////
int processPayment(String uid, String buy, String pin){
  // Create a URL for the request. Modify YOUR_HOST_DIRECTORY so that you're pointing to the PHP file.
  String url = "/ECafe/terminal_pay.php?uid=";
  url += uid;
  url += "&pin=";
  url += pin;
  url += "&buy=";
  url += buy;
  String q = (readUntilEndMarker(url));
  Serial.println();
  Serial.print("Response from server: ");
  Serial.println(q);
  if(q.equals("PAID")){
    return 1;
  }else if(q.equals("Not enough balance")){
    return 2;
  }else if(q.equals("Invalid PIN!")){
    return 3;
  }else{
    return 4;
  }
}

////////////////////////////////READ FROM START MARKER UNTIL END MARKER///////////////////////////////////
String readUntilEndMarker(String url){
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;
  String res = "";
  
  // This will send the request to the server
  Serial.print("Requesting URL: ");
  Serial.println(url);
  Serial.println("Please wait...");
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
     
  unsigned long timeout = millis();
  
  while (client.available() == 0) {
    if (millis() - timeout > 10000) {  //if after 10 sec client still not available
      Serial.println(">>> Client Timeout !");
      client.stop();
      break;
    }
    yield();
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available() > 0 && newData == false){
    rc = client.read();
    if(recvInProgress == true){
      if(rc!=endMarker){
        receivedChars[ndx] = rc;
        Serial.print(receivedChars);
        ndx++;
        if(ndx >= numChars){
          ndx = numChars - 1;
        }
      }else{
        receivedChars[ndx] = '\0';
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if(rc == startMarker){
      recvInProgress = true;
    }
    yield();
  }
  client.stop();

  if(newData == true){
    res = String(receivedChars);
    newData = false;
  }
  return res;
}
