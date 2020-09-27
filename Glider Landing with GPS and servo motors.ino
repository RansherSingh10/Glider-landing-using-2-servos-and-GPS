#include <math.h>
#include <Servo.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
 START
currentx: 12.82088
curenty:  80.03920
*/
/*
 END
currentx: 12.82060
curenty:  80.03919

*/
static const int RXPin = 5, TXPin = 3;
static const uint32_t GPSBaud = 9600;
float a=0,b=0,dist,idealdist=0;
float slope_flight,slope_ideal;
int angle;
int ang;
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
   myservo2.attach(10);                                         //tail servo
   myservo.attach(9);
   myservo.write(90);
   myservo2.write(90);
   Serial.println("Setup Complete");
   angle=90;
   ang=90;
}

void loop() 
{
 if((ss.available()<=0))
 {
  myservo.write(angle);
  myservo2.write(ang);
 }
 else if ((ss.available()>0) )                                     
  {   
   gps.encode(ss.read());
   myservo.write(angle);
   myservo2.write(ang);
   if(gps.location.isUpdated())                                  //if gps location updated
    {  
      
     if(i==1)                                                    //for first run of loop
      {
      a=gps.location.lat();
      b=gps.location.lng();
      Serial.print("fuck");
      cx=a;
      cy=b;
     cy-=finaly;
     cx-=finalx;
      i=2;
      }
     a= gps.location.lat();                                      // get latitude into a    
     b= gps.location.lng();
     px=cx;
     py=cy;
     cx=a-finalx;
     cy=b-finaly;
     dist=pow((cx-px)*100000,2)+pow((cy-py)*100000,2);      
        Serial.print("\ndistance travelled :");
        Serial.print(dist);
       {
       if((dist>idealdist)&&0)
        {
          idealdist=dist;
          Serial.print("\n ideal distance updated\n");
          Serial.print(idealdist);
        }       
       else if((idealdist>=(dist*4))||1)
        {
          ang = 0;
          //Serial.print("\nelevate\ndistance  :  ");
          //Serial.print(dist);
          Serial.print("\nelevate");
        }
       else
        {
          //myservo2.write(150);
          //ang = 100;
          ang=50;
          Serial.print("\nno need\n"); 
        }
       }
        myservo2.write(ang);
       
       //slope_flight=((cy-py)*100000)/((cx-px)*100000);
       slope_ideal=((py)*100000)/((px)*100000);
       pdfc=sqrt(pow((px)*100000,2)+pow((py)*100000,2));
       cdfc=sqrt(pow((cx)*100000,2)+pow((cy)*100000,2));
       if(cx==px and cy==py)
        slope_flight=slope_ideal;
       else if(cx==px)
        slope_flight=1000;
       else
        slope_flight=((cy-py)*100000)/((cx-px)*100000);
       /*pdfc=pdfc/1.1132;
       pdfc=sqrt(pdfc);
       cdfc=cdfc/1.1132;
       cdfc=sqrt(cdfc);
       if(cdfc>pdfc)
        //while(1)
          {
            Serial.print("\nspiral\n");
            myservo.write(55);
          }
       */
       if((slope_flight==slope_ideal)||1)
        {
          Serial.print("\nsame line \n");
          //angle=150;
          angle=0;   
        }
        else if((slope_flight>slope_ideal))      //if slope of path> ideal path slope then rotate anticlockwise
        {
         Serial.print("\nrotate clockwise\n");
         angle=180;
         //angle=1023
        }
        else                                                                //else rotate clockwise
        {
          Serial.print("\nrotate anti-clockwise\n");
          angle=50;
        } 
       //int val = map(angle, 0, 1023, 0, 180);    
       //myservo.write(val);
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
    //delay(500);
   }
   else
   {
    myservo.write(angle);
    myservo2.write(ang);
   }
  }
}  
  
  //Serial.println("fuck this");
  // put your main code here, to run repeatedly:
