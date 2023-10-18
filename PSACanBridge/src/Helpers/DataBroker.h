#pragma once

#ifndef _DataBroker_h
    #define _DataBroker_h

#include <stdint.h>
#include "IntUnions.h"

class DataBroker
{
    public:
        //036
        uint8_t Ignition;
        uint8_t IgnitionMode;
        uint8_t Brightness;
        uint8_t BlackPanel;
        uint8_t EconomyMode;
        uint8_t NightMode;
        bool IgnitionDataArrived = false;
        //0F6
        uint8_t S_0F6Byte1;
        uint8_t S_0F6Byte2;
        uint8_t S_0F6Byte3;
        uint8_t S_0F6Byte4;
        uint8_t S_0F6Byte5;
        uint8_t S_0F6Byte6;
        uint8_t S_0F6Byte7;
        uint8_t S_0F6Byte8;
        uint8_t ExternalTemperature;
        uint8_t CoolantTemperature;
        uint8_t IsReverseEngaged;
        //0B6
        uint8_t S_0B6Byte1;
        uint8_t S_0B6Byte2;
        uint8_t S_0B6Byte3;
        uint8_t S_0B6Byte4;
        uint8_t S_0B6Byte5;
        uint8_t S_0B6Byte6;
        uint8_t S_0B6Byte7;
        uint8_t S_0B6Byte8;
        //217
        uint8_t S_217Byte1;
        uint8_t S_217Byte2;
        uint8_t S_217Byte3;
        uint8_t S_217Byte4;
        uint8_t S_217Byte5;
        uint8_t S_217Byte6;
        uint8_t S_217Byte7;
        uint8_t S_217Byte8;
        //227
        uint8_t S_227Byte1;
        uint8_t S_227Byte2;
        uint8_t S_227Byte3;
        uint8_t S_227Byte4;
        //221
        uint8_t S_221Byte1;
        uint8_t S_221Byte2;
        uint8_t S_221Byte3;
        uint8_t S_221Byte4;
        uint8_t S_221Byte5;
        uint8_t S_221Byte6;
        uint8_t S_221Byte7;
        //128
        uint8_t OdometerEnabled;
        uint8_t FuelLow;
        uint8_t SeatbeltWarning;
        uint8_t PassengerAirbagDeactivated;
        uint8_t ChildSafetyActivated;
        uint8_t Handbrake;
        uint8_t Airbag;
        uint8_t PreHeatingStatus;
        uint8_t StopOnCMB;
        uint8_t EspDeactivated;
        uint8_t EspInRegulation;
        uint8_t SideLights;
        uint8_t DRL;
        uint8_t LeftTurnIndicator;
        uint8_t RightTurnIndicator;
        uint8_t RearFog;
        uint8_t FrontFog;
        uint8_t LowBeam;
        uint8_t HighBeam;
        uint8_t GearBoxMode;
        uint8_t GearBoxSelection;
        uint8_t GearPosition;
        uint8_t GearBlinking;
        uint8_t GearPositionDrive;
        uint8_t S_128Byte7;
        uint8_t S_128Byte8;
        uint8_t ServiceOnCMB;
        uint8_t FootOnBrake;
        uint8_t SuspensionStatus;
        uint8_t SpaceMeasureBlinking;
        uint8_t SpaceMeasureOn;
        uint8_t DoorOpen;
        uint8_t Row1RLSeatBelt;
        uint8_t Row1RLSeatBeltBlinking;
        uint8_t SCRIndicator;
        uint8_t PassengerSeatbeltBlinking;
        uint8_t PassengerSeatbeltWarning;
        uint8_t DriverSeatbeltWarning;
        uint8_t DriverSeatbeltWarningBlinking;
        uint8_t FSEDisabled;
        uint8_t ActivatePassengerProtection;
        uint8_t WarningLight;
        uint8_t Row1RRSeatBelt;
        uint8_t Row1RRSeatBeltBlinking;
        uint8_t Row1RCSeatBelt;
        uint8_t Row1RCSeatBeltBlinking;
        uint8_t ChangePersonalization;
        uint8_t ChangeColor;
        uint8_t Ready;
        uint8_t MINCBlinking;
        uint8_t FootOnClutch;
        //168
        uint8_t S_168Byte1;
        uint8_t EngineSpeedThreshold2;
        uint8_t EngineSpeedThreshold1;
        uint8_t AutoWipingActive;
        uint8_t FapClogged;
        uint8_t FlatTyreAlert;
        uint8_t TyrePressureAlert;
        uint8_t GeneratorFault;
        uint8_t SeriousSuspensionFault;
        uint8_t SeriousREFFault;
        uint8_t Mil;
        uint8_t BrakePadFault;
        uint8_t AutoGearboxFault;
        uint8_t EspFault;
        uint8_t AbsFault;
        uint8_t FSETighteningFault;
        uint8_t FSESystemFault;
        uint8_t STTLampStatus;
        uint8_t PowerSteeringFault;
        uint8_t CAARLampStatus;
        uint8_t CurveCodeFault;
        uint8_t WaterInDiesel;
        uint8_t EnableVTH;
        uint8_t Row2RRSeatBelt;
        uint8_t Row2RRSeatBeltBlinking;
        uint8_t Row2RCSeatBelt;
        uint8_t Row2RCSeatBeltBlinking;
        uint8_t Row2RLSeatBelt;
        uint8_t Row2RLSeatBeltBlinking;

