//////////////////////////CONNECT TO WIFI NETWORK//////////////////////////
void connectToWiFi(){
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    yield();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  delay(1000);
}

//////////////////////////CONNECT TO SERVER//////////////////////////
void connectToHost(){
  // Connect to host
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(host);
  const int httpPort = 80;
  while (!client.connect(host, httpPort)) {
    Serial.print(".");
    yield();
  }
  Serial.println("Connected to host");
}
