#pragma once

#include <libriccore/riccoresystem.h>

#include <string_view>
#include <array>

//config includes
#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"
#include "Config/types.h"

#include <SPI.h>
#include <Wire.h>


#include "Commands/commands.h"


#include "Sensors/sensors.h"
#include "Sensors/estimator.h"

#include "Sound/tunezHandler.h"

#include <librnp/rnp_networkmanager.h>
#include "Storage/nandflash.hpp"

class System : public RicCoreSystem<System,SYSTEM_FLAG,Commands::ID>
{
    public:

        System();
        
        void systemSetup();

        void systemUpdate();

        //board communication
        SPIClass vspi;
        SPIClass hspi;


        Sensors sensors;
        Estimator estimator;

        TunezHandler tunezhandler;


        NANDFlash nandflash;

        uint8_t* data_ptr;

    private:

        void setupSPI();
        void setupPins();
        void loadConfig();


        uint32_t telemetry_log_delta = 5000; //200hz
        uint32_t prev_telemetry_log_time;

       
        uint32_t prevTime;
        


};