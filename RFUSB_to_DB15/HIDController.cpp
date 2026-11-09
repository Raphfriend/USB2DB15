//
// Created by Kitsune on 8/22/2020.
//

#include "drivers.h"
#include "HIDController.h"
#include "debug.h"
#include "analog_mappings.h"

/**
 * Returns if there is a controller connected and ready
 *
 * @return If there is a controller connected and ready
 */
bool HIDController::Connected() {
  return isReady();
}

bool HIDController::ConfigReport(uint8_t length, int16_t id) {
  if (length > 64 || id < -1 || id > 255) return false;
  inputReportLength = length;
  inputReportId = id;
  return true;
}

bool HIDController::ConfigAnalog(uint8_t x, uint8_t y, uint8_t lx, uint8_t hx,
                                uint8_t ly, uint8_t hy, bool ix, bool iy,
                                uint8_t reportLength, int16_t reportId) {
  return ConfigAnalogAxes(x, y, 8, false, false, lx, hx, ly, hy, ix, iy,
                          reportLength, reportId);
}

bool HIDController::ConfigAnalogAxes(uint8_t x, uint8_t y, uint8_t bits,
                                    bool signedValue, bool bigEndian,
                                    int32_t lx, int32_t hx, int32_t ly, int32_t hy,
                                    bool ix, bool iy, uint8_t reportLength, int16_t reportId) {
  analogEnabled = false;
  if (bits != 8 && bits != 16) return false;
  const uint8_t width = bits / 8;
  const int32_t minimum = signedValue ? (bits == 8 ? -128L : -32768L) : 0L;
  const int32_t maximum = signedValue ? (bits == 8 ? 127L : 32767L) : (bits == 8 ? 255L : 65535L);
  if (x > 64 - width || y > 64 - width || abs((int)x - (int)y) < width || lx >= hx || ly >= hy ||
      lx < minimum || ly < minimum || hx > maximum || hy > maximum ||
      reportLength > 64 || reportId < -1 || reportId > 255 ||
      (reportLength && (x + width > reportLength || y + width > reportLength))) return false;
  analogX = x; analogY = y;
  analogBits = bits; analogSigned = signedValue; analogBigEndian = bigEndian;
  lowX = lx; highX = hx; lowY = ly; highY = hy;
  invertX = ix; invertY = iy;
  ConfigReport(reportLength, reportId);
  analogEnabled = true;
  return true;
}

// Assemble the integer explicitly without relying on alignment or pointer casts.
static int32_t readAxis(const uint8_t *buf, uint8_t index, uint8_t bits,
                        bool signedValue, bool bigEndian) {
  if (bits == 8) return signedValue ? (int32_t)(int8_t)buf[index] : (int32_t)buf[index];
  uint16_t value = bigEndian ? ((uint16_t)buf[index] << 8) | buf[index + 1]
                            : ((uint16_t)buf[index + 1] << 8) | buf[index];
  return signedValue ? (int32_t)(int16_t)value : (int32_t)value;
}

enum CardinalDirection {
  DIRECTION_UP = 0x01,
  DIRECTION_RIGHT = 0x02,
  DIRECTION_DOWN = 0x04,
  DIRECTION_LEFT = 0x08
};

static uint8_t getDirections(uint32_t state) {
  uint8_t directions = 0;
  if (state & (MASK_UP | MASK_UP_RIGHT | MASK_UP_LEFT)) directions |= DIRECTION_UP;
  if (state & (MASK_RIGHT | MASK_UP_RIGHT | MASK_DOWN_RIGHT)) directions |= DIRECTION_RIGHT;
  if (state & (MASK_DOWN | MASK_DOWN_RIGHT | MASK_DOWN_LEFT)) directions |= DIRECTION_DOWN;
  if (state & (MASK_LEFT | MASK_UP_LEFT | MASK_DOWN_LEFT)) directions |= DIRECTION_LEFT;
  return directions;
}

static uint32_t getDirectionState(uint8_t directions) {
  uint32_t state = 0;
  if (directions & DIRECTION_UP) state |= MASK_UP;
  if (directions & DIRECTION_RIGHT) state |= MASK_RIGHT;
  if (directions & DIRECTION_DOWN) state |= MASK_DOWN;
  if (directions & DIRECTION_LEFT) state |= MASK_LEFT;
  return state;
}

