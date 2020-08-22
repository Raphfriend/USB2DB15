//
// Created by Kitsune on 8/21/2020.
//

#ifndef USB2DB15_DEVICE_DESCRIPTOR_H
#define USB2DB15_DEVICE_DESCRIPTOR_H

// Button Types
#define BUTTON_SINGLE 0
#define BUTTON_DPAD 1
#define BUTTON_DIRECTIONAL 2

// Directional Constants/D-Pad Directions
#define DIR_UP 0
#define DIR_UP_RIGHT 1
#define DIR_RIGHT 2
#define DIR_DOWN_RIGHT 3
#define DIR_DOWN 4
#define DIR_DOWN_LEFT 5
#define DIR_LEFT 6
#define DIR_UP_LEFT 7

// Directional Digital Constants
#define DIR_MAX = 0xFF
#define DIR_MIX = 0x00

// Common Button Ids
#define BUTTON_UP         0
#define BUTTON_UP_RIGHT   1
#define BUTTON_RIGHT      2
#define BUTTON_DOWN_RIGHT 3
#define BUTTON_DOWN       4
#define BUTTON_DOWN_LEFT  5
#define BUTTON_LEFT       6
#define BUTTON_UP_LEFT    7
#define BUTTON_START      8
#define BUTTON_COIN       9   // Select, Back
#define BUTTON_1          10  // A, Square
#define BUTTON_2          11  // B, Triangle
#define BUTTON_3          12  // C, R1
#define BUTTON_4          13  // X, X
#define BUTTON_5          15  // Y, Circle
#define BUTTON_6          16  // Z, R2
#define BUTTON_7          17  // L1
#define BUTTON_8          18  // L2
#define BUTTON_9          19  // R3
#define BUTTON_10         20  // L3

#endif //USB2DB15_DEVICE_DESCRIPTOR_H
