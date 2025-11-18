/**
 * @file HAL.cpp
 * @purpose Implementation of Hardware Abstraction Layer
 * @dependencies HAL.h, Arduino, Wire, OneWire, DallasTemperature
 * @version 1.0.0
 * @performance_notes Optimized for ESP32 hardware peripherals
 */

#include "HAL.h"
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Global OneWire and DallasTemperature instances
static OneWire* oneWire = nullptr;
static DallasTemperature* dallasSensors = nullptr;

bool HAL::initialize() {
    if (initialized_) {
        return true;
    }

    Serial.println("[HAL] Initializing Hardware Abstraction Layer...");

    // Initialize all PWM channels as unused
    for (int i = 0; i < 16; i++) {
        pwm_channels_[i].in_use = false;
        pwm_channels_[i].pin = 255;
        pwm_channels_[i].resolution = 8;
        pwm_channels_[i].frequency = 5000;
    }

    // Set ADC width to 12-bit (0-4095)
    analogReadResolution(12);

    // Set ADC attenuation to 11dB (0-3.3V range) for all ADC pins
    analogSetAttenuation(ADC_11db);

    initialized_ = true;
    Serial.println("[HAL] Initialization complete");

    return true;
}

// ========== GPIO Functions ==========

void HAL::pin_mode(uint8_t pin, PinMode mode) {
    switch (mode) {
        case PinMode::INPUT:
            pinMode(pin, INPUT);
            break;
        case PinMode::INPUT_PULLUP:
            pinMode(pin, INPUT_PULLUP);
            break;
        case PinMode::INPUT_PULLDOWN:
            pinMode(pin, INPUT_PULLDOWN);
            break;
        case PinMode::OUTPUT:
            pinMode(pin, OUTPUT);
            break;
        case PinMode::OUTPUT_OPEN_DRAIN:
            pinMode(pin, OUTPUT_OPEN_DRAIN);
            break;
    }
}

void HAL::digital_write(uint8_t pin, bool value) {
    digitalWrite(pin, value ? HIGH : LOW);
    pin_states_[pin] = value;
}

bool HAL::digital_read(uint8_t pin) {
    return digitalRead(pin) == HIGH;
}

void HAL::digital_toggle(uint8_t pin) {
    bool current = pin_states_[pin];
    digital_write(pin, !current);
}

// ========== Analog Functions ==========

uint16_t HAL::analog_read(uint8_t pin, uint8_t samples) {
    if (samples == 1) {
        return analogRead(pin);
    }

    // Average multiple samples for noise reduction
    uint32_t sum = 0;
    for (uint8_t i = 0; i < samples; i++) {
        sum += analogRead(pin);
        if (i < samples - 1) {
            delayMicroseconds(100);  // Small delay between samples
        }
    }

    return sum / samples;
}

float HAL::analog_read_voltage(uint8_t pin, uint8_t samples) {
    uint16_t raw = analog_read(pin, samples);
    // Convert 12-bit value (0-4095) to voltage (0-3.3V)
    return (raw / 4095.0f) * 3.3f;
}

void HAL::analog_set_attenuation(uint8_t pin, AnalogReference ref) {
    adc_attenuation_t atten;

    switch (ref) {
        case AnalogReference::ATTEN_0DB:
            atten = ADC_0db;
            break;
        case AnalogReference::ATTEN_2_5DB:
            atten = ADC_2_5db;
            break;
        case AnalogReference::ATTEN_6DB:
            atten = ADC_6db;
            break;
        case AnalogReference::ATTEN_11DB:
        case AnalogReference::DEFAULT_3V3:
        default:
            atten = ADC_11db;
            break;
    }

    // Note: analogSetPinAttenuation is channel-specific on ESP32
    analogSetPinAttenuation(pin, atten);
}

// ========== PWM Functions ==========

