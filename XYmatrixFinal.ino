#include             <FastLED.h>
#define LED_PIN      D5

#define SD1 A0
//#define SD2 SDD2

#define COLOR_ORDER  GRB
#define CHIPSET      WS2812
#define BRIGHTNESS   60
const uint8_t        kMatrixWidth = 32;
const uint8_t        kMatrixHeight = 8;
const bool           kMatrixSerpentineLayout = true;
const bool           kMatrixVertical = true;
#define NUM_LEDS     (kMatrixWidth * kMatrixHeight)
CRGB                 leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB*                const leds( leds_plus_safety_pixel + 1);

uint16_t XY( uint8_t x, uint8_t y)

{

  uint16_t i;
  if( kMatrixSerpentineLayout == false) {
    if (kMatrixVertical == false) {
      i = (y * kMatrixWidth) + x;
    } else {
      i = kMatrixHeight * (kMatrixWidth - (x+1))+y;
    }
  }
  if( kMatrixSerpentineLayout == true) {
    if (kMatrixVertical == false) {
      if( y & 0x01) {
        uint8_t reverseX = (kMatrixWidth - 1) - x;
        i = (y * kMatrixWidth) + reverseX;
      } else {
        i = (y * kMatrixWidth) + x;
      }
    } else {
      if ( x & 0x01) {
        i = 255 - ( kMatrixHeight * (kMatrixWidth - (x+1))+y );
      } else {
        i = 255 - ( kMatrixHeight * (kMatrixWidth - x) - (y+1) );
      }
    }
  }
  return i;

}
uint16_t XYsafe( uint8_t x, uint8_t y) 

