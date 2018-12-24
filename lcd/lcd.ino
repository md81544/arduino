/*
   LCD Test

   Demonstrates the use of a 16x2 LCD display.  The LiquidCrystal
   library works with all LCD displays that are compatible with the
   Hitachi HD44780 driver. There are many of them out there, and you
   can usually tell them by the 16-pin interface.

   The circuit:
   (LCD pins in brackets counting from left if they're on top)
    * LCD RS pin (4) to digital pin 12
    * LCD Enable pin (6) to digital pin 11
    * LCD D4 pin (11) to digital pin 5
    * LCD D5 pin (12) to digital pin 4
    * LCD D6 pin (13) to digital pin 3
    * LCD D7 pin (14) to digital pin 2
    * LCD R/W pin (5) to ground
    * 10K pot: ends to +5V and ground
    * wiper to LCD VO pin (pin 3)
    * 5V to pin 2 and pin 15 (backlight)
    * GND to pin 1 and pin 16 (backlight)

    I was worried about whether the LCD backlight (pins 15 & 16)
    needed a current limiting resistor, but 330R left the display very
    dim.  So I tried without and it hasn't blown up yet.

    http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

#include <LiquidCrystal.h>

extern "C"
{
    #include <string.h> // needed for strlen()
}

byte cursor[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111
    };

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd( 12, 11, 5, 4, 3, 2 );

void teletype(const char* sMsg)
{
    lcd.clear();
    char szCurrentLine[17];
    const char *pos = sMsg;
    boolean bDone=false;

    while( pos-sMsg < static_cast<int>( strlen( sMsg ) ) )
    {
        int linePos = 0;
        int i;
        for( i = 0; i < 16; ++i )
        {
            char c = *( pos + i );
            if( c == '\0' )
            {
                bDone = true;
                break;
            }
            if( c == ' ' && linePos == 0)
            {
                continue; // no space on beginning of line
            }
            if( c == ' ' )
            {
                // new word coming up - will it fit on the current line?
                char *p = strchr(pos + i + 1, ' ' );
                if( !p )
                {
                    // no space found, search for end of string instead
                    p = strchr( pos+i+1, '\0' );
                }
                if( p )
                {
                    int wordLen = p - ( pos + linePos + 1 );
                    if( linePos + wordLen > 15 )
                    {
                        break; // i.e. new line
                    }
                }
            }
            szCurrentLine[ linePos ] = c;
            ++linePos;
            szCurrentLine[ linePos ] = '\0';
            lcd.setCursor( 0, 1 );
            lcd.print( szCurrentLine );
            lcd.write( static_cast<uint8_t>( 0 ) ); // faux cursor
            delay( 64 );
        }
        if(! bDone ){
            lcd.clear();
            lcd.print( szCurrentLine );
        }
        pos += i;
    }
}

void setup() {
    lcd.createChar( 0, cursor ); // create the custom char (we can have 8)
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
}


void loop() {
    teletype( "Roses = #F00    "
              "Violets = #00F  "
              "All of my base  "
              "Are belong to you"
              );
    delay(2000);
}

