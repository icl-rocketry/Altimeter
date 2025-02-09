/**
 * @file commands.h
 * @author Kiran de Silva (kd619@ic.ac.uk)
 * @brief Contains signatures of all commands in the system. Note there is no requirement ot have all the command signatures defined in a single file, just ensure all the seperate files are included into the command_config.h
 * @version 0.1
 * @date 2023-06-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <stdint.h>

// Forward declaration required here as commands.h contains command function signatures, so is included in Config/commands_config.h which is 
// then included in system.h as the command id enum type is required in the generation of the riccoresystem template. Better solutions may
//exist but currently this seems okay
#include "Config/forward_decl.h" 

#include <librnp/rnp_packet.h>

namespace Commands{
    
    void SetBetaCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void CalibrateAccelGyroBiasCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void CalibrateHighGAccelBiasCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void CalibrateMagFullCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void CalibrateBaroCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void FreeRamCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void SetHomeCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void StartLoggingCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void StopLoggingCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void TelemetryCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void ResetOrientationCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    void ResetLocalizationCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    // void PreventLoggingCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    // void ReadMetaDataCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    // void DumpData(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);
    // void DumpSavedDataCommand(ForwardDecl_SystemClass& system, const RnpPacketSerialized& packet);

    
}