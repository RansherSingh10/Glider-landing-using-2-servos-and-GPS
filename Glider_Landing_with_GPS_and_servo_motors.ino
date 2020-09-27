#include <math.h>
#include <Servo.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 5, TXPin = 3;
static const uint32_t GPSBaud = 9600;
float a=0,b=0,dist,idealdist=0;
float slope_flight,slope_ideal;
int angle;
int angle2;
float h=0,n,d,an=0,van,sum=0,pdfc,cdfc;
float cx,cy,px,py;
int i=1,j;
float finalx=12.82072;                                         //final coordinate of x
float finaly=80.03927;                                         //final coordinate of y
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
Servo myservo;
Servo myservo2;


void setup() 
{
   Serial.begin(9600);
   ss.begin(GPSBaud);
   myservo2.attach(10);                                         //servo1
   myservo.attach(9);                                           //servo2
   myservo.write(90);                                           //default servo1 angle
   myservo2.write(90);                                          //default servo2 angle
   Serial.println("Setup Complete");
   angle=90;
   angle2=90;
}

void loop() 
{
 if((ss.available()<=0))                                        //if the GPS does not connect to the sattelites
 {
  myservo.write(angle);
  myservo2.write(ang);
 }
 else if ((ss.available()>0) )                                  //if the GPS is connected   
  {   
   gps.encode(ss.read());
   myservo.write(angle);
   myservo2.write(angle2);
   if(gps.location.isUpdated())                                 //if gps location updated
    {  
      
     if(i==1)                                                   //for first run of loop
      {
      a=gps.location.lat();                                     //current longitude
      b=gps.location.lng();                                     //current latitude
      cx=a;                                                     
      cy=b;                                                     
     cy-=finaly;
     cx-=finalx;
      i=2;
      }
     a= gps.location.lat();                                      // get latitude into a    
     b= gps.location.lng();                                      // get longitude into b
     px=cx;
     py=cy;
     cx=a-finalx;
     cy=b-finaly;
     dist=pow((cx-px)*100000,2)+pow((cy-py)*100000,2);      
        Serial.print("\ndistance travelled :");
        Serial.print(dist);
       {
       if((dist>idealdist))
        {
          idealdist=dist;
          Serial.print("\n ideal distance updated\n");
          Serial.print(idealdist);
        }       
       else if((idealdist>=(dist*4))
        {
          angle2 = 0;                                           // this value depends on your servo motor
          Serial.print("\nelevate");
        }
       else
        {
          angle2 = 50;                                          // this value depends on your servo motor
          Serial.print("\nno need\n"); 
        }
       }
        myservo2.write(angle2);
       
       slope_ideal=((py)*100000)/((px)*100000);
       pdfc=sqrt(pow((px)*100000,2)+pow((py)*100000,2));
       cdfc=sqrt(pow((cx)*100000,2)+pow((cy)*100000,2));
       if(cx==px and cy==py)
        slope_flight=slope_ideal;
       else if(cx==px)
        slope_flight=1000;
       else
        slope_flight=((cy-py)*100000)/((cx-px)*100000);

       if((slope_flight==slope_ideal))
        {
          Serial.print("\nsame line \n");
          angle=0;                                           // this value depends on your servo motor   
        }
        else if((slope_flight>slope_ideal))      //if slope of path> ideal path slope then rotate anticlockwise
        {
         Serial.print("\nrotate clockwise\n");
         angle=180;                                         // this value depends on your servo motor
        }
        else                                                                //else rotate clockwise
        {
          Serial.print("\nrotate anti-clockwise\n");
          angle=50;
        } 
       myservo.write(angle);
       Serial.print("\ncdfc:  ");
       Serial.println(cdfc);
       
       
       Serial.print("currentx: ");
       Serial.println(cx,5);
       Serial.print("curenty:  ");
       Serial.println(cy,5);
       Serial.print("previousx:  ");
       Serial.println(px,5);
       Serial.print("previousy:  ");
       Serial.println(py,5);      
    delay(500);                                           // this is optional
   }
   else
   {
    myservo.write(angle);
    myservo2.write(angle2);
   }
  }
}  
