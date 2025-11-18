# HVAC Controller - ESP32 Based Heat Pump Control System

A comprehensive, modular HVAC control system built for ESP32 microcontrollers. This project implements a professional-grade heat pump controller following the Skeleton + Module architecture pattern.

## 🎯 Project Overview

This HVAC controller provides intelligent management of:
- Heat pump operation (compressor, COP optimization, defrost)
- Circulation pumps (variable speed, multi-zone)
- Heat storage systems (stratification management)
- Ventilation systems (HRV/ERV, air quality)
- Multi-source heat management
- Humidity control

## 📋 Current Implementation Status

### ✅ Completed Components

#### Core Skeleton System
- **Module Registry**: Lifecycle management for all system modules
- **Event Bus**: Decoupled publish-subscribe messaging between modules
- **Safety Engine**: Real-time safety monitoring with FreeRTOS task
- **Config Manager**: JSON-based configuration with versioning and backup
- **Skeleton Core**: Main system coordinator

#### Hardware Abstraction Layer (HAL)
- GPIO control (digital I/O, pull-up/pull-down)
- Analog input (ADC with averaging and calibration)
- PWM output (LED control, motor control)
- OneWire bus (DS18B20 temperature sensors)
- I2C bus (sensors and peripherals)
- Utility functions (chip temperature, Hall sensor)

#### Modules
- **TemperatureSensorModule**: DS18B20 sensor management
  - Auto-discovery of sensors on OneWire bus
  - Calibration support with offset adjustment
  - Event publishing on temperature changes
  - Health monitoring and fault detection

- **PumpModule**: Variable speed circulation pump control
  - Multiple pump support with independent control
  - PWM-based speed control (0-100%)
  - Current monitoring for fault detection
  - Soft start to reduce mechanical stress
  - Minimum on/off time protection
  - Dry running and overcurrent detection

- **HeatPumpModule**: Heat pump control with advanced features
  - Compressor control with protection timers
  - COP (Coefficient of Performance) monitoring
  - Automatic defrost cycle management
  - Multiple operating modes (heating, cooling, defrost)
  - Temperature-based safety limits
  - Outdoor temperature compensation

- **VentilationModule**: HRV/ERV ventilation control
  - Supply and exhaust fan control
  - Variable speed operation
  - Multiple operating modes (off, low, medium, high, boost, auto)
  - Runtime tracking

- **HeatStorageModule**: Heat storage tank management
  - Multi-tank support
  - Temperature stratification monitoring
  - Charge percentage calculation
  - Stored energy calculation (kWh)
  - State detection (idle, charging, discharging, full, empty)

### 🚧 Pending Components

The following modules are specified in the system documentation but not yet implemented:
- Web Server Module (REST API, WebSocket, web dashboard)
- MQTT Integration Module (smart home connectivity)
- Modbus Communication Module (industrial equipment integration)
- Advanced COP optimization algorithms
- Predictive maintenance features
- Energy usage analytics

## 🏗️ Architecture

This project follows the **Skeleton + Module** pattern defined in `CODING_STANDARDS.md`:

```
┌─────────────────────────────────────────┐
│           Application Modules           │
│  (HeatPump, Pumps, Sensors, etc.)      │
├─────────────────────────────────────────┤
│          Skeleton Core System           │
│  • Module Registry                      │
│  • Event Bus                            │
│  • Safety Engine                        │
│  • Config Manager                       │
├─────────────────────────────────────────┤
│     Hardware Abstraction Layer (HAL)    │
│  (GPIO, ADC, PWM, OneWire, I2C)        │
├─────────────────────────────────────────┤
│     ESP-IDF / Arduino Framework         │
├─────────────────────────────────────────┤
│            FreeRTOS RTOS                │
├─────────────────────────────────────────┤
│         ESP32 Hardware Platform         │
└─────────────────────────────────────────┘
```

## 📁 Project Structure

