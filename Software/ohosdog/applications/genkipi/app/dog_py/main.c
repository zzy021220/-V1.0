#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"

#include "genki_base.h"
#include "genki_web_ohosdog.h"
#include "genki_web.h"
#include "genki_pin.h"
#include "dog.h"


static void init_service(void) {
    genki_web_register(newDogService());
}

static void start(void) {
    IoTIoSetFunc(IOT_IO_NAME_10, IOT_IO_FUNC_10_I2C0_SDA);
    IoTIoSetFunc(IOT_IO_NAME_9, IOT_IO_FUNC_9_I2C0_SCL);
    IoTI2cInit(0, 400000);

    dog_init();

    genki_services_start();
    init_service();
}

APP_FEATURE_INIT(start);