#include <LiquidCrystal.h>
#include <string.h>
LiquidCrystal lcd(12,11,5,4,3,2);
const int TPadL=6,TPadR=7,RstBotton=8;
void setup() {
 pinMode(TPadL,INPUT);
 pinMode(TPadR,INPUT);
 pinMode(RstBotton,INPUT);
 pinMode(13,OUTPUT);
 lcd.begin(16,2);
 lcd.setCursor(0,0);
 lcd.print("  NeTimer Lite");
 lcd.setCursor(0,1);
 lcd.print("           0.000");
 Serial.begin(9600);
}

void loop() {
  long MsRec,Hs,Mins,Ss,Ms;
  String MsNom,MsSpec,SsNom,SsSpec,MinsNom,MinsSpec,AnsTime;
  bool LEDLeave,flag;
  if (digitalRead(RstBotton))
    {
      digitalWrite(13,HIGH);
       for (;;)
       {
        if (!digitalRead(RstBotton))
          {
            digitalWrite(13,LOW);
            Hs=0; Mins=0; Ss=0; Ms=0;
            lcd.setCursor(0,1);
            lcd.print("           0.000");
            break;
          }
       }
    }
   if (digitalRead(TPadL)&&digitalRead(TPadR))
     {
        MsRec=millis();
        for (;;)
          {
            if (digitalRead(TPadL)&&digitalRead(TPadR)&&(millis()-MsRec)>=300)
              {
                digitalWrite(13,HIGH);
                LEDLeave=true;
                Hs=0; Mins=0; Ss=0; Ms=0;
                lcd.setCursor(0,1);
                lcd.print("           0.000");
                for (;;)
                {
                  flag=false;
                  if (!digitalRead(TPadL)||!digitalRead(TPadR))
                   {
                      flag=true;
                      MsRec=millis();
                      for(;;)
                      {
                        if ((millis()-MsRec)%100==0)
                          {
                            LEDLeave=!LEDLeave;
                            digitalWrite(13,LEDLeave);
                          }
                        lcd.setCursor(0,1);
                        Hs=(millis()-MsRec)/3600000;
                        Mins=(millis()-MsRec)%3600000/60000;
                        Ss=(millis()-MsRec)%60000/1000;
                        Ms=(millis()-MsRec)%1000;
                        MsSpec=(String) Ms;
                        MsNom=(Ms>=100?MsSpec:(Ms>=10?"0"+MsSpec:"00"+MsSpec));
                        SsSpec=(String) Ss+"."+MsNom;
                        SsNom=(Ss>=10?SsSpec:"0"+SsSpec);
                        MinsSpec=(String) Mins+":"+SsNom;
                        MinsNom=(Mins>=10?MinsSpec:"0"+MinsSpec);
                        MinsSpec=(Mins>0?(String) Mins+":"+SsNom:SsSpec);
                        AnsTime=(Hs>0?(String) Hs+":"+MinsNom:MinsSpec);
                        lcd.setCursor(16-AnsTime.length(),1);
                        lcd.print(AnsTime);
                       // lcd.setCursor(0,0);
                       // lcd.print(MsRec);
                       // lcd.setCursor(8,0);
                       // lcd.print(millis());
                       if (digitalRead(RstBotton))
                         {
                           Serial.println(AnsTime);
                           digitalWrite(13,HIGH);
                           for (;;)
                             {
                               if (!digitalRead(RstBotton))
                                 {
                                   digitalWrite(13,LOW);
                                   Hs=0; Mins=0; Ss=0; Ms=0;
                                   lcd.setCursor(0,1);
                                   lcd.print("           0.000");
                                   break;
                                 }
                             }
                            break;   
                         }
                        if (digitalRead(TPadL)&&digitalRead(TPadR))
                          {
                            digitalWrite(13,LOW);
                            Serial.println(AnsTime);
                            break;
                          }
                      }    
                   }
                 if (flag) break;
                }
              MsRec=millis();
             }
            if (!digitalRead(TPadL)||!digitalRead(TPadR)&&(millis()-MsRec)<300)
              break;
          }
     }  
}