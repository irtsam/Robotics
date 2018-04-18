/*
 SMS receiver
 The below code uses a GSM shield, SIM card, and an Arduino microcontroller
 It receives a message from user which contains lat and longitude as well as from robots
 also via text messages which too contain lat and long of robots. It then uses thes to 
 calculate distance between user and robots using the HaverSine formulae and depending on 
 whether the robot is vacant or not send a confirmation or rejection message to user.
 
*/

// include the GSM library
#include <GSM.h>

// PIN Number for the SIM
#define PINNUMBER ""

// initialize the library instances
GSM gsmAccess;

GSM_SMS sms;
// Array to hold the number a SMS is retreived from
char senderNumber[20];
char message[35]={0};
float latt1=0; 
float lont1=0; 
float latt2=0; 
float lont2=0; 
float latu=0; 
float lonu=0; 
//char message1[30];
char lat[17];
char lon[17];
char num[12];
 
  int t1=0;
  int t2=0;
  int t3=0;

void setup() 

{
  
  int t1=0;
  int t2=0;
  int t3=0;
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("SMS Messages Receiver");
    
  // connection state
  boolean notConnected = true;
  
  // Start GSM connection
  while(notConnected)
  {
    if(gsmAccess.begin(PINNUMBER)==GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  
  Serial.println("GSM initialized");
  Serial.println("Waiting for messages");
}

void loop() 
{
  char c;
  int a=0;
  int b=0;
  int d=0;
 
   
  if (sms.available())
  {
   
   // Serial.println("Message received from:");
    
    // Get remote number
    sms.remoteNumber(senderNumber, 20);
    Serial.println(senderNumber);
    
    
    
   
 
    
    while(c=sms.read())
    {
      message[a]=c;
      a++;
      //Serial.print(c);  
  }
  delay(1000);
  
     
    Serial.println(message);
   
  for(int z=0;z<8;z++)
 {
  lat[z]=message[z];
 }
 
 delay(1000);

  //Serial.print();
 // Serial.println(lat);

float lat1=atof(lat);
Serial.println(lat1,5);
 //Serial.println(message);
for(int z=18;z<26;z++)
{
  lon[b]=message[z];
  b++;
}

  //Serial.print();
 // Serial.println(lon);



float lon1=atof(lon);
Serial.println(lon1,5);


delay(1000);

 int m=0;
int n=0;
int p=0;
//detection of cab status
  //char latt1[20],lont1[20];
  char cab1[14]="+923034093347";
  char cab2[14]="+923314355583";
  char cnfrm[]=" Your CAB has been directed to your present location";
  char busy1[]= " Sorry, No vacant CABS are available.";
  int incr=0;
//  Serial.println(senderNumber);
  for (int l=0;l<13;l++)
  {
    if (senderNumber[l]==cab1[l])
    {
    incr++;}
  }
 // Serial.println(incr);
    if(incr==13)
    {
      m=1;
      t1=1;
      latt1=lat1;
      lont1=lon1;
      Serial.println("Cab1");
      
      incr=0;
    }
   
   
    else if (m!=1){
      incr=0;
      for(int l=0;l<13;l++){
      if (senderNumber[l]==cab2[l])
      {
      incr++;
      }
    }

   // Serial.println("in cab2 else");    
 }

 //Serial.println(incr);
    if (incr==13)
    {
      t2=1;
      n=1;
     latt2=lat1;
      lont2=lon1;
      Serial.println("Cab2");
    }
     
   if(m==0 && n==0)
   { 
     t3=1;
     p=1;
     latu=lat1;
     lonu=lon1;
     Serial.println("User");
   }
   
 //  Serial.println(message);
   
   if(t1==1 && t2==0 && t3==1)
   {
     t1=0;
     t3=0;
     sms.beginSMS(cab1);
     sms.print(message);
     sms.endSMS();
     Serial.println("message sent");
     sms.beginSMS(senderNumber);
     sms.print(cnfrm);
     sms.endSMS();
      Serial.println("confirmatory message sent");
   }
   
   if(t1==0 && t2==1 && t3==1)
   {
    
     t2=0;
     t3=0;
     sms.beginSMS(cab2);
     sms.print(message);
     sms.endSMS();
     Serial.println("message sent");
     sms.beginSMS(senderNumber);
     sms.print(cnfrm);
     sms.endSMS();
      Serial.println("confirmatory message sent");
   }
   
   if(t1==0 && t2==0 && t3==1)
   {
     t3=0;
     sms.beginSMS(senderNumber);
     sms.print(busy1);
     sms.endSMS();
      Serial.println("busy message sent");
   }
   
   if(t1==1 && t2==1 && t3==1)
   {
     t3=0;
     
     Serial.println(latt1,5);
     Serial.println(lont1,5);
     Serial.println(latt2,5);
     Serial.println(lont2,5);
     Serial.println(latu,5);
     Serial.println(lonu,5);
     
    float d1=haver(latt1, lont1, latu, lonu);
     float d2=haver(latt2, lont2, latu, lonu);
     Serial.println(d1);
     Serial.println(d2);
     
     if(d1<d2)
     {
       
      t1=0;
       sms.beginSMS(cab1);
     sms.print(message);
     sms.endSMS();
     Serial.println("coordinates of transferred to CAB");
     sms.beginSMS(senderNumber);
     sms.print(cnfrm);
     sms.endSMS();
      Serial.println("confirmatory message sent to user");
      Serial.println("cab1 sent");
      latt1=0;
       lont1=0;
        latu=0;
         lonu=0;
    }
     else
     {
       
      t2=0;
       sms.beginSMS(cab2);
     sms.print(message);
     sms.endSMS();
     Serial.println("coordinates of transferred to CAB");
     sms.beginSMS(senderNumber);
     sms.print(cnfrm);
     sms.endSMS();
      Serial.println("confirmatory message sent to user");
      Serial.println("cab2 sent");
      latt2=0;
       lont2=0;
        latu=0;
         lonu=0;
    }
   }
     
   
  

   
    sms.flush();
    Serial.println("MESSAGE DELETED");
  }
  delay(1000);
}
  
  
 



float haver(float flat1, float flon1, float x2lat, float x2lon)
{
  float diflat=0, diflon=0, dist_calc=0, dist_calc2=0;
 diflat=radians(x2lat-flat1);  //notice it must be done in radians
flat1=radians(flat1);
x2lat=radians(x2lat);
diflon=radians((x2lon)-(flon1));

dist_calc = (sin(diflat/2.0)*sin(diflat/2.0));
dist_calc2= cos(flat1);
dist_calc2*=cos(x2lat);
dist_calc2*=sin(diflon/2.0);                                       
dist_calc2*=sin(diflon/2.0);
dist_calc +=dist_calc2;

dist_calc=(2*atan2(sqrt(dist_calc),sqrt(1.0-dist_calc)));

dist_calc*=6371000.0; //Converting to meters
return dist_calc;
}


