// by kikookraft with the help of:
// https://arduinojson.org/v6/example/parser/
// https://github.com/lakshanthad/Wio_Terminal_Classroom_Arduino/tree/main/Classroom%202


// Find the details about library in the readme.md file
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <ArduinoJson.h>
#include <string.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library

//====================================================================================
//                                    Setup
//====================================================================================

void setup(void) { // initiate the screen
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
      int nb = 8; // number of line used


      for(int i=1; i<nb+1; i++) { 
        //for each item: get his name, amount, ... and call the function to draw on screen
        String index = String(i);
        const char* item = doc[index]["item"]; // name of the item
        int prod = doc[index]["current"]; // total production of this item
        float effi = doc[index]["effi"]; // efficiency %age
        int C = doc[index]["prod"]["C"]; // Cosumption of this item
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

  // drawing the verticals lines of the table
  for(int i=1; i<line_number; i++) { 
    int posx = width/line_number*i;
    tft.drawLine(posx,0,posx,height,tft.color565(169,120,0));
  }
  int space = 25;

  // drawing the horizontal lines of the table
  for(int i=1;space*i<height;i++) {
    tft.drawLine(0,i*space,width,i*space,tft.color565(169,120,0));
  }

  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.setTextDatum(TL_DATUM);

  // if you change the text below, you may need to adjust the position
  tft.drawString("Products", 7, 4); // item name text
  tft.drawString("% effi", 126, 4); // efficiency text
  tft.drawString("nb prod", 221, 4); // total production text
  // example: tft.drawString("text you want", pos x, pos y);
}

void put_item(int line, const char* item, float effi, int prod,int C) {
  // put the item and its data in the screen
  int height = 240;
  int width = 320;
  int space = 25;
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  int posy = space*line+(space/2);

  // for each column, set the position 
  for(int i=0; i<3;i++) {
    int posx = width/3*i+(width/3)/2;

    if (i==0) { // setting up the item name
      if (strlen(item)>=10) {tft.setTextSize(1);} // if text is longer than 10 char, size it down
      if (strlen(item)<=2) {item = "N/A";} // if no name avaliable set text to N/A
      tft.drawString(item, posx, posy);
      tft.setTextSize(2);
    }

    if (i==1) { // put the efficiency number with a color system
      // if %age if near 0 the color will be more red
      // if %age is near 100 the color will be more green
      // example: if the number is 75, there will be 75% of green and 25% of red in the color 
      tft.setTextColor(tft.color565(255*(1-(effi/100)),255*(effi/100),20));
      tft.drawNumber(effi, posx, posy);
    }

    if (i==2) { // put the total production number
      // if this item is more consummed than created, show in red or else in blue
      if (C>prod) {tft.setTextColor(tft.color565(255,20,20));}
      else {tft.setTextColor(tft.color565(100,100,255));}
      tft.drawNumber(prod, posx, posy);
    }
  }
}
