//          USB2DB15 adapter for Supergun/Neogeo
//
//
//         Author:           Raph_friend
//         Date:             06.07.2020
//         Version:          1.0
//
//         USB Host Shield 2.0 modded lib:         https://github.com/Raphfriend/USB_Host_Shield_2.0
//
//         Compiled with Arduino 1.8.10
//
//
//                 Thanks to:
//
//              Oleg Mazurov for USB Host Shield 2.0 and libs
//              HUYE (twitter @huye_4589) for USB controller -> ATARI ver 1.21
//              たねけん (twitter @taneken2000) for kind help on HUYE code understanding
//              NearlyLogical beta testing and code improvement implementation
//              Frank_fjs for beta testing and all in one PCB design
//              fluxcore for beta testing
//              suverman for beta testing
//              akiramishima for beta testing
//              SpaceJockey for beta testing
//
//
//
//
//
//
//
//
//            メインプログラム、コントローラタイプ(TYPE_PS4, TYPE_MDmini) :
//
//            コントローラタイプ(TYPE_PSC, TYPE_RAP3, TYPE_RAP4, TYPE_SNES, TYPE_RETROFREAK) :
//
//

// Override USB Polling freqency --This needs to go before includes--
#define POLLING_OVERRIDE 1000

#include <XBOXONE.h>
//#include <XBOXUSB.h>
#include <PS3USB.h>
#include <EEPROM.h>
#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

#include "PS3Controller.h"
#include "XBoxOneController.h"
#include "USB2DB15.h"



#define TYPE_PS4 0
#define TYPE_MDmini 1
#define TYPE_PSC 2
#define TYPE_RAP3 3
#define TYPE_RAP4 4
#define TYPE_SNES 5
#define TYPE_RETROFREAK 6
#define TYPE_8BITDO 7

#define MAX_JOYSTICK 4

//#define LED_PIN A7



#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

bool printAngle;
uint8_t state = 0;


int output = 0;
bool led_on = false;
const struct {
  uint16_t vid;
  uint16_t pid;
  int joy_type;
} Tbl_cnv_data[] = {
  {0x054c, 0x09cc, TYPE_PS4}, // PS4標準コントローラ
  {0x054c, 0x05C4, TYPE_PS4}, // PS4Controller
  //  {0x054c, 0x0268, TYPE_RAP3},// PS3Controller
  {0x1532, 0x0401, TYPE_PS4}, // Razer Panthera
  {0x0C12, 0x0C30, TYPE_PS4}, // Brook universal PCB
  {0xC410, 0xC082, TYPE_PS4}, // UPCB
  {0x0079, 0x0011, TYPE_RAP3}, // Retrobit Sega Saturn wireless
  {0x0ca3, 0x0024, TYPE_MDmini}, // MDmini標準コントローラ
  {0x054c, 0x0ba0, TYPE_PS4}, // PS4 Wireless Adapter
  {0x054c, 0x0cda, TYPE_PSC}, // PlayStation Classic USB Controller
  {0x0f0d, 0x0011, TYPE_RAP3}, // HORI Real Arcade Pro.3 SA PS3コントローラ
  {0x0f0d, 0x0026, TYPE_RAP3}, // HORI Real Arcade リアルアーケードPro.3 Premium VLX
  {0x0f0d, 0x008b, TYPE_RAP3}, // HORI RAP V HAYABUSA Controller(PS3)
  {0x0f0d, 0x008a, TYPE_PS4},  // HORI RAP V HAYABUSA Controller(PS4)
  {0x0f0d, 0x0066, TYPE_PS4},  // HORIPAD FPS+(PS4)
  {0x0f0d, 0x00c1, TYPE_8BITDO},
  {0x0f0d, 0x00ee, TYPE_PS4},  // HORI ワイヤードコントローラライト for PS4-102
  {0x0583, 0x2060, TYPE_SNES}, // iBUFFALO SNES CLASSIC USB GAMEPAD
  {0x1345, 0x1030, TYPE_SNES}, // RetroFreak GAME CONTROLLER
  {0x0413, 0x502b, TYPE_RETROFREAK}, // RetroFreak CONTROLLER ADAPTER
  {0, 0, -1} // データ終端
};

