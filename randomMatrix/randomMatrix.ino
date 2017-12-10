#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <adafruit_feather.h>
#include "adafruit_spiflash.h"

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define PIN PC7 // This pin varies by board. 


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 4, 3, 1, PIN,
                            NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
                            NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
                            NEO_GRB + NEO_KHZ800);

bool justSet = 0;


void setup() {
  matrix.begin();
}

int x = matrix.width();  //8
int y = matrix.height(); //4
int pass = 0;
int i = 0;
int rDelta = 2;
int gDelta = 10;
int bDelta = 3;
int rVal;
int gVal;
int bVal;
int tCol = (0, 0, 0);
int setType = 2; // 0 for pixels, 1 for rows, 2 for columns


void loop() {
  if (i > 255) {
    randomSeed(analogRead(0));
    i = 0;
  }


  matrix.setBrightness(100);
  if (justSet == 0) {
    // pixels
    if (setType == 0) {
      for (int k = 0; k < y ; k++) {
        for (int j = 0; j < x ; j++) {
          setColors();
          matrix.drawPixel(j, k, matrix.Color(rVal, gVal, bVal));
        }
      }
    }

    // rows
    else if (setType == 1) {
      for (int k = 0; k < y ; k++) {
        setColors();
        for (int j = 0; j < x ; j++) {
          matrix.drawPixel(j, k, matrix.Color(rVal, gVal, bVal));
        }
      }
    }



    // columns
    else if (setType == 2) {
      for (int j = 0; j < x ; j++) {
        setColors();
        for (int k = 0; k < y ; k++) {
          matrix.drawPixel(j, k, matrix.Color(rVal, gVal, bVal));
        }
      }
    }

  }

  matrix.show();
  justSet = 1;


  delay(100);
  justSet = 0;
  i++;
}


void setColors() {
  //randomSeed(analogRead(0));
  rVal = random(20, 31); //(rDelta*i);
  gVal = random(8, 19); //(gDelta*i);
  bVal = random(21, 32); //(bDelta*i);
}

