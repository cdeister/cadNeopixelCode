#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

#define PIN 6 // This pin varies by board. 

char serialHeads[] = {'i', 'r', 'g', 'b', 'o', 'p', 'q', 'd', 'm'};
int serialVars[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
// MAP: brightness, rVal,gVal,bVal,on/run trial,p=column,q=row,trialLength/delay,d multiplier
int serialCount = 9;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 4, PIN,
                            NEO_MATRIX_TOP  + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
                            NEO_GRB            + NEO_KHZ800);


void setup() {
  matrix.begin();
  Serial.begin(9600);
}


// Matrix Vars.
int rows = matrix.height();
int columns = matrix.width();
int cRow;
int cCol;

// Color Vars.
int rVal;
int gVal;
int bVal;

// State Vars.
int currentOn;
int lastOn = 0;
bool runTrial = 0;

void loop() {
  flagReceive(serialHeads, serialVars);
  // look for a state change and call for trial
  rVal = serialVars[1];
  gVal = serialVars[2];
  bVal = serialVars[3];

  currentOn = serialVars[4];
  if (currentOn - lastOn != 0) {
    runTrial = 1;
  }
  lastOn = currentOn;

  if (runTrial == 1) {
    matrix.setBrightness(serialVars[0]);

    // *** Turn All Pixels One Color
    if (serialVars[5] == -1) {
      for (cRow = 0; cRow < rows; cRow++) {
        for (cCol = 0; cCol < columns; cCol++) {
          matrix.drawPixel(cCol, cRow, matrix.Color(rVal, gVal, bVal));
        }
      }
      pulseTrial();
    }

    // *** Address One Pixel
    else if (serialVars[5] != -1) {
      for (cRow = 0; cRow < rows; cRow++) {
        for (cCol = 0; cCol < columns; cCol++) {
          matrix.drawPixel(cRow, cCol, matrix.Color(0, 0, 0));
        }
      }
      matrix.show();
      cRow = serialVars[5];
      cCol = serialVars[6];

      matrix.drawPixel(cRow, cCol, matrix.Color(rVal, gVal, bVal));
    }
    pulseTrial();
  }
}





void blankMatrix() {
  for (cRow = 0; cRow < rows; cRow++) {
    for (cCol = 0; cCol < columns; cCol++) {
      matrix.drawPixel(cCol, cRow, matrix.Color(0, 0, 0));
    }
  }
}


bool flagReceive(char varAr[], int valAr[]) {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char endMarker = '>';
  char rc;
  int nVal;
  const byte numChars = 32;
  char writeChar[numChars];
  bool newData = 0;
  int selectedVar = 0;

  while (Serial.available() > 0 && newData == 0) {
    rc = Serial.read();
    if (recvInProgress == false) {
      for ( int i = 0; i < serialCount; i++) {
        if (rc == varAr[i]) {
          selectedVar = i;
          recvInProgress = true;
        }
      }
    }

    else if (recvInProgress == true) {
      if (rc != endMarker) {
        writeChar[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }

      else if (rc == endMarker ) {
        writeChar[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = 1;

        nVal = int(String(writeChar).toInt());
        valAr[selectedVar] = nVal;

      }
    }
  }
  return newData; // tells us if a valid variable arrived.
}


void pulseTrial() {
  matrix.show();
  delay(serialVars[7]*serialVars[8]);
  blankMatrix();
  matrix.show();
  resetVars();
}

void resetVars() {
  for ( int i = 0; i < serialCount; i++) {
    serialVars[i] = 0;
  }
  currentOn = 0;
  lastOn = 0;
  runTrial = 0;
}

