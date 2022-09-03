#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <ArduinoJson.h>
#include <string.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library

//====================================================================================
//                                    Setup
//====================================================================================

void setup(void) {
    int height = 240;
    int width = 320;
    Serial.begin(9600);
    tft.setTextColor(TFT_YELLOW);
    tft.setTextSize(2);
    tft.init();
    tft.setRotation(3);
    drawscreen();
}

//====================================================================================
//                                    Loop
//====================================================================================

void loop() {
  if (Serial.available()) 
  {
    // Allocate the JSON document
    // This one must be bigger than the sender's because it must store the strings
    StaticJsonDocument<2048> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, Serial);

    if (err == DeserializationError::Ok) 
    {
      drawscreen();
      int nb = 8;
      for(int i=1; i<nb+1; i++) {
        String index = String(i);
        const char* item = doc[index]["item"];
        int prod = doc[index]["current"];
        float effi = doc[index]["effi"];
        int C = doc[index]["prod"]["C"];
        put_item(i,item, effi, prod, C);
      }
      
    } 
    else 
    {
  
      // Flush all bytes in the "link" serial port buffer
      while (Serial.available() > 0)
        Serial.read();
    }
  }
}

void drawscreen() {
  tft.fillScreen(TFT_BLACK);
  int height = 240;
  int width = 320;
  int line_number=3;
  for(int i=1; i<line_number; i++) {
    int posx = width/line_number*i;
    tft.drawLine(posx,0,posx,height,tft.color565(169,120,0));
  }
  int space = 25;
  for(int i=1;space*i<height;i++) {
    tft.drawLine(0,i*space,width,i*space,tft.color565(169,120,0));
  }
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setTextDatum(TL_DATUM);
  tft.drawString("Produits", 7, 4);
  tft.drawString("% effi", 126, 4);
  tft.drawString("qte prod", 221, 4);
}

void put_item(int line, const char* item, float effi, int prod,int C) {
  int height = 240;
  int width = 320;
  int space = 25;
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  int posy = space*line+(space/2);
  for(int i=0; i<3;i++) {
    int posx = width/3*i+(width/3)/2;
    if (i==0) {
      if (strlen(item)>=10) {tft.setTextSize(1);}
      if (strlen(item)<=2) {item = "N/A";}
      tft.drawString(item, posx, posy);
      tft.setTextSize(2);
      }
    if (i==1) {
      tft.setTextColor(tft.color565(255*(1-(effi/100)),255*(effi/100),20));
      tft.drawNumber(effi, posx, posy);
      }
    if (i==2) {
      if (C>prod) {tft.setTextColor(tft.color565(255,20,20));}
      else {tft.setTextColor(tft.color565(100,100,255));}
      tft.drawNumber(prod, posx, posy);
      }
  }
}
