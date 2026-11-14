#ifndef USB2DB15_BUILD_CONFIG_H
#define USB2DB15_BUILD_CONFIG_H

// Set to 1 for competitive play without Serial output.
// Set to 0 to enable the Serial Monitor for diagnostics.
#define USB2DB15_RELEASE_MODE 0

#if USB2DB15_RELEASE_MODE
#define REDUCED_SERIAL_OUTPUT
#define RELEASE_MODE
#endif

#endif
