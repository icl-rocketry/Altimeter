#pragma once

#include <stdint.h>
#include <unordered_map>
#include <functional>
#include <initializer_list>

#include <libriccore/commands/commandhandler.h>
#include <librnp/rnp_packet.h>

#include "Config/forward_decl.h"
#include "Commands/commands.h"


namespace Commands
{
    enum class ID : uint8_t
    {
        Nocommand = 0,
        Launch_Abort = 3,
        Set_Home = 4,
        Start_Logging = 5,
        Stop_Logging = 6,
        Telemetry = 8,
        Play_Song = 14,
        Skip_Song = 15,
        Clear_Song_Queue = 16,
        Reset_Orientation = 50,
        Reset_Localization = 51,
        Set_Beta = 52,
        Calibrate_AccelGyro_Bias = 60, // bias callibration requires sensor z axis aligned with up directioN!
        Calibrate_Mag_Full = 61, //changed for compatibility
        Calibrate_HighGAccel_Bias = 62,
        Calibrate_Baro = 63,
        Prevent_Logging = 70,
        Read_Meta_Data = 71,
        Dump_Data = 72,
        Dump_Saved_Data = 73,
        Free_Ram = 250,
    };

    inline std::initializer_list<ID> defaultEnabledCommands = { // if you want commands enabled use this
        ID::Free_Ram,
        ID::Telemetry,
        ID::Prevent_Logging,
        ID::Read_Meta_Data,
        ID::Dump_Data,
        ID::Dump_Saved_Data
    };

    inline std::unordered_map<ID, std::function<void(ForwardDecl_SystemClass &, const RnpPacketSerialized &)>> command_map{
        {ID::Calibrate_AccelGyro_Bias, CalibrateAccelGyroBiasCommand},
        {ID::Calibrate_HighGAccel_Bias, CalibrateHighGAccelBiasCommand},
        {ID::Calibrate_Mag_Full, CalibrateMagFullCommand},
        {ID::Calibrate_Baro, CalibrateBaroCommand},
        {ID::Set_Beta, SetBetaCommand},
        {ID::Telemetry, TelemetryCommand},
        {ID::Free_Ram, FreeRamCommand}
        // {ID::Prevent_Logging, PreventLoggingCommand},
        // {ID::Read_Meta_Data, ReadMetaDataCommand},
        // {ID::Dump_Data, DumpData},
        // {ID::Dump_Saved_Data, DumpSavedDataCommand}
    };



};