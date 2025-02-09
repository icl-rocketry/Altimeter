#include "system.h"

#include <memory>

#include <ArduinoJson.h>

#include <libriccore/riccoresystem.h>
#include <libriccore/storage/wrappedfile.h>


#include "Config/systemflags_config.h"
#include "Config/commands_config.h"
#include "Config/pinmap_config.h"
#include "Config/general_config.h"

#include "Commands/commands.h"


#include "Sensors/sensors.h"
#include "Sensors/estimator.h"
#include "Sensors/sensorStructs.h"
#include "Sound/tunezHandler.h"


#include "States/idle.h"



#include "esp_system.h"
#include <Arduino.h>


#ifdef CONFIG_IDF_TARGET_ESP32S3
static constexpr int VSPI_BUS_NUM = 0;
static constexpr int HSPI_BUS_NUM = 1;
#else
static constexpr int VSPI_BUS_NUM = VSPI;
static constexpr int HSPI_BUS_NUM = HSPI;
#endif

System::System() : RicCoreSystem(Commands::command_map, Commands::defaultEnabledCommands, Serial),
                   vspi(VSPI_BUS_NUM),
                   hspi(HSPI_BUS_NUM),
                   sensors(hspi, systemstatus),
                   estimator(systemstatus),
                   nandflash(vspi, PinMap::IC_Cs, PinMap::IC_WP, PinMap::IC_Hold)
                   {};

void System::systemSetup()
{

    Serial.setRxBufferSize(GeneralConfig::SerialRxSize);
    Serial.begin(GeneralConfig::SerialBaud);
  
    statemachine.initalize(std::make_unique<Idle>(systemstatus,commandhandler));
    setupPins();
    // initalize spi interface
    setupSPI();

    tunezhandler.setup();

    loadConfig();

    estimator.setup();
    estimator.setHome(sensors.getData());

    delay(5000);
    Serial.println("clearing line");
    nandflash.setup();

    for (int i = 0; i < 2048; i++) {
        nandflash.m_cache_ptr[i] = 6;
    }
    // nandflash.write_page(0,1);
    nandflash.read_page(0,1);
    nandflash.print_cache();
};

void System::systemUpdate()
{
    // tunezhandler.update();
    sensors.update();
    estimator.update(sensors.getData());
    
    nandflash.read_page(0,1);
    nandflash.print_cache_short();
};

void System::setupSPI()
{
    hspi.begin(PinMap::SNSR_SCLK,PinMap::SNSR_MISO,PinMap::SNSR_MOSI);
    hspi.setFrequency(8000000);
    hspi.setBitOrder(MSBFIRST);
    hspi.setDataMode(SPI_MODE0);

    vspi.begin(PinMap::IC_SCLK,PinMap::IC_MISO,PinMap::IC_MOSI);
    vspi.setFrequency(1000000);
    vspi.setBitOrder(MSBFIRST);
    vspi.setDataMode(SPI_MODE0); // Set the SPI data mode
}

void System::setupPins()
{
    pinMode(PinMap::Accel_Gyro_Cs, OUTPUT);
    pinMode(PinMap::Accel_Cs, OUTPUT);
    pinMode(PinMap::Baro_Cs, OUTPUT);
    pinMode(PinMap::Mag_Cs, OUTPUT);
    pinMode(PinMap::IC_Cs, OUTPUT);
    pinMode(PinMap::IC_WP, OUTPUT);
    pinMode(PinMap::LED_RED, OUTPUT);
    pinMode(PinMap::LED_BLUE, OUTPUT);
    pinMode(PinMap::LED_GREEN, OUTPUT);
    // initialise cs pins
    digitalWrite(PinMap::Accel_Gyro_Cs, HIGH);
    digitalWrite(PinMap::Accel_Cs, HIGH);
    digitalWrite(PinMap::Baro_Cs, HIGH);
    digitalWrite(PinMap::Mag_Cs, HIGH);
    digitalWrite(PinMap::IC_Cs, HIGH);
    digitalWrite(PinMap::IC_WP, HIGH);
    digitalWrite(PinMap::LED_RED, HIGH);
    digitalWrite(PinMap::LED_BLUE, HIGH);
    digitalWrite(PinMap::LED_GREEN, HIGH);
}

void System::loadConfig()
{
    DynamicJsonDocument configDoc(16384); //allocate 16kb for config doc MAXSIZE
    DeserializationError jsonError;

    try
    {
        sensors.setup(configDoc.as<JsonObjectConst>()["Sensors"]);

    }
    catch (const std::exception &e)
    {
         RicCoreLogging::log<RicCoreLoggingConfig::LOGGERS::SYS>("Exception occured while loading flight config! - " + std::string(e.what()));

         throw e; //continue throwing as we dont want to continue
    }
   
}