bool HAL::pwm_init(uint8_t pin, uint8_t channel, uint32_t frequency, uint8_t resolution) {
    if (channel >= 16) {
        Serial.printf("[HAL] ERROR: Invalid PWM channel %d (max 15)\n", channel);
        return false;
    }

    if (pwm_channels_[channel].in_use && pwm_channels_[channel].pin != pin) {
        Serial.printf("[HAL] WARNING: PWM channel %d already in use by pin %d\n",
                     channel, pwm_channels_[channel].pin);
    }

    // Configure PWM channel
    ledcSetup(channel, frequency, resolution);
    ledcAttachPin(pin, channel);

    // Update channel info
    pwm_channels_[channel].pin = pin;
    pwm_channels_[channel].resolution = resolution;
    pwm_channels_[channel].frequency = frequency;
    pwm_channels_[channel].in_use = true;

    Serial.printf("[HAL] PWM initialized: pin=%d, channel=%d, freq=%dHz, res=%d-bit\n",
                 pin, channel, frequency, resolution);

    return true;
}

void HAL::pwm_write(uint8_t channel, uint8_t duty_percent) {
    if (channel >= 16 || !pwm_channels_[channel].in_use) {
        Serial.printf("[HAL] ERROR: PWM channel %d not initialized\n", channel);
        return;
    }

    // Clamp duty cycle to 0-100%
    if (duty_percent > 100) duty_percent = 100;

    // Calculate raw duty value based on resolution
    uint8_t resolution = pwm_channels_[channel].resolution;
    uint32_t max_duty = (1 << resolution) - 1;  // 2^resolution - 1
    uint32_t duty_value = (duty_percent * max_duty) / 100;

    ledcWrite(channel, duty_value);
}

void HAL::pwm_write_raw(uint8_t channel, uint32_t duty_value) {
    if (channel >= 16 || !pwm_channels_[channel].in_use) {
        Serial.printf("[HAL] ERROR: PWM channel %d not initialized\n", channel);
        return;
    }

    ledcWrite(channel, duty_value);
}

void HAL::pwm_set_frequency(uint8_t channel, uint32_t frequency) {
    if (channel >= 16 || !pwm_channels_[channel].in_use) {
        Serial.printf("[HAL] ERROR: PWM channel %d not initialized\n", channel);
        return;
    }

    ledcWriteTone(channel, frequency);
    pwm_channels_[channel].frequency = frequency;
}

// ========== OneWire / DS18B20 Functions ==========

bool HAL::onewire_init(uint8_t pin) {
    if (oneWire != nullptr) {
        delete dallasSensors;
        delete oneWire;
    }

    oneWire = new OneWire(pin);
    dallasSensors = new DallasTemperature(oneWire);

    dallasSensors->begin();
    onewire_pin_ = pin;

    int device_count = dallasSensors->getDeviceCount();
    Serial.printf("[HAL] OneWire initialized on pin %d, found %d devices\n",
                 pin, device_count);

    return true;
}

std::vector<uint64_t> HAL::onewire_scan() {
    std::vector<uint64_t> addresses;

    if (!dallasSensors) {
        Serial.println("[HAL] ERROR: OneWire not initialized");
        return addresses;
    }

    DeviceAddress device_addr;
    oneWire->reset_search();

    while (oneWire->search(device_addr)) {
        // Convert 8-byte address to uint64_t
        uint64_t addr = 0;
        for (int i = 0; i < 8; i++) {
            addr |= ((uint64_t)device_addr[i]) << (i * 8);
        }
        addresses.push_back(addr);

        Serial.printf("[HAL] Found OneWire device: %016llX\n", addr);
    }

    return addresses;
}

float HAL::read_ds18b20(uint64_t address) {
    if (!dallasSensors) {
        Serial.println("[HAL] ERROR: OneWire not initialized");
        return -127.0f;
    }

    // Convert uint64_t back to DeviceAddress (8 bytes)
    DeviceAddress device_addr;
    for (int i = 0; i < 8; i++) {
        device_addr[i] = (address >> (i * 8)) & 0xFF;
    }

    // Request temperature
    dallasSensors->requestTemperaturesByAddress(device_addr);

    // Wait for conversion (could be optimized with async reads)
    delay(10);  // DS18B20 takes ~750ms max, but we can poll earlier

    // Read temperature
    float temp_c = dallasSensors->getTempC(device_addr);

    if (temp_c == DEVICE_DISCONNECTED_C) {
        Serial.printf("[HAL] ERROR: DS18B20 sensor %016llX disconnected\n", address);
        return -127.0f;
    }

    return temp_c;
}