        //3E7
        uint8_t IsMaintenanceDue;
        uint8_t WrenchIcon;
        uint8_t MaintenanceSignKm;
        uint8_t MaintenanceSignKmBlinking;
        uint8_t MaintenanceSignTime;
        uint8_t MaintenanceSignTimeBlinking;
        UInt16 MaintenanceKilometers;
        UInt16 MaintenanceTime;

        //228
        uint8_t CruiseControlStatus;
        uint8_t CruiseControlAlert;
        //uint8_t CruiseControlSpeedUnit;
        uint8_t CruiseControlActivateFunction;
        uint8_t CruiseControlStatusOfSelectedFunction;
        uint8_t CruiseControlSelectedFunction;
        uint8_t CruiseSpeed1;
        uint8_t CruiseSpeed2;
        uint8_t CruiseByte1;
        uint8_t CruiseByte2;

        //361
        uint8_t AasDisable;
        uint8_t AmbientLighting;
        uint8_t AutoLighting;
        uint8_t AutomaticElectricBrake;
        uint8_t BlindspotMonitoring;
        uint8_t BreakingOnAlarmRisk;
        uint8_t ConfigEnabled;
        uint8_t DrlPresent;
        uint8_t FollowMeHome;
        uint8_t HighwayLightingPresent;
        uint8_t HingePanelSelect;
        uint8_t IrcPresent;
        uint8_t PermanentRearFlapLock;
        uint8_t RearWiperOption;
        uint8_t TnbPresent;
        uint8_t TpmsPresent;
        uint8_t TpmsResetPresent;
        //0E6
        uint8_t S_0E6Byte1;
        uint8_t S_0E6Byte2;
        uint8_t S_0E6Byte3;
        uint8_t S_0E6Byte4;
        uint8_t S_0E6Byte5;
        //161
        uint8_t S_161Byte1;
        uint8_t S_161Byte3;
        uint8_t S_161Byte4;
        uint8_t S_161Byte7;
        //1A1
        uint8_t S_1A1Byte1;
        uint8_t S_1A1Byte2;
        uint8_t S_1A1Byte3;
        uint8_t S_1A1Byte4;
        uint8_t S_1A1Byte5;
        uint8_t S_1A1Byte6;
        uint8_t S_1A1Byte7;
        uint8_t S_1A1Byte8;
        //21F
        uint8_t RemoteCommand1;
        uint8_t RemoteScrollPosition;
        uint8_t RemoteCommand3;
        //Door
        uint8_t DoorStatusByte;
        uint8_t IsFrontLeftDoorOpen;
        uint8_t IsFrontRightDoorOpen;
        uint8_t IsRearLeftDoorOpen;
        uint8_t IsRearRightDoorOpen;
        uint8_t IsBootLidOpen;
        uint8_t IsHoodOpen;
        uint8_t IsFuelFlapOpen;
        uint8_t IsRearWindowOpen;

        uint8_t BatteryChargeFault;

        uint8_t IsCan2004LogEnabled = 1;
        uint16_t ProcessedPackets;

        //UInt16 CruiseControlSpeed;
        uint8_t SpeedInKmh; //speed in km/h

        uint8_t Hour;
        uint8_t Minute;
        uint8_t Second;
        uint8_t MDay;
        uint8_t Month;
        uint16_t Year;

        unsigned long LastWebPageActivity = 0;
};

#endif
