//
// Created by Kitsune on 8/21/2020.
//

#ifndef USB2DB15_NEOGEOUSBCONTROLLER_H
#define USB2DB15_NEOGEOUSBCONTROLLER_H

#include "Controller.h"
#include "NeoGeoXboxUSB.h"

#define XBOX_VID    0x2DC8
#define XBOX_PID    0x3106

/**
 * Wrapper for the USB Host Shield XBoxOne Controller
 *
 * This class wraps the USB Host Shield XBoxUSB controller. It implements
 * the Controller Interface that the USB2DB15 class expects.
 */
class NeoGeoUSBController : public Controller {
    NeoGeoXboxUSB *neoGeo;

public:
    NeoGeoUSBController(XBOXUSB *p, NeoGeoXboxUSB *neo);

    bool GetButtonClick(uint8_t button);
    bool GetButtonState(uint8_t button);
    bool Connected();

    uint16_t GetVid();
    uint16_t GetPid();
};

#endif //USB2DB15_NEOGEOUSBCONTROLLER_H