```
porject_keros/
├── CODING_STANDARDS.md      # Universal coding framework and standards
├── docs/
│   └── SYSTEM_DESCRIPTION.md # Complete HVAC system specification
├── platformio.ini           # PlatformIO build configuration
├── include/                 # Header files
│   ├── ModuleInterface.h    # Base interface for all modules
│   ├── EventBus.h           # Event-driven messaging system
│   ├── SafetyEngine.h       # Safety monitoring and fault detection
│   ├── ConfigManager.h      # Configuration management
│   ├── Skeleton.h           # Main skeleton coordinator
│   ├── HAL.h                # Hardware Abstraction Layer
│   └── modules/             # Module headers
│       └── TemperatureSensorModule.h
├── src/                     # Source files
│   ├── main.cpp             # Application entry point
│   ├── EventBus.cpp
│   ├── SafetyEngine.cpp
│   ├── ConfigManager.cpp
│   ├── Skeleton.cpp
│   ├── HAL.cpp
│   └── modules/
│       └── TemperatureSensorModule.cpp
├── lib/                     # Custom libraries (none yet)
├── test/                    # Unit tests (pending)
├── deploy/                  # Deployment scripts (pending)
├── config/                  # Configuration files
└── data/                    # SPIFFS data files
```

## 🚀 Getting Started

### Prerequisites

