//
// Created by Frank_fjs on 7/10/2020.
//

// USB2DB15_RELEASE_MODE is defined in BuildConfig.h.
// Release mode disables Serial output so it cannot delay controller polling.
#include "BuildConfig.h"

#if USB2DB15_RELEASE_MODE
#define REDUCED_SERIAL_OUTPUT
#define RELEASE_MODE
#endif
