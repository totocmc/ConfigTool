#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

/** AM32 / tool EEPROM image (248 bytes). Keep in sync with EEPROM_DATA_SIZE in defaults.h. */
#if defined(_MSC_VER) && !defined(__clang__)
#pragma pack(push, 1)
#define EEPROM_U_PACKED_SUFFIX
#else
#define EEPROM_U_PACKED_SUFFIX __attribute__((__packed__))
#endif
typedef union EEprom_u {
    struct {
        uint8_t reserved_0;       // 0 boot / start marker
        uint8_t eeprom_version;   // 1
        uint8_t reserved_1;       // 2 bootloader version
        struct {
            uint8_t major;       // 3
            uint8_t minor;       // 4
        } version;
        char firmware_name[12];   // 5–16
        uint8_t dir_reversed;    // 17
        uint8_t bi_direction;    // 18
        uint8_t use_sine_start;  // 19
        uint8_t comp_pwm;        // 20
        uint8_t variable_pwm;   // 21
        uint8_t stuck_rotor_protection; // 22
        uint8_t advance_level;   // 23
        uint8_t pwm_frequency;   // 24
        uint8_t startup_power;   // 25
        uint8_t motor_kv;        // 26
        uint8_t motor_poles;     // 27
        uint8_t brake_on_stop;   // 28
        uint8_t stall_protection; // 29
        uint8_t beep_volume;     // 30
        uint8_t telemetry_on_interval; // 31
        struct {
            uint8_t low_threshold;  // 32
            uint8_t high_threshold;  // 33
            uint8_t neutral;         // 34
            uint8_t dead_band;       // 35
        } servo;
        uint8_t low_voltage_cut_off;   // 36
        uint8_t low_cell_volt_cutoff;  // 37
        uint8_t rc_car_reverse;        // 38
        uint8_t use_hall_sensors;      // 39
        uint8_t sine_mode_changeover_thottle_level; // 40
        uint8_t drag_brake_strength;   // 41
        uint8_t driving_brake_strength; // 42
        struct {
            uint8_t temperature; // 43
            uint8_t current;     // 44
        } limits;
        uint8_t sine_mode_power; // 45
        uint8_t input_type;      // 46
        uint8_t auto_advance;    // 47
        uint8_t tune[128];       // 48–175
        struct {
            uint8_t can_node;              // 176
            uint8_t esc_index;             // 177
            uint8_t require_arming;        // 178
            uint8_t telem_rate;            // 179
            uint8_t require_zero_throttle; // 180
            uint8_t filter_hz;             // 181
            uint8_t debug_rate;            // 182
            uint8_t term_enable;           // 183
            uint8_t reserved[8];           // 184–191
        } can;
        struct {
            uint8_t min_rpm;               // 192  value × 100
            uint8_t max_rpm;               // 193
            uint16_t flight_time;          // 194–195  seconds
            uint8_t landed_wait;          // 196
            uint8_t speed_target;         // 197  %
            uint16_t softstart_floor_rpm; // 198–199
            uint8_t accel_pct_per_sec;    // 200  VCC throttle ramp %/s (1–100)
            uint8_t reserved[47];         // 201–247
        } vcc;
    };
    uint8_t buffer[248];
} EEPROM_U_PACKED_SUFFIX EEprom_t;

#if defined(_MSC_VER) && !defined(__clang__)
#pragma pack(pop)
#endif
#undef EEPROM_U_PACKED_SUFFIX

static_assert(sizeof(EEprom_t) == 248, "EEprom_t must be 248 bytes");

/** Copy raw bytes into typed layout (zero-pad if short). */
inline void eeprom_from_bytes(const uint8_t* src, std::size_t src_len, EEprom_t* out)
{
    std::memset(out, 0, sizeof(*out));
    if (src != nullptr && src_len > 0) {
        std::size_t n = src_len < sizeof(EEprom_t) ? src_len : sizeof(EEprom_t);
        std::memcpy(out, src, n);
    }
}

/** Serialize typed layout to raw bytes (dst must hold at least sizeof(EEprom_t)). */
inline void eeprom_to_bytes(const EEprom_t* in, uint8_t* dst)
{
    std::memcpy(dst, in, sizeof(EEprom_t));
}