unsigned long mode_time;
byte cnv_mode;
volatile byte if_com_flg;
volatile byte atari_data_SELL;
volatile byte atari_data_SELH;
volatile byte joydrv_snddata[MAX_JOYSTICK][16];
//  0: X X X X X X SELECT START
//  1: R4 R3 R2 R1 L4 L3 L2 L1
//  2: Z' Z Y X D C B A
//  3: R右 R左 R下 R上 L右 L左 L下 L上
//  4: L上下アナログ
//  5: L左右アナログ
//  6: R上下アナログ
//  7: R左右アナログ
//  8: 左アナログボタン1
//  9: 左アナログボタン2
// 10: 左アナログボタン3
// 11: 左アナログボタン4
// 12: 右アナログボタン1
// 13: 右アナログボタン2
// 14: 右アナログボタン3
// 15: 右アナログボタン4
volatile struct {
  byte motor1;
  byte motor2;
  byte led_r;
  byte led_g;
  byte led_b;
  byte flush_on;
  byte flush_off;
  bool flg_change;
} stick_ctrldata[MAX_JOYSTICK];



class JoystickHID : public HIDUniversal {
  public:
    JoystickHID(USB *p) :
      HIDUniversal(p) { };

  protected:
    virtual void ParseHIDData(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
    {
      byte i;
      byte port_no;
      int cnv_pointer;
      byte d_pointer;
      uint8_t w_buf[32];

      port_no = (bAddress >= 9 ? bAddress - 9 : 0);

      cnv_pointer = 0;
      while (Tbl_cnv_data[cnv_pointer].joy_type != -1) {
        if (Tbl_cnv_data[cnv_pointer].vid == VID && Tbl_cnv_data[cnv_pointer].pid == PID) break;
        cnv_pointer++;
      }

      // if (Tbl_cnv_data[cnv_pointer].joy_type == -1) return;

      /*   Serial.print(VID, HEX);
          Serial.print(F(":"));
          Serial.print(PID, HEX);
          Serial.print(F(":"));
          Serial.println(buf[0], HEX);


        // デバッグ用VID/PID buf[x]表示
        char strString[256];
        sprintf(strString, "VID:%04X PID:%04X cnv_pointer:%02X\t", VID, PID, cnv_pointer);
        Serial.print(strString);
        Serial.print(" ");
        for (int i = 0; i < len; i++ ) {
        PrintHex<uint8_t > (buf[i], 0);
        Serial.print(" ");
        }
        Serial.println("");

      */

      joydrv_snddata[port_no][0] = joydrv_snddata[port_no][1] = joydrv_snddata[port_no][2] = joydrv_snddata[port_no][3] = B11111111;
      joydrv_snddata[port_no][4] = joydrv_snddata[port_no][5] = joydrv_snddata[port_no][6] = joydrv_snddata[port_no][7] = B10000000;
      for (i = 8; i < 16; i++) joydrv_snddata[port_no][i] = 0;

      // コントローラ別処理
      switch (Tbl_cnv_data[cnv_pointer].joy_type) {

        case TYPE_RAP3: // RAP (PS3mode)
        // [[ HORI Real Arcade Pro.V HAYABUSA(PS3 MODE) ------------------------------------------------------
        //          joydrv_snddata[port_no][3] = ps_udlr_data[byte(buf[2])];
        default: // 標準

          output = 0;

          if (byte(buf[2]) == 0) {
            output |= 8; // pin A3
            Serial.println("UP");
          }

          if (byte(buf[2]) == 6) {
            output |= 4; //pin A2
            Serial.println("LEFT");
          }
          if (buf[0] & 0x0001) { // Xボタン (square)
            output |= 2;
            Serial.println(F("A button"));
          }

          if (buf[0] & 0x0020) { // R1ボタン
            output |= 1;
            Serial.println("C button");
          }

          if (buf[0] & 0x0040) { // Z Retrobit
            output |= 1;
            Serial.println("C button_RETROBIT");
          }

          if (buf[0] & 0x0080) { // R2ボタン
            output |= 16;
            Serial.println("Z button");
          }
          if (buf[1] & 0x0002) { // STARTボタン
            output |= 32;
            Serial.println("Start button");
          }

          DDRC = output;
          output = 0;

          if (byte(buf[2]) == 2) {
            output |= 8; //pin 3
            Serial.println("RIGHT");
          }
          if (byte(buf[2]) == 4) {
            output |= 4; // pin 2
            Serial.println("DOWN");
          }

          if (byte(buf[2]) == 3) {
            output |= 12; // pin 2 + 3
            Serial.println("DOWN+RIGHT");
          }

          if (buf[0] & 0x0008) { // Yボタン (triangle)
            output |= 16;
            Serial.println(F("B button"));
          }
          if (buf[0] & 0x0002) { // Aボタン (cross)
            output |= 32;
            Serial.println("X button");

          }



          if (buf[1] & 0x0001) { // SELECTボタン
            output |= 64;
            Serial.println("Select button");
          }

          DDRD = output;

          if (byte(buf[2]) == 5) {
            DDRD |= 4;
            DDRC |= 4;
            Serial.println("DOWN+LEFT");
          }

          if (byte(buf[2]) == 7) {
            DDRC |= 12;

            Serial.println("UP+LEFT");
          }

          if (byte(buf[2]) == 1) {
            DDRD |= 8;
            DDRC |= 8;
            Serial.println("UP+RIGHT");
          }


          if (buf[0] & 0x0004) { // Bボタン (circle)
            DDRD |= 128;
            Serial.println("Y button");
          }

          //          if (buf[1] & 0x0008) // R3ボタン
          //            if (buf[0] & 0x0010) // L1ボタン
          //              if (buf[0] & 0x0040) // L2ボタン
          //                if (buf[1] & 0x0004) // L3ボタン


          break;


        case TYPE_8BITDO:
          output = 0;

          if (buf[4] == 0) {
            output |= 8; // pin A3
            Serial.println("UP");
          }

          if (buf[3] == 0) {
            output |= 4; //pin A2
            Serial.println("LEFT");
          }

          if (buf[0] & 0x0001) { // Xボタン (square)
            output |= 2;
            Serial.println(F("A button"));
          }

          if (buf[0] & 0x0020) { // R1ボタン
            output |= 1;
            Serial.println("C button");
          }

          if (buf[0] & 0x0080) { // R2ボタン
            output |= 16;
            Serial.println("Z button");
          }

          if (buf[1] & 0x0002) { // STARTボタン
            output |= 32;
            Serial.println("Start button");
          }

          DDRC = output;
          output = 0;

          if (buf[3] == 0xFF) {
            output |= 8; //pin 3
            Serial.println("RIGHT");
          }

          if (buf[4] == 0xFF) {
            output |= 4; // pin 2
            Serial.println("DOWN");
          }

          if (buf[0] & 0x0008) { // Yボタン (triangle)
            output |= 16;
            Serial.println(F("B button"));
          }

          if (buf[0] & 0x0002) { // Aボタン (cross)
            output |= 32;
            Serial.println("X button");
          }

          if (buf[0] & 0x0004) { // Bボタン (circle)
            output |= 128;
            Serial.println("Y button");
          }

          if (buf[1] & 0x0001) { // SELECTボタン
            output |= 64;
            Serial.println("Select button");
          }

          DDRD = output;
          break;

        case TYPE_PS4: // PS4


          output = 0;

          if (byte(buf[5]  & 0x0F) == 0) {
            output |= 8; // pin A3
            Serial.println("UP");
          }

          if (byte(buf[5]  & 0x0F) == 6) {
            output |= 4; //pin A2
            Serial.println("LEFT");
          }
          if (buf[d_pointer + 5] & 0x0010) { // Xボタン (square)
            output |= 2;

            Serial.println("A button");
          }

          if (buf[d_pointer + 6] & 0x0002) { // R1ボタン
            output |= 1;
            Serial.println("C button");
          }

          if (buf[d_pointer + 6] & 0x0008) { // R2ボタン
            output |= 16;
            Serial.println("Z button");
          }
          if (buf[d_pointer + 6] & 0x0020) { // STARTボタン
            output |= 32;
            Serial.println("Start button");
          }

          DDRC = output;

          output = 0;

          if ((byte(buf[5]) & 0x0F) == 2) {
            Serial.println("RIGHT");
            output |= 8; //pin 3
          }
          if (byte(buf[5] & 0x0F) == 4) {
            output |= 4; // pin 2
            Serial.println("DOWN");
          }

          if (byte(buf[5]  & 0x0F ) == 3) {
            output |= 12; // pin 2 + 3
            Serial.println("DOWN+RIGHT");
          }

          if (buf[d_pointer + 5] & 0x0080) { // Yボタン (triangle)
            output |= 16;
            Serial.println(F("B button"));
          }
          if (buf[d_pointer + 5] & 0x0020) { // Aボタン (cross)
            output |= 32;
            Serial.println("X button");
          }

          if (buf[d_pointer + 5] & 0x0040) { // Bボタン (circle)
            output |= 128;
            Serial.println("Y button");
          }

          if (buf[d_pointer + 6] & 0x0010) { // SELECTボタン
            output |= 64;
            Serial.println("Select button");
          }

          DDRD = output;

          if (byte(buf[5] & 0x0F) == 5 ) {
            DDRD |= 4;
            DDRC |= 4;
            Serial.println("DOWN+LEFT");
          }

          if (byte(buf[5] & 0x0F) == 7) {
            DDRC |= 12;

            Serial.println("UP+LEFT");
          }

          if (byte(buf[5] & 0x0F) == 1 ) {
            DDRD |= 8;
            DDRC |= 8;
            Serial.println("UP+RIGHT");
          }

          if (buf[d_pointer + 6] & 0x0080) // R3ボタン
            if (stick_ctrldata[port_no].flg_change) {
              memset(w_buf, 0, sizeof(w_buf));
              w_buf[0]  = 0x05;
              w_buf[1]  = 0xFF;
              w_buf[4]  = stick_ctrldata[port_no].motor2;
              w_buf[5]  = stick_ctrldata[port_no].motor1;
              w_buf[6]  = stick_ctrldata[port_no].led_r;
              w_buf[7]  = stick_ctrldata[port_no].led_g;
              w_buf[8]  = stick_ctrldata[port_no].led_b;
              w_buf[9]  = stick_ctrldata[port_no].flush_on;
              w_buf[10] = stick_ctrldata[port_no].flush_off;

              stick_ctrldata[port_no].flg_change = false;
              SndRpt(sizeof(w_buf), w_buf);
            }
          break;

      }
    }
};

USB Usb;
PS3USB PS3(&Usb);
XBOXONE Xbox(&Usb);
//XBOXUSB XboxT(&Usb);
PS3Controller PS3Con(&PS3);
XBoxOneController XBoxCon(&Xbox);
USB2DB15 Usb2db15(PS3Con, XBoxCon);


USBHub Hub(&Usb);
JoystickHID Hid1(&Usb);
//JoystickHID Hid2(&Usb);
//JoystickHID Hid3(&Usb);
//JoystickHID Hid4(&Usb);

void setup() {
  Serial.begin(115200);
  
  while (!Serial);

  Serial.print("Using Profile: ");
  Serial.println(EEPROM.read(CURRENT_PROFILE_ADDR));

  byte i, j;

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }
}


void loop() {

  Usb.Task();

  delay(1);

  Usb2db15.GenerateOutput();

  delay(1);

}



//******************************************
// End of File
//******************************************
