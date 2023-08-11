
#include "WROVER_KIT_LCD.h" // Hardware-specific library
#include <SD_MMC.h>

WROVER_KIT_LCD tft;

extern const uint8_t font8x8_ib8x8u[224][8];
uint16_t *bfr,*bfp;
uint16_t temp[240];

//
// drawchar x range 0 - 39 y range 0 - 24

void drawchar(int ch,int x,int y) {

  y+=2;
  for (int ix=0;ix<8;ix++)
    for (int iy=0,rtbit=0x80;iy<8;iy++,rtbit>>=1)
      if (font8x8_ib8x8u[ch][ix] & rtbit) {
        //canvas.drawPixel(x*8+iy, ix+y*8, WROVER_WHITE);
        bfr[239-(y*8+ix)+240*(x*8+iy)]=WROVER_WHITE;
        tft.drawPixel(239-(y*8+ix), x*8+iy, WROVER_WHITE);
      }
}

void drawstring(int x,int y,char *str) {
  while (*str) 
    drawchar(*str++,x++,y);
}

void vscroll() {

  for (int i=0;i<320;i++) {
    bfp=bfr+(i*240);
      memmove(bfp+8,bfp,216*2);
  }
  tft.drawBitmap(0, 0, 240, 320, (const uint16_t *)bfr);
}

void setup(void) {

  char bstr[128];

  Serial.begin(115200);

  tft.begin();
  //tft.setRotation(1);
  tft.fillScreen(WROVER_BLUE);
  bfr = (uint16_t *)heap_caps_malloc(320*240*2, MALLOC_CAP_SPIRAM);
    if (!bfr) {
        Serial.printf("Cannot allocate memory Core 1\r\n");
        while (1);
    }
  memset(bfr,0,320*240*2);
  memset(temp,0,sizeof(temp));
  Serial.print("Initializing SD card...");
  if (!SD_MMC.begin()) {
    Serial.println("failed!");
    return;
  }
  
  Serial.println("OK!");

  for (int line=0;line<25;line++){
    for (int i=0x21+line;i<0x21+40+line;i++) {
      drawchar(i,i-0x21-line,line);
    }
  }

  for (int i=0;i<20;i++) {
  vscroll();
  //delay(100);
  sprintf(bstr,"Test line:%d",i+1);
  drawstring(0,24,bstr);
  //tft.drawBitmap(0, 0, 240, 320, (const uint16_t *)bfr);
  }
}

void loop() {}
