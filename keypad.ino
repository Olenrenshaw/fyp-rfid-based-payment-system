//////////////////////////FUNCTION TO GET PRICE//////////////////////////
//int getNumber(){
//  int num = 0;
//   char key = kpd.getKey();
//   while(key != '#'){
//      switch (key){
//         case NO_KEY:
//            break;
//
//         case '0': case '1': case '2': case '3': case '4':
//         case '5': case '6': case '7': case '8': case '9':
//            Serial.print(key);
//            num = num * 10 + (key - '0');
//            break;
//
//         case '*':
//            num = 0;
//            break;
//      }
//      key = kpd.getKey();
//      yield();
//   }
//   return num;
//}

String getNumber(){
  String toReturn = "";
  int b = 0;
  char num[price_length];
   char key = kpd.getKey();
   while(key != '#'){
      switch (key){
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
         case '.':
            b++;
            Serial.print(key);
            num[b] = key;
            toReturn += String(num[b]);
            break;
      }
      key = kpd.getKey();
      yield();
   }
   return toReturn;
}

//////////////////////////FUNCTION TO GET PIN//////////////////////////
int getSPin(){
  int num = 0;
   char key = kpd.getKey();
   while(key != '#'){
      switch (key){
         case NO_KEY:
            break;

         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            Serial.print("X");
            num = num * 10 + (key - '0');
            break;

         case '*':
            num = 0;
            break;
      }
      key = kpd.getKey();
      yield();
   }
   return num;
}

//////////////////////////UNUSED//////////////////////////
boolean keypadConfirm(){
  boolean error = false;
  Serial.println("1. Proceed?");
  Serial.println("2. Cancel");
  do{
    if (key){
      Serial.println(key);
      if (key == '1'){
        error = false;
        return true;
      }else if (key == '2'){
        error = false;
        return false; //not really!!
      }else{
        error = true;
        Serial.println("INVALID CHOICE!");
      }
      clearData();
    }
    yield();
  }while(error=true); 
}

//////////////////////////UNUSED//////////////////////////
void clearData()
{
  while(data_count !=0)
  {   // This can be used for any array size, 
    price[data_count--] = 0; //clear array for new data
  }
  return;
}