std::map<uint64_t, float> HAL::read_all_ds18b20() {
    std::map<uint64_t, float> temperatures;

    if (!dallasSensors) {
        Serial.println("[HAL] ERROR: OneWire not initialized");
        return temperatures;
    }

    // Request temperatures from all sensors
    dallasSensors->requestTemperatures();

    // Get all device addresses
    std::vector<uint64_t> addresses = onewire_scan();

    // Read each sensor
    for (uint64_t addr : addresses) {
        DeviceAddress device_addr;
        for (int i = 0; i < 8; i++) {
            device_addr[i] = (addr >> (i * 8)) & 0xFF;
        }

        float temp_c = dallasSensors->getTempC(device_addr);
        if (temp_c != DEVICE_DISCONNECTED_C) {
            temperatures[addr] = temp_c;
        }
    }

    return temperatures;
}

// ========== I2C Functions ==========

bool HAL::i2c_init(uint8_t sda_pin, uint8_t scl_pin, uint32_t frequency) {
    if (i2c_initialized_) {
        Wire.end();
    }

    Wire.begin(sda_pin, scl_pin);
    Wire.setClock(frequency);

    i2c_initialized_ = true;

    Serial.printf("[HAL] I2C initialized: SDA=%d, SCL=%d, freq=%dHz\n",
                 sda_pin, scl_pin, frequency);

    return true;
}

std::vector<uint8_t> HAL::i2c_scan() {
    std::vector<uint8_t> devices;

    if (!i2c_initialized_) {
        Serial.println("[HAL] ERROR: I2C not initialized");
        return devices;
    }

    Serial.println("[HAL] Scanning I2C bus...");

    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            devices.push_back(addr);
            Serial.printf("[HAL] Found I2C device at 0x%02X\n", addr);
        }
    }

    Serial.printf("[HAL] I2C scan complete, found %d devices\n", devices.size());

    return devices;
}

bool HAL::i2c_write_byte(uint8_t address, uint8_t reg, uint8_t value) {
    if (!i2c_initialized_) {
        return false;
    }

    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
}

bool HAL::i2c_read_byte(uint8_t address, uint8_t reg, uint8_t* value) {
    if (!i2c_initialized_) {
        return false;
    }

    Wire.beginTransmission(address);
    Wire.write(reg);
    if (Wire.endTransmission() != 0) {
        return false;
    }

    if (Wire.requestFrom(address, (uint8_t)1) != 1) {
        return false;
    }

    *value = Wire.read();
    return true;
}

bool HAL::i2c_read_bytes(uint8_t address, uint8_t reg, uint8_t* buffer, uint8_t length) {
    if (!i2c_initialized_) {
        return false;
    }

    Wire.beginTransmission(address);
    Wire.write(reg);
    if (Wire.endTransmission() != 0) {
        return false;
    }

    if (Wire.requestFrom(address, length) != length) {
        return false;
    }

    for (uint8_t i = 0; i < length; i++) {
        buffer[i] = Wire.read();
    }

    return true;
}

// ========== Utility Functions ==========

float HAL::get_chip_temperature() {
    // ESP32 internal temperature sensor (approximate)
    // Note: This is not very accurate and varies by chip
    return temperatureRead();
}

int HAL::get_hall_sensor() {
    // ESP32 Hall effect sensor
    return hallRead();
}

bool HAL::is_pin_valid(uint8_t pin, PinMode mode) {
    // ESP32 pin restrictions
    // Input only pins: 34-39
    // Strapping pins (be careful): 0, 2, 5, 12, 15
    // Flash pins (don't use): 6-11
    // UART0: 1 (TX), 3 (RX)

    if (pin >= 40) {
        return false;  // Invalid pin number
    }

    // Flash pins - never use
    if (pin >= 6 && pin <= 11) {
        return false;
    }

    // Input only pins
    if (pin >= 34 && pin <= 39) {
        return mode == PinMode::INPUT;
    }

    return true;
}