static uint8_t applyHitBoxSocd(uint8_t directions) {
  if ((directions & DIRECTION_LEFT) && (directions & DIRECTION_RIGHT)) {
    directions &= ~(DIRECTION_LEFT | DIRECTION_RIGHT);
  }
  if ((directions & DIRECTION_UP) && (directions & DIRECTION_DOWN)) {
    directions &= ~DIRECTION_DOWN;
  }
  return directions;
}

/**
 * Gets if the button has been clicked since the last check
 *
 * This function checks the buttonState variable to see if the bit
 * corresponding to the button is set to true. Then checks lastButtonState
 * to make sure it wasn't previously pressed.
 *
 * @param button The button to check. Defined in "device_description.h"
 * @return If the requested button is pressed
 */
bool HIDController::GetButtonClick(uint8_t button) {
  // MASK_UP is the farthest right and BUTTON_UP is zero, so shift the
  // mask button times to get the correct mask for the button
  uint32_t mask = MASK_UP >> button;

  if(button >= MAX_HID_BUTTONS) return false;

  if(clickState & mask) {
    clickState &= ~mask;
    return true;
  }
  return false;
}

/**
 * Gets the State(pressed or not) of a given button
 *
 * This function checks the buttonState variable to see if the bit
 * corresponding to the button is set to true. For directionals it checks
 * the primary direction as well as the 2 diagonals that include the direction.
 *
 * @param button The button to check. Defined in "device_description.h"
 * @return If the requested button is pressed
 */
bool HIDController::GetButtonState(uint8_t button) {
  if(button >= MAX_HID_BUTTONS) return false;

  switch (button) {
    case BUTTON_UP :
      return (buttonState & MASK_UP)  || (buttonState & MASK_UP_RIGHT) || (buttonState & MASK_UP_LEFT);
    case BUTTON_DOWN :
      return (buttonState & MASK_DOWN)  || (buttonState & MASK_DOWN_RIGHT) || (buttonState & MASK_DOWN_LEFT);
    case BUTTON_RIGHT :
      return (buttonState & MASK_RIGHT)  || (buttonState & MASK_UP_RIGHT) || (buttonState & MASK_DOWN_RIGHT);
    case BUTTON_LEFT :
      return (buttonState & MASK_LEFT)  || (buttonState & MASK_UP_LEFT) || (buttonState & MASK_DOWN_LEFT);
    case BUTTON_NULL:
      return false;
    default:
      // MASK_UP is the farthest right and BUTTON_UP is zero, so shift the
      // mask button times to get the correct mask for the button
      return (buttonState & (MASK_UP >> button));
  }
}

/**
 * Gets the Vendor ID for the controller
 * @return The Controller's Vendor ID
 */
uint16_t HIDController::GetVid() {
  return VID;
}

/**
 * Gets the Product ID for the controller
 * @return The Controller's Product ID
 */
uint16_t HIDController::GetPid() {
  return PID;
}

/**
 * Configures a button.
 *
 * This function is an alias for ConfigButton(uint8_t, uint8_t, uint8_t, uint8_t)
 * and should be used when the Mask is the same as the target value. This is often
 * true for single bit masks.
 *
 * @param button_id The button id as defined in "device_descriptor.h"
 * @param index The index in the usb packet buffer that the button is located at
 * @param mask The bit mask to target the bits that make up the button
 */
void HIDController::ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask) {
  ConfigButton(button_id, index, mask, mask);
}

/**
 * Configures a Button.
 *
 * This functions configures a button for use in the ParseHIDData function.
 *
 * @param button_id The button id as defined in "device_descriptor.h"
 * @param index The index in the usb packet buffer that the button is located
 * @param mask The bit mask to target the bits that make up the button
 * @param value The value the byte should equal after the bit mask is applied
 */
void HIDController::ConfigButton(uint8_t button_id, uint8_t index, uint8_t mask, uint8_t value) {
  if (button_id >= MAX_HID_BUTTONS) return;
  #ifndef REDUCED_SERIAL_OUTPUT
  Serial.print("Button ID: ");
  Serial.print(button_id, DEC);
  Serial.print(" Index: ");
  Serial.print(index, DEC);
  Serial.print(" Mask: ");
  Serial.print(mask, BIN);
  Serial.print(" Value: ");
  Serial.println(value, DEC);
  #endif
  buttons[button_id].index = index;
  buttons[button_id].mask = mask;
  buttons[button_id].value = value;
}

