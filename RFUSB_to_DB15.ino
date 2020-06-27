// USB controller -> ATARI ver 1.21
// Copyright
//   メインプログラム、コントローラタイプ(TYPE_PS4, TYPE_MDmini) :
//     HUYE 2018-2020
//   コントローラタイプ(TYPE_PSC, TYPE_RAP3, TYPE_RAP4, TYPE_SNES, TYPE_RETROFREAK) :
//     たねけん 2020
//
#include <XBOXONE.h>
#include <EEPROM.h>
#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

#define TYPE_PS4 0
#define TYPE_MDmini 1
#define TYPE_PSC 2
#define TYPE_RAP3 3
#define TYPE_RAP4 4
#define TYPE_SNES 5
#define TYPE_RETROFREAK 6

#define MAX_JOYSTICK 4
#define CYBER_WAIT  25
#define JOYDRV_WAIT 1000000L
#define AD_CNV_DATA1 64
#define AD_CNV_DATA2 192

#define IF_MODE_ATARI 0
#define IF_MODE_CPSF  1
#define IF_MODE_CYBER 2
#define IF_MODE_JOYDRV 3

#define MODE_CHG_TIME 3000 // 3秒

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>


int output = 0;
const struct {
  uint16_t vid;
  uint16_t pid;
  int joy_type;
} Tbl_cnv_data[] = {
  {0x054c, 0x09cc, TYPE_PS4}, // PS4標準コントローラ
  {0x054c, 0x05C4, TYPE_PS4}, // PS4Controller
  {0x1532, 0x0401, TYPE_PS4}, // Razer Panthera
  {0x045E, 0x02D1, TYPE_PS4}, // XONE Controller
  {0x0ca3, 0x0024, TYPE_MDmini}, // MDmini標準コントローラ
  {0x054c, 0x0ba0, TYPE_PS4}, // PS4 Wireless Adapter
  {0x054c, 0x0cda, TYPE_PSC}, // PlayStation Classic USB Controller
  {0x0f0d, 0x0011, TYPE_RAP3}, // HORI Real Arcade Pro.3 SA PS3コントローラ
  {0x0f0d, 0x0026, TYPE_RAP3}, // HORI Real Arcade リアルアーケードPro.3 Premium VLX
  {0x0f0d, 0x008b, TYPE_RAP3}, // HORI RAP V HAYABUSA Controller(PS3)
  {0x0f0d, 0x008a, TYPE_PS4},  // HORI RAP V HAYABUSA Controller(PS4)
  {0x0f0d, 0x0066, TYPE_PS4},  // HORIPAD FPS+(PS4)
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

const byte ps_udlr_data[16] = {B11111110, B11110110, B11110111, B11110101, B11111101, B11111001, B11111011, B11111010,
                               B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111
                              };

const byte psc_udlr_data[16] = {B11111010, B11111110, B11110110, B11111111, B11111011, B11111111, B11110111, B11111111,
                                B11111001, B11111101, B11110101, B11111111, B11111111, B11111111, B11111111, B11111111
                               };

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

      // コントローラ別処理
      switch (Tbl_cnv_data[cnv_pointer].joy_type) {

        case TYPE_RAP3: // RAP (PS3mode)
          // [[ HORI Real Arcade Pro.V HAYABUSA(PS3 MODE) ------------------------------------------------------
          joydrv_snddata[port_no][3] = ps_udlr_data[byte(buf[2])];


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

          if (buf[0] & 0x0004) { // Bボタン (circle)
            output |= 128;
            Serial.println("Y button");
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






          //   joydrv_snddata[port_no][2] &= B11111110;

          //   joydrv_snddata[port_no][2] &= B11111101;

          //     joydrv_snddata[port_no][1] &= B11101111;

          //     joydrv_snddata[port_no][1] &= B11011111;
          if (buf[1] & 0x0008) // R3ボタン

            /*if (buf[0] & 0x0001) // Xボタン (square)

              Serial.println(F("A button"))
              //  joydrv_snddata[port_no][2] &= B11101111;
              if (buf[0] & 0x0008) // Yボタン (triangle)
              Serial.println("B button");;*/

            if (buf[0] & 0x0010) // L1ボタン

              if (buf[0] & 0x0040) // L2ボタン

                if (buf[1] & 0x0004) // L3ボタン




                  //   joydrv_snddata[port_no][0] &= B11111101;

                  if (stick_ctrldata[port_no].flg_change)
                    stick_ctrldata[port_no].flg_change = false;
          break;

        case TYPE_PS4: // PS4
defult: // 標準
          //          if (buf[0] == 0x01) d_pointer = 0;
          //          else if (buf[0] == 0x11) {
          //            if (len < 4) return;
          //            d_pointer = 2;
          //          }
          //          else return;

          //          joydrv_snddata[port_no][5] = byte(buf[d_pointer + 1]); // L左右アナログ
          //          joydrv_snddata[port_no][4] = byte(buf[d_pointer + 2]); // L上下アナログ
          //          joydrv_snddata[port_no][7] = byte(buf[d_pointer + 3]); // R左右アナログ
          //          joydrv_snddata[port_no][6] = byte(buf[d_pointer + 4]); // R上下アナログ
          //          joydrv_snddata[port_no][8]  = byte(buf[d_pointer + 8]); // 左アナログボタン
          //          joydrv_snddata[port_no][12] = byte(buf[d_pointer + 9]); // 右アナログボタン

          //       joydrv_snddata[port_no][3] = ps_udlr_data[byte(buf[d_pointer+5])&B00001111];



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






          //  joydrv_snddata[port_no][2] &= B11111101;




          // joydrv_snddata[port_no][1] &= B11101111;

          //  joydrv_snddata[port_no][1] &= B11011111;
          if (buf[d_pointer + 6] & 0x0080) // R3ボタン
            //   joydrv_snddata[port_no][1] &= B10111111;






            //  joydrv_snddata[port_no][2] &= B11101111;




            //            //  joydrv_snddata[port_no][2] &= B11011111;
            //            if (buf[d_pointer + 6] & 0x0001) // L1ボタン
            //              joydrv_snddata[port_no][1] &= B11111110;
            //          if (buf[d_pointer + 6] & 0x0004) // L2ボタン
            //            joydrv_snddata[port_no][1] &= B11111101;
            //          if (buf[d_pointer + 6] & 0x0040) // L3ボタン
            //            joydrv_snddata[port_no][1] &= B11111011;
            //

            //         joydrv_snddata[port_no][0] &= B11111110;

            // joydrv_snddata[port_no][0] &= B11111101;

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
XBOXONE Xbox(&Usb);
USBHub Hub(&Usb);
JoystickHID Hid1(&Usb);
JoystickHID Hid2(&Usb);
JoystickHID Hid3(&Usb);
JoystickHID Hid4(&Usb);

void setup() {

  // PORTC = B00000000;
  // PORTD = B00000000;
  byte i, j;

  Serial.begin(115200);
  while (!Serial);



  //  set_cnv_mode();

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1);
  }

  mode_time = millis();  /* 起動時の時間 */
  if_com_flg = 0; /* 通信要求クリア */
}



void loop() {

  Usb.Task();
  if (Xbox.XboxOneConnected) {
    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500
        || Xbox.getAnalogHat(LeftHatY) > 7500
        || Xbox.getAnalogHat(LeftHatY) < -7500
        || Xbox.getAnalogHat(RightHatX) > 7500
        || Xbox.getAnalogHat(RightHatX) < -7500
        || Xbox.getAnalogHat(RightHatY) > 7500
        || Xbox.getAnalogHat(RightHatY) < -7500) {
      if (Xbox.getAnalogHat(LeftHatX) > 7500
          || Xbox.getAnalogHat(LeftHatX) < -7500) {
        Serial.print(F("LeftHatX: "));
        Serial.print(Xbox.getAnalogHat(LeftHatX));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(LeftHatY) > 7500
          || Xbox.getAnalogHat(LeftHatY) < -7500) {
        Serial.print(F("LeftHatY: "));
        Serial.print(Xbox.getAnalogHat(LeftHatY));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(RightHatX) > 7500
          || Xbox.getAnalogHat(RightHatX) < -7500) {
        Serial.print(F("RightHatX: "));
        Serial.print(Xbox.getAnalogHat(RightHatX));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
        Serial.print(F("RightHatY: "));
        Serial.print(Xbox.getAnalogHat(RightHatY));
      }
      Serial.println();
    }

    if (Xbox.getButtonPress(L2) > 0 || Xbox.getButtonPress(R2) > 0) {
      if (Xbox.getButtonPress(L2) > 0) {
        Serial.print(F("L2: "));
        Serial.print(Xbox.getButtonPress(L2));
        Serial.print("\t");
      }
      if (Xbox.getButtonPress(R2) > 0) {
        Serial.print(F("R2: "));
        Serial.print(Xbox.getButtonPress(R2));
        Serial.print("\t");
      }
      Serial.println();
    }

    // Set rumble effect
    //    static uint16_t oldL2Value, oldR2Value;
    //    if (Xbox.getButtonPress(L2) != oldL2Value || Xbox.getButtonPress(R2) != oldR2Value) {
    //      oldL2Value = Xbox.getButtonPress(L2);
    //      oldR2Value = Xbox.getButtonPress(R2);
    //    uint8_t leftRumble = map(oldL2Value, 0, 1023, 0, 255); // Map the trigger values into a byte
    //    uint8_t rightRumble = map(oldR2Value, 0, 1023, 0, 255);
    //    if (leftRumble > 0 || rightRumble > 0)
    //      Xbox.setRumbleOn(leftRumble, rightRumble, leftRumble, rightRumble);
    //    else
    Xbox.setRumbleOff();
    //}

    if (Xbox.getButtonClick(UP))
      Serial.println(F("Up"));

    output = 0;

    if (Xbox.getButtonClick(LEFT)) {
      output |= 4; //pin A2

      Serial.println(F("Left"));
    }

    DDRC = output;
    output = 0;

    if (Xbox.getButtonClick(DOWN)) {
      output |= 4; // pin 2
      Serial.println(F("Down"));
    }


    DDRD = output;



    if (Xbox.getButtonClick(RIGHT))
      Serial.println(F("Right"));

    if (Xbox.getButtonClick(START))
      Serial.println(F("Start"));
    if (Xbox.getButtonClick(BACK))
      Serial.println(F("Back"));
    if (Xbox.getButtonClick(XBOX))
      Serial.println(F("Xbox"));
    if (Xbox.getButtonClick(SYNC))
      Serial.println(F("Sync"));

    if (Xbox.getButtonClick(L1))
      Serial.println(F("L1"));
    if (Xbox.getButtonClick(R1))
      Serial.println(F("R1"));
    if (Xbox.getButtonClick(L2))
      Serial.println(F("L2"));
    if (Xbox.getButtonClick(R2))
      Serial.println(F("R2"));
    if (Xbox.getButtonClick(L3))
      Serial.println(F("L3"));
    if (Xbox.getButtonClick(R3))
      Serial.println(F("R3"));


    if (Xbox.getButtonClick(A))
      Serial.println(F("A"));
    if (Xbox.getButtonClick(B))
      Serial.println(F("B"));
    if (Xbox.getButtonClick(X))
      Serial.println(F("X"));
    if (Xbox.getButtonClick(Y))
      Serial.println(F("Y"));
  }
  delay(1);






  byte cyber_data[12];
  // 上位4bitはステータス、下位4bitがデータ
  //  0 : A+A' B+B' C D
  //  1 : E1 E2 F(START) G(SELECT)
  //  2 : 左上下 上位4bit
  //  3 : 左左右 上位4bit
  //  4 : 右上下 上位4bit
  //  5 : 右左右 上位4bit
  //  6 : 左上下 下位4bit
  //  7 : 左左右 下位4bit
  //  8 : 右上下 下位4bit
  //  9 : 右左右 下位4bit
  // 10 : A B A' B'
  // 11 : 1 1 1 1
  int joydrv_port;
  int motor1;
  int motor2;
  byte atari_work_SELL;
  byte atari_work_SELH;
  byte flg_chg_mode;
  int i;
  unsigned long now_time;

  if (if_com_flg == 0) Usb.Task();

  now_time = millis(); // 現在の起動からの時間

  if ((now_time - mode_time) >= MODE_CHG_TIME) {
    SPI.end();
    cnv_mode = flg_chg_mode;
    EEPROM.write(0, cnv_mode);
    //   set_cnv_mode();
    SPI.begin();
    mode_time = now_time = millis(); // 現在の起動からの時間
    if_com_flg = 0; /* 通信要求クリア */
  }
}







/* PC本体側のBUSY終了待ち */








void int_cyber()
{
  if_com_flg = 1;
}

void int_joydrv()
{
  if_com_flg = 2;
}
