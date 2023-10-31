#include <Wire.h>
#include "Adafruit_MPR121.h"

Adafruit_MPR121 cap_sense = Adafruit_MPR121();

#define MPR121_ADDR 0x5A
#define PRINT_DELAY 200

uint32_t sensedPoints[6][6];
volatile bool readFlag = false;

// Rows are pin 0-5, cols are pins 6-11
/*

Pin 6 | 7 | 8 | 9 | 10 | 11
0 |   |   |   |   |    |
------|---|---|---|----|---
1 |   |   |   |   |    |
------|---|---|---|----|---
2 |   |   |   |   |    |
------|---|---|---|----|---
3 |   |   |   |   |    |
------|---|---|---|----|---
4 |   |   |   |   |    |
------|---|---|---|----|---
5 |   |   |   |   |    |
------|---|---|---|----|---

*/

void setup()
{
    Serial.begin(9600);
    while(!Serial);
    if(!cap_sense.begin(MPR121_ADDR))
    {
        Serial.println("Error setting up MPR121");
        while(1);
    }
    attachInterrupt(digitalPinToInterrupt(2), updateSensedFlagSet, FALLING);
    Serial.println("Ready to sense");
}

void loop()
{
    /*if(readFlag)
    {
        readFlag = false;
        updateSensed();
    }*/
    updateSensed();
    printTable();
    delay(PRINT_DELAY);
}

void printTable()
{
    Serial.println("TABLE");
    for(int i=0; i<6; i++)
    {
        for(int j=0; j<6; j++)
        {
            if(j != 5) Serial.print(String(sensedPoints[i][j]) + " | ");
            else Serial.println(String(sensedPoints[i][j]));
        }
        //if(i == 5) Serial.println("\n-------------------\n");
    }
    
}

void updateSensedFlagSet()
{
    readFlag = true;
}

void updateSensed()
{
    //Serial.println("Reading");
    for(uint8_t i=0; i<6; i++)
    {
        for(uint8_t j=0; j<6; j++) sensedPoints[i][j] = cap_sense.filteredData(i) * cap_sense.filteredData(j + 6);
    }
    //Serial.println("Table updated");
}
