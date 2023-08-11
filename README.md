# An EGA (40x25) scrolling terminal for the ESP32 Wrover kit.

This is a very simple example app for the Arduino environment that:
1. Tests the SD Card interface.
2. Demonstrates a 40x25 line scrolling text window using the CP437 IBM Font.

Hardware changes:
  You will need to remove R167 from the board to allow the SD card and LCD to work together.

The App:
  The application displays a test pattern of text on the LCD with a 40x25 line layout (8x8 font). The key subroutine
  in the app is vscroll(). The ILI9341 LCD pixel layout is natively 240 (X) x 320 (Y) with the pixels starting
  at 0,0 at the LCD edge next to the USB socket. There are 320 rows. The ILI9341 driver chip contains a hardware
  scroller but, the scroll direction is in the Y direction such that if the display is oriented such that the Y axis
  is left to right (to use a 40x25 text layout), the scrolling direction is incorrect. To resolve this, the app
  contains a frame buffer which is written with the same data as the tft in drawchar(...). The frame bufer is
  240x320x2 bytes (150 KBytes). This is quite a bit of memory and due to the ESP32 memory mapping will only fit into PSRAM.
  This is requested in setup(). The frame buffer and screen layout are setup such that there are 2 empty lines of text
  at the top of the screen and 3 at the bottom. In order to scroll the display, the frame buffer is copied column by column
  shifted up by 8 bytes. The magic number of bytes to copy (217*2). This is the number of rows to move (25-1) plus the
  offset (3) from the bottom of the screen = (24+3)*8 = 216. As can be seen, the 3 blank lines at the bottom of the screen
  are important as a blank line will be copied into text row 25. The 2 blank lines at the top of the screen are left alone.
  It is also relevant to note that due to zero indexing, the offsets in drawchar(...) are 239 which are used to ensure that
  the screen layout is correct when the USB socket is at the top of the screen.

  I hope this example is of use.

  Ian Schofield 2023.
  