{

  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int countX = 0 ; int countY = 0 ; int countRowX = 31 ; //A0
int ms3 ; int ms2 ; int ms ; 
int hue ; int theta ; int r;
int timer ; int autoFlag;
String str; String s1; String s2;
float addHue = 1; float hue2; 
float addTheta = 1; float theta2;
char *rFlag = "clockwise";
int letters[64]; int ll;

int animationNumber = 7 ;
int indexstart = 7;
int textdelay = 200;
int br = 100;

int angle[256] =
{
  282724, 283570, 284534, 285642, 286927, 288434, 290224, 292380, 295016, 298300, 302471, 307874, 315000, 324462, 336801, 351869, 8130,   23198,  35537,  45000,  52125,  57528,  61699,  64983,  67619,  69775,  71565,  73072,  74357,  75465,  76429,  77275, 
  279162, 279782, 280491, 281309, 282264, 283392, 284743, 286389, 288434, 291037, 294443, 299054, 305537, 315000, 329036, 348690, 11309,  30963,  45000,  54462,  60945,  65556,  68962,  71565,  73610,  75256,  76607,  77735,  78690,  79508,  80217,  80837, 
  275527, 275906, 276340, 276842, 277431, 278130, 278972, 280007, 281309, 282994, 285255, 288434, 293198, 300963, 315000, 341565, 18434,  45000,  59036,  66801,  71565,  74744,  77005,  78690,  79992,  81027,  81869,  82568,  83157,  83659,  84093,  84472, 
  271847, 271974, 272121, 272290, 272489, 272726, 273012, 273366, 273814, 274398, 275194, 276340, 278130, 281309, 288434, 315000, 45000,  71565,  78690,  81869,  83659,  84805,  85601,  86185,  86633,  86987,  87273,  87510,  87709,  87878,  88025,  88152, 
  268152, 268025, 267878, 267709, 267510, 267273, 266987, 266633, 266185, 265601, 264805, 263659, 261869, 258690, 251565, 225000, 135000, 108434, 101309, 98130,  96340,  95194,  94398,  93814,  93366,  93012,  92726,  92489,  92290,  92121,  91974,  91847, 
  264472, 264093, 263659, 263157, 262568, 261869, 261027, 259992, 258690, 257005, 254744, 251565, 246801, 239036, 225000, 198434, 161565, 135000, 120963, 113198, 108434, 105255, 102994, 101309, 100007, 98972,  98130,  97431,  96842,  96340,  95906,  95527, 
  260837, 260217, 259508, 258690, 257735, 256607, 255256, 253610, 251565, 248962, 245556, 240945, 234462, 225000, 210963, 191309, 168690, 149036, 135000, 125537, 119054, 114443, 111037, 108434, 106389, 104743, 103392, 102264, 101309, 100491, 99782,  99162, 
  257275, 256429, 255465, 254357, 253072, 251565, 249775, 247619, 244983, 241699, 237528, 232125, 225000, 215537, 203198, 188130, 171869, 156801, 144462, 135000, 127874, 122471, 118300, 115016, 112380, 110224, 108434, 106927, 105642, 104534, 103570, 102724, 
};

const uint64_t alphabet[] = 
{
  0x6666667e66663c00,
  0x3e66663e66663e00,
  0x3c66060606663c00,
  0x3e66666666663e00,
  0x7e06063e06067e00,
  0x0606063e06067e00,
  0x3c66760606663c00,
  0x6666667e66666600,
  0x3c18181818183c00,
  0x1c36363030307800,
  0x66361e0e1e366600,
  0x7e06060606060600,
  0xc6c6c6d6feeec600,
  0xc6c6e6f6decec600,
  0x3c66666666663c00,
  0x06063e6666663e00,
  0x603c766666663c00,
  0x66361e3e66663e00,
  0x3c66603c06663c00,
  0x18181818185a7e00,
  0x7c66666666666600,
  0x183c666666666600,
  0xc6eefed6c6c6c600,
  0xc6c66c386cc6c600,
  0x1818183c66666600,
  0x7e060c1830607e00,
  0x0000000000000000,
  0x7c667c603c000000,
  0x3e66663e06060600,
  0x3c6606663c000000,
  0x7c66667c60606000,
  0x3c067e663c000000,
  0x0c0c3e0c0c6c3800,
  0x3c607c66667c0000,
  0x6666663e06060600,
  0x3c18181800180000,
  0x1c36363030003000,
  0x66361e3666060600,
  0x1818181818181800,
  0xd6d6feeec6000000,
  0x6666667e3e000000,
  0x3c6666663c000000,
  0x06063e66663e0000,
  0xf0b03c36363c0000,
  0x060666663e000000,
  0x3e403c027c000000,
  0x1818187e18180000,
  0x7c66666666000000,
  0x183c666600000000,
  0x7cd6d6d6c6000000,
  0x663c183c66000000,
  0x3c607c6666000000,
  0x3c0c18303c000000,
  0x3c66666e76663c00,
  0x7e1818181c181800,
  0x7e060c3060663c00,
  0x3c66603860663c00,
  0x30307e3234383000,
  0x3c6660603e067e00,
  0x3c66663e06663c00,
  0x1818183030667e00,
  0x3c66663c66663c00,
  0x3c66607c66663c00
};

void setup()

{
  FastLED.addLeds<CHIPSET,LED_PIN,COLOR_ORDER>(leds,NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
}

void loop()

{
  
  if(digitalRead(A0,HIGH))
  {
    Serial.println("SD1");
  }
  


  listen();
  updateAnimation();
  FastLED.show();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void updateAnimation()

{
  
  if(animationNumber == 0 )
  {
    if(countX==countRowX)
    {
      countX=0;
      countRowX-=1;
    }
    else
    {
      countX+=1;
    }
    if(countRowX==0)
    {
      countY+=1;
      countRowX=31;
    }
    if(countY==8)
    {
      countY=0;
    }
  }
  
  if( autoFlag == 1 & millis() - ms3 > 4000 )
  {

    ms3 = millis();
    if ( animationNumber == 9 )
    {
      animationNumber = 0;
    }
    else
    {
      animationNumber += 1;
    }

  }

  if( millis() - ms > 10 )
  {

    ms = millis();
    drawAnimation();

    if( hue2 >= 255)
    {
      hue2 = 0;
    }
    else
    {
      hue2 += addHue;
      hue = int( hue2 );
    }

    if( timer == 300 )
    {
      timer=0;
      if( rFlag == "clockwise" )
      {
        rFlag = "antiClockwise";
        r = -1;
      }
      else
      {
        rFlag = "clockwise";
        r = 1;
      }
    }
    else
    {
      timer += 1;
    }

    if( theta2 > 360000 )
    {
      theta2 = 0;
    }
    else if( theta2 < 0 )
    {
      theta2 = 360000;
    }
    else if( ( theta2 > 65000 & theta2 < 115000 ) | ( theta2 > 255000 & theta2 < 295000 ) )
    {
      theta2 += 200 * addTheta * r;
    }
    else
    {
      theta2 += 1000 * addTheta * r;
    }
    theta = int( theta2 );
  }

  if( millis() - ms2 > textdelay )
  {
    ms2 = millis();
    if( indexstart == 0 )
    {
      indexstart = 7;
      int le = letters[0];
      
      for( int i = 0 ; i < ll - 1 ; i++ )
      {
        letters[ i ] = letters[ i + 1 ];
      }
      letters[ ll - 1 ] = le;
    }
    else
    {
      indexstart -= 1;
    }
  }

}

void drawAnimation()

{
  
  for( uint8_t y = 0 ; y < kMatrixHeight ; y++ )  //8
  {  

    for( uint8_t x = 0 ; x < kMatrixWidth ; x++ ) //32
    { 

      float xa = x - 15.5 ; 
      float ya = y - 3.5 ;
      float d = sqrt( xa * xa + ya * ya );

      int ang = angle[ 32 * y + x ];
      int arms[] = { ang%90000 , theta%90000 , ang%45000 , theta%45000 , ang , theta };
 
      switch(animationNumber)
      {
        
        case 0:
        {

          draw ( 0 , ( ( x == countX | x > countRowX ) & y == countY ) | y < countY , 180 , x , y , hue );
          break;

        }
        case 1:
        {

          draw ( 0 , sin16( -d * 4 * 4096 + hue * 256 ) > 0.2 , 180 , x , y , hue );
          break;

        }
        case 2:
        {

          int newInt = map(d,0,17,200,35) + 255 * sin16 ( ( timer + 1 ) * 256 ) / 32768;
          draw ( 1 , 1 , newInt , x , y , hue );
          break;

        }
        case 3:
        {

          int newInt = map ( d , 0 , 17 , 200 , 35 ) + 255 * sin16 ( ( ( hue + 1 ) % 128 ) * 256 ) / 32768;
          draw ( 1 , 1 , newInt , x , y , hue );
          break;

        }
        case 4:
        {

          int newInt = map ( d , 0 , 17 , 200 , 35 ) + 255 * abs ( sin16 ( ( hue + 1 ) * 512 ) ) / 32768;
          draw ( 0 , ang > theta , newInt , x , y , hue );
          break;

        }
        case 5:
        {

          drawArms ( arms[4] , arms[5] , 4000 , x , y , hue );
          break;

        }
        case 6:
        {

          drawArms ( arms[2] , arms[3] , 3000 , x , y , hue );
          break;

        }
        case 7:
        {

          drawArms ( arms[0] , arms[1] , 4000 , x , y, hue );
          break;

        }
        case 8:
        {

          drawLetter( - indexstart , 0 , 8 , x , y , 0 , hue );
          drawLetter( 0 , 8 , 0 , x , y , 1 , hue );
          drawLetter( 8 , 16 , - 8 , x , y , 2 , hue );
          drawLetter( 16 , 24 , -16 , x , y , 3 , hue );
          drawLetter( 24 , 32 , - 24 , x , y , 4 , hue );
          break;

        }

      }

    }
  }
}

void listen()
{
  if( Serial.available() > 0 )
    {

      str = Serial.readString();
      int comma = str.indexOf(";");

      if (comma == -1 )
      {
        if( str == "disauto\n" )
        {
          autoFlag = 0;
        }
        else if( str ==  "enauto\n" )
        {
          autoFlag = 1;
        }
        else
        {
          animationNumber = str.toInt();
        }
      }
      else
      {
        
        s1 = str.substring( 0 , comma );
        s2 =  str.substring( comma + 1, str.length() );

        if( s1 == "hue" )
        {
          addHue = float(s2.toInt())/10;
        }
        if( s1 == "theta" )
        {
          addTheta = float(s2.toInt())/10;
        }
        if( s1 == "delay" )
        {
          textdelay = s2.toInt();
        }
        if( s1 == "br" )
        {
          br = s2.toInt();
        }
        if( s1 == "text" )
        {
          ll = s2.length();
          
          for( int i = 0; i < ll - 1 ; i ++ )
          {
            if( s2[i] >= 65 & s2 [i] <= 90 )
            {
              letters[i] = s2[i] - 65;
            }
            if( s2[i] >= 97 & s2[i] <= 122 )
            {
              letters[i] = s2[i] - 70;
            }
            else if( s2[i] >= 48 & s2[i] <= 57 )
            {
              letters[i] = s2[i] + 5;
            }
            else if( s2[i] == 32 )
            {
              letters[i] = s2[i] - 6;
            }
          }

          letters[ ll - 1 ] = 26;
          
          if( ll < 5 )
          {
            if( ll == 1 ){letters[1] = 26;letters[2] = 26;letters[3] = 26;letters[4] = 26;letters[5] = 26;ll = 4;}
            if( ll == 2 ){letters[2] = 26;letters[3] = 26;letters[4] = 26;letters[5] = 26;ll = 5;}
            if( ll == 4 ){letters[3] = 26;letters[4] = 26;letters[5] = 26;letters[6] = 26;ll = 7;}
          }

        }
      }

    }
}

void draw(int global, bool condition, int brg , int x, int y, int hue)
{
  if( condition )
  {
    leds[ XY ( x , y ) ] = CHSV ( hue , 200 , brg );
  }
  else if ( global == 0 )
  {
    leds[ XY ( x , y ) ] = CHSV ( 0 , 0 , 0 );
  }  
}

void drawArms(int val, int val2, int shift, int x, int y, int hue )
{

  float xa = x - 15.5 ; 
  float ya = y - 3.5 ;
  float d = sqrt( xa * xa + ya * ya );

  if ( ( val > val2 & val < val2 + shift ) ) 
  {
  leds[ XY ( x , y ) ] = CHSV ( hue , 200 , map( d, 0, 17, 200, 35 ) + 255 * abs( sin16( ( hue + 1) * 512 ) ) / 32768 );
  } 
  else 
  {
    leds[ XY ( x , y ) ] = CHSV( 0 , 0 , 0 );
  }

}

void drawLetter(int start, int end, int shift, int x, int y , int place, int hue)
{

  if ( x >= indexstart + start & x < indexstart + end ) 
  {
    if (y < 4) 
    {
      if ( alphabet[ letters[ place ] ] & (1 << ( x + 8 * y + shift - indexstart ) ) ) 
      {
        leds[ XY ( x , y ) ] = CHSV( hue , 200 , br );
      } 
      else 
      {
        leds[ XY ( x , y ) ] = CHSV( 0 , 0 , 0 );
      }
    } 
    else 
    {
      if ( alphabet[ letters[ place ] ] >> 32 & ( 1 << ( x + 8 * y + shift - indexstart ) ) ) 
      {
        leds[ XY ( x , y ) ] = CHSV( hue , 200 , br );
      } 
      else 
      {
        leds[ XY ( x , y ) ] = CHSV( 0 , 0 , 0 );
      }
    }
    if(x<=8)
    {
      leds[ XY ( indexstart -1 , 3 ) ] = CHSV( 0, 0 , 0 );
      if(indexstart==1)
      {
        leds[ XY ( 8 , 3 ) ] = CHSV( 0, 0 , 0 );
      }
      if(indexstart==0)
      {
        leds[ XY ( 7 , 3 ) ] = CHSV( 0, 0 , 0 );
      }
    }
    else if(x>8)
    {
      leds[ XY ( indexstart + start + 7 , 3 ) ] = CHSV( 0, 0 , 0 );
    }
  }  

}