- [PlatformIO](https://platformio.org/) installed
- ESP32 development board (ESP32-WROOM-32 or ESP32-WROVER)
- USB cable for programming
- VS Code with PlatformIO extension (recommended)

### Hardware Setup

**Minimum Required Pins:**
- GPIO 4: OneWire bus for DS18B20 temperature sensors
- GPIO 21: I2C SDA (optional, for future sensors)
- GPIO 22: I2C SCL (optional, for future sensors)

**Avoid These Pins:**
- GPIO 6-11: Connected to flash memory
- GPIO 34-39: Input only
- GPIO 0, 2: Strapping pins (use with caution)

### Building and Uploading

```bash
# Clone the repository
git clone <repository-url>
cd porject_keros

# Build the project
platformio run

# Upload to ESP32
platformio run --target upload

# Monitor serial output
platformio device monitor --baud 115200
```

### First Boot

On first boot, the system will:
1. Initialize the SPIFFS filesystem
2. Create default configuration files
3. Scan for temperature sensors
4. Start the safety monitoring task
5. Enter main control loop

Expected serial output:
```
====================================
  HVAC Controller Starting...
====================================
Chip Model: ESP32-D0WDQ6
...
[Skeleton] Initializing HVAC Controller Skeleton...
[ConfigManager] Initialized (version 1.0.0, ...)
[SafetyEngine] Initialized successfully
[TempSensor] Found 2 sensors on bus
...
System Ready - Entering Main Loop
```

## ⚙️ Configuration

Configuration is stored in `/config/system.json` on the SPIFFS filesystem.

### Default Configuration

The system creates a default configuration with safe values:
- Maximum supply temperature: 65°C
- Maximum return temperature: 50°C
- Maximum tank temperature: 80°C
- Minimum outdoor temperature: -30°C
- Safety pressure limits: 0.5-3.0 bar

### Modifying Configuration

Configuration can be modified via:
1. Web interface (pending implementation)
2. Direct file editing in SPIFFS
3. ConfigManager API in code

Example:
```cpp
ConfigManager& config = ConfigManager::get_instance();
config.set_float(ConfigSection::HEAT_PUMP, "max_supply_temp", 70.0);
config.save();
```

## 🔧 Development

### Adding a New Module

1. **Create header file** in `include/modules/YourModule.h`:
```cpp
#include "ModuleInterface.h"

class YourModule : public ModuleInterface {
public:
    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override;
};
```

2. **Implement in** `src/modules/YourModule.cpp`

3. **Register in** `src/main.cpp`:
```cpp
#include "modules/YourModule.h"

void setup() {
    static YourModule your_module;
    system.register_module(&your_module, 50);  // Priority 50
    system.initialize();
}
```

### Module Guidelines

All modules must:
- Inherit from `ModuleInterface`
- Implement all pure virtual methods
- Initialize within 5 seconds
- Health check within 100ms
- Shutdown within 2 seconds
- Leave hardware in safe state on shutdown

See `CODING_STANDARDS.md` for complete requirements.

## 📊 System Monitoring

### Health Checks

The SafetyEngine monitors all modules every 100ms:
- Module health percentage (0-100%)
- Watchdog timeout detection
- Safety violation tracking
- Automatic recovery attempts

### Event System

Modules communicate via events:
```cpp
// Subscribe to temperature events
EventBus::get_instance().subscribe(
    EventType::TEMPERATURE_CHANGED,
    [](const EventData* data) {
        auto temp = static_cast<const TemperatureEventData*>(data);
        Serial.printf("Temp: %.2f°C\n", temp->temperature_c);
    }
);

// Publish events
TemperatureEventData event;
event.sensor_id = "tank_top";
event.temperature_c = 45.5;
EventBus::get_instance().publish(EventType::TEMPERATURE_CHANGED, &event);
```

### Available Events

- `SYSTEM_STARTUP` / `SYSTEM_SHUTDOWN`
- `TEMPERATURE_CHANGED` / `TEMPERATURE_THRESHOLD_EXCEEDED`
- `SAFETY_VIOLATION` / `EMERGENCY_SHUTDOWN`
- `CONFIG_CHANGED`
- See `EventBus.h` for complete list

## 🔒 Safety Features

### Multi-Level Safety System

1. **Normal** (Level 0): Normal operation
2. **Warning** (Level 1): Log warning, attempt auto-recovery
3. **Degraded** (Level 2): Reduce functionality
4. **Critical** (Level 3): Graceful shutdown
5. **Emergency** (Level 4): Immediate hardware shutdown

### Safety Limits

Configurable limits in `SafetyEngine`:
- Temperature limits (supply, return, tank, outdoor)
- Pressure limits (min/max system pressure)
- Current limits (compressor, pumps, total)
- Voltage limits (min/max supply voltage)
- Module health thresholds
- Watchdog timeout

### Emergency Shutdown

GPIO 25 is configured as emergency shutdown relay:
- Active LOW (shutdown)
- HIGH = normal operation
- Triggered on EMERGENCY safety level

## 📝 Memory Usage

Current memory allocation (estimated):

| Component | RAM (KB) | Flash (KB) |
|-----------|----------|------------|
| ESP-IDF/FreeRTOS | 50 | 500 |
| Skeleton Core | 30 | 100 |
| EventBus | 10 | 20 |
| SafetyEngine | 15 | 30 |
| ConfigManager | 12 | 25 |
| HAL | 8 | 35 |
| TemperatureSensorModule | 8 | 20 |
| PumpModule | 12 | 25 |
| HeatPumpModule | 15 | 35 |
| VentilationModule | 5 | 15 |
| HeatStorageModule | 10 | 20 |
| **Total Used** | **175 KB** | **825 KB** |
| **ESP32 Available** | 520 KB | 4096 KB |
| **Safety Margin** | **66%** | **80%** |

## 📚 Documentation

- **CODING_STANDARDS.md**: Universal coding framework and requirements
- **docs/SYSTEM_DESCRIPTION.md**: Complete HVAC system specification (128KB, 3927 lines)
- **Header files**: Comprehensive inline documentation with examples

## 🧪 Testing

Testing infrastructure is pending implementation. Planned:
- Unit tests (>80% coverage requirement)
- Integration tests (module interfaces)
- System tests (end-to-end workflows)
- Performance tests (no regression)
- Hardware-in-the-loop tests

## 📦 Dependencies

Managed by PlatformIO:
- `OneWire@^2.3.7` - OneWire protocol
- `DallasTemperature@^3.11.0` - DS18B20 sensors
- `Adafruit BME280@^2.2.4` - Environmental sensors
- `AsyncTCP@^1.1.4` - Async networking
- `ESP Async WebServer@^1.2.4` - Web server
- `ArduinoJson@^7.2.1` - JSON parsing
- `PubSubClient@^2.8` - MQTT client

## 🤝 Contributing

This project follows strict coding standards defined in `CODING_STANDARDS.md`:
- Skeleton + Module architecture (mandatory)
- >80% test coverage required
- Comprehensive documentation required
- Performance profiling required
- Single-command deployment

## 📄 License

[License information to be added]

## 🔗 References

- [ESP32 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)

## 🚀 Roadmap

### Phase 1: Core Infrastructure ✅ (COMPLETED)
- ✅ Skeleton + Module architecture
- ✅ Event bus communication
- ✅ Safety monitoring system
- ✅ Configuration management
- ✅ Hardware Abstraction Layer
- ✅ Temperature sensor module

### Phase 2: Control Modules ✅ (COMPLETED)
- ✅ Pump control module
- ✅ Heat pump module
- ✅ Ventilation module
- ✅ Heat storage module

### Phase 3: User Interface
- ⏳ Web server with REST API
- ⏳ Web dashboard (HTML/CSS/JS)
- ⏳ Mobile app (future)

### Phase 4: Integration
- ⏳ MQTT integration
- ⏳ Modbus support
- ⏳ Cloud connectivity
- ⏳ OTA updates

### Phase 5: Testing & Deployment
- ⏳ Unit test suite
- ⏳ Integration tests
- ⏳ Hardware-in-the-loop testing
- ⏳ Deployment automation
- ⏳ Production hardening

---

**Built with ❤️ for efficient, reliable HVAC control**
