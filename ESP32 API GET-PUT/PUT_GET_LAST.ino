
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <string.h>
#include <stdio.h>
#include <SoftwareSerial.h>
#include "EasyNextionLibrary.h" 
EasyNex myNex(Serial);
SoftwareSerial myport;
//------------------------------------------------------------------------------------------------------------
const char* ssid ="CLM_2.4GHz";
const char* password="E4fVjaXy";
/*
http://91.198.66.24:3758/api/Products/Change/1          //Postman Status change
http://91.198.66.24:3758/api/Products                   //API-GET
*/
//------------------------------------------------------------------------------------------------------------
uint8_t LED1pin = D4;
uint8_t LED2pin = D5;
uint8_t LED3pin = D6;
uint8_t LED4pin = D8;
uint8_t Buton1 = D7;


boolean buton1_Durumu = LOW;
boolean buton2_Durumu = LOW;


int durum=0;
int nextion_state=0;
uint32_t SWITCH=0;
int switch_pos;
char jsonOutput[128];

WiFiClient client;
HTTPClient sender;
HTTPClient putter;

//------------------------------------------------------------------------------------------------------------

void get()
{
//--------------------------------------------------------------------

if((WiFi.status()==WL_CONNECTED))
{

sender.begin(client,"http://91.198.66.24:3758/api/Products");
sender.addHeader("Content-Type","text/plain");
        while(1)
        {
          int httpCode = sender.GET();
        if(httpCode>0)
        {
          String payload=sender.getString();
          
          String caseResult;
          //String Case=sender.getString().substring(x=>{caseResult=x.case});
        
          Serial.println("\nStatuscode: "+String(httpCode));
          Serial.println(payload);
          
          //-----------------DISPLAY----------------------
           display_Ctrl();
 

          if(payload[30]=='t')
          {
            digitalWrite(LED1pin,HIGH);
            digitalWrite(LED2pin,LOW);
          }
          else
          {
            digitalWrite(LED1pin,LOW);
            digitalWrite(LED2pin,HIGH);
          }
         
          button_Control_switch(); 
    
          //---------------------------------
        }
      
      else
        {
            Serial.println("Error on HTTP request!");
            Serial.println(httpCode);
        }
        delay(150);
    }     
}
else
{
      Serial.println("Connection LOST!");
}




//--------------------------------------------------------------------
}
//----------------------------------------------------------------------------------------------------

void  button_Control_switch()
{
       buton1_Durumu=digitalRead(Buton1);
       switch_pos=myNex.readNumber("b0.val");
       if(buton1_Durumu==HIGH && durum==0)
        {
        putter.begin(client,"http://91.198.66.24:3758/api/Products/Change/1");
        int httpCoded=putter.PUT("ESP32 PUT METHOT");
        Serial.println(httpCoded);
        Serial.println(buton1_Durumu);
        digitalWrite(LED2pin,LOW);
        digitalWrite(LED1pin,HIGH);
      
        durum!=durum;
         }
        else if(buton1_Durumu==HIGH && durum==1)
        {
        digitalWrite(LED1pin,LOW);
        digitalWrite(LED2pin,HIGH);
        
        durum!=durum;  
        }
       
}

void setup() 
{
//-----------------------------------------------------------
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(LED3pin, OUTPUT);
  pinMode(LED4pin, OUTPUT);
  pinMode(Buton1, INPUT);
  
//----------------------------------------------------------- 
      Serial.begin(115200);
      myNex.begin(9600); 
      myNex.NextionListen();
      WiFi.begin(ssid,password);
      Serial.print("Connecting to CLM WiFi...");
//-----------------------------------------------------------
    while(WiFi.status() !=WL_CONNECTED)
      {
        Serial.print(".");
        delay(500);
      }
//-----------------------------------------------------------
    Serial.println("\nConnected to the CLM WiFi network");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  
  get();
  
}

void loop() 
{
    

}
void display_Ctrl()
{
     
      String payload=sender.getString();
          String str= payload;
          String new_string="";
          char arr[payload.length()+1];
          strcpy(arr,str.c_str());
          int i;
          char*a;
        
          for ( i = 13; i < str.length()-7; i++)
          {
            Serial.print(arr[i]);
            remove_char(arr,']');
            remove_char(arr,'}');
            remove_char(arr,'"');
            remove_char(arr,'{');
            remove_char(arr,'[');
            remove_char(arr,':');
            remove_char(arr,',');
          }
          a=&(arr[i]); 
          
          new_string=a;

          myNex.readStr("t0.txt");
          myNex.writeStr("t0.txt",new_string);
         //-------------Display_button------------------------
         switch_pos=myNex.readNumber("b0.val");
        
        if(switch_pos==1 && nextion_state==0)
        {
        putter.begin(client,"http://91.198.66.24:3758/api/Products/Change/1");
        int httpCoded=putter.PUT("ESP32 PUT METHOT");
        digitalWrite(LED3pin,LOW);
        digitalWrite(LED4pin,HIGH);
        nextion_state!=nextion_state;
         }
        else if(switch_pos==1 && nextion_state==1 )
        {
        digitalWrite(LED4pin,LOW);
        digitalWrite(LED3pin,HIGH);
        nextion_state!=nextion_state;
        }
         
		     
    
}
char *remove_char(char *p, int ch)
{
	char *pw = p, *pr = p;
 
	while (*pr) {
		if (*pr != ch)
			*pw++ = *pr;
		++pr;
	}
	*pw = '\0';
	
	return p;
}