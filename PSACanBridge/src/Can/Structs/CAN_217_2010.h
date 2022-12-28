// CAN_217_2010.h
#pragma once

#ifndef _CAN_217_2010_h
    #define _CAN_217_2010_h

#include <stdint.h>

const uint16_t CAN_217_2010_INTERVAL = 100;

// CANID: 217
const uint16_t CAN_ID_217_2010 = 0x217;

// Read right to left in documentation
union CAN_217_2010_Byte1Struct {
    struct {
        uint8_t airbag_disabled    : 2; // bit 0-1
        uint8_t reset_maintenance  : 1; // bit 2
        uint8_t black_panel        : 1; // bit 3
        uint8_t brightness         : 4; // bit 4-7
    }data;
  uint8_t asByte;
};

// Read right to left in documentation
union CAN_217_2010_Byte2Struct {
    struct {
        uint8_t distance_unit      : 1; // bit 0
        uint8_t reset_trip_partial : 1; // bit 1
        uint8_t reset_trip1        : 1; // bit 2
        uint8_t cmb_fault          : 1; // bit 3
        uint8_t check_auto         : 1; // bit 4
        uint8_t check              : 1; // bit 5
        uint8_t airbag_fault       : 2; // bit 6-7
    }data;
  uint8_t asByte;
};

// Read right to left in documentation
union CAN_217_2010_Byte3Struct {
    struct {
        uint8_t                    : 1; // bit 0
        uint8_t                    : 1; // bit 1
        uint8_t filter_pushed      : 1; // bit 2
        uint8_t check_matt         : 1; // bit 3
        uint8_t amvar_push         : 1; // bit 4
        uint8_t child_lock_push    : 1; // bit 5
        uint8_t esp_pushed         : 1; // bit 6
        uint8_t aas_pushed         : 1; // bit 7
    }data;
  uint8_t asByte;
};

// Read right to left in documentation
union CAN_217_2010_Byte4Struct {
    struct {
        uint8_t reset_trip2        : 1; // bit 0
        uint8_t max_compressor     : 1; // bit 1
        uint8_t mpd_push           : 1; // bit 2
        uint8_t sam_push           : 1; // bit 3
        uint8_t luch_push          : 1; // bit 4
        uint8_t clim_push          : 1; // bit 5
        uint8_t stl_push           : 1; // bit 6
        uint8_t cafr_push          : 1; // bit 7
    }data;
  uint8_t asByte;
};

// Read right to left in documentation
union CAN_217_2010_Byte5Struct {
    struct {
        uint8_t scp_status_change  : 1; // bit 0
        uint8_t display_fault_arc  : 1; // bit 1
        uint8_t artiv_minus        : 1; // bit 2
        uint8_t artiv_plus         : 1; // bit 3
        uint8_t matt_reset         : 1; // bit 4
        uint8_t artiv_push         : 1; // bit 5
        uint8_t ctp_activation     : 1; // bit 6
        uint8_t dsgi_push          : 1; // bit 7
    }data;
  uint8_t asByte;
};

// Read right to left in documentation
union CAN_217_2010_Byte7Struct {
    struct {
        uint8_t ambience_level       : 3; // bit 0-2
        uint8_t amibience_push       : 1; // bit 3
        uint8_t display_fault        : 1; // bit 4
        uint8_t dynamic_mode_request : 1; // bit 5
        uint8_t eco_mode_request     : 1; // bit 6
        uint8_t stt_push             : 1; // bit 7
    }data;
  uint8_t asByte;
};

// Read left to right in documentation
struct CAN_217Struct {
    CAN_217_2010_Byte1Struct Brightness;
    CAN_217_2010_Byte2Struct Field2;
    uint8_t Field3;
    uint8_t Field4;
    uint8_t Field5;
    uint8_t VehicleSpeed;
    uint8_t AmbienceLevel;
    uint8_t CMBApperance;
};
#endif