void HIDController::EnableHitBoxSocd(bool enabled) {
  analogSocdEnabled = enabled;
}

/**
 * Handle any setup that needs to happen when a controller is plugged in
 *
 * This is a callback function that is invoked by the USB Shield library
 * once a controller is plugged in and successfully initalized.
 *
 * This function looks up and runs the configuration for the controller
 * that was plugged in.
 *
 * @return error number, zero for no error
 */
uint8_t HIDController::OnInitSuccessful() {
  #ifndef RELEASE_MODE
  Serial.println("Detected HID device. ");
  #endif
  ResetController();
  setupController(VID, PID, this);
  setupAnalogMapping(VID, PID, this);
  return 0;
}

/**
 * Processes in incoming USB data packet
 *
 * This is a callback function defined by HIDUniversal and is called whenever
 * the controller sends data about it's current state. Do not call this
 * function directly
 *
 * This function resets the buttonState to 0. It then iterates over each button
 * shifting a 1 onto buttonState if it is pressed otherwise it leaves it at
 * zero. The end result is that the current state of each button is represented
 * as a single bit in buttonState. This will allow us to check the value of any
 * button by ANDing buttonState with a bit mask later on.
 *
 * @param hid
 * @param is_rpt_id
 * @param len Length of the buffer
 * @param buf The buffer
 */
void HIDController::ParseHIDData(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  // Ignore unrelated reports without releasing the current gamepad state.
  if ((inputReportLength && len != inputReportLength) ||
      (inputReportId >= 0 && (!len || buf[0] != inputReportId))) return;
  uint8_t value = 0;
  buttonState = 0;

  // For each button shift left then append the button state
  // UP is the left most bit, BUTTON_10 is right most
  for(uint8_t i = 0; i < MAX_HID_BUTTONS; i++) {
    buttonState <<= 1;  // Shift to the next bit
    if (buttons[i].mask == 0 || buttons[i].index >= len) continue;

    value = buf[buttons[i].index] & buttons[i].mask;  // Get the bits we care about
    if(value == buttons[i].value) {
      buttonState |= 1;  // Set the final bit
    }
  }

  if (analogEnabled && analogX + analogBits / 8 <= len && analogY + analogBits / 8 <= len) {
    const int32_t x = readAxis(buf, analogX, analogBits, analogSigned, analogBigEndian);
    const int32_t y = readAxis(buf, analogY, analogBits, analogSigned, analogBigEndian);
    if (analogSocdEnabled) {
      uint8_t directions = getDirections(buttonState);
      if (x < lowX)  directions |= invertX ? DIRECTION_RIGHT : DIRECTION_LEFT;
      if (x > highX) directions |= invertX ? DIRECTION_LEFT : DIRECTION_RIGHT;
      if (y < lowY)  directions |= invertY ? DIRECTION_DOWN : DIRECTION_UP;
      if (y > highY) directions |= invertY ? DIRECTION_UP : DIRECTION_DOWN;
      buttonState &= ~MASK_DIRECTIONS;
      buttonState |= getDirectionState(applyHitBoxSocd(directions));
    } else {
      if (x < lowX)  buttonState |= invertX ? MASK_RIGHT : MASK_LEFT;
      if (x > highX) buttonState |= invertX ? MASK_LEFT : MASK_RIGHT;
      if (y < lowY)  buttonState |= invertY ? MASK_DOWN : MASK_UP;
      if (y > highY) buttonState |= invertY ? MASK_UP : MASK_DOWN;
    }
  }

  // Update Click State and LastButtonState
  if(buttonState != lastButtonState) {
    clickState = buttonState & ~lastButtonState;
    lastButtonState = buttonState;
  }
}

/**
 * Unsets all buttons on the controller
 *
 * Sets all button masks to zero.  This signals to the system that the buttons
 * are unset and should not be used.
 */
void HIDController::ResetController() {
  analogEnabled = false;
  analogSocdEnabled = false;
  inputReportLength = 0;
  inputReportId = -1;
  buttonState = lastButtonState = clickState = 0;
  for(uint8_t i = 0; i < MAX_HID_BUTTONS; i++) {
    buttons[i].mask = 0;
  }
}
