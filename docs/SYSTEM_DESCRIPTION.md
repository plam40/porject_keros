# Universal HVAC Controller - System Description Document

**Project:** Keros - Universal HVAC Control System
**Platform:** ESP32-based Embedded System
**Version:** 1.0.0
**Last Updated:** 2025-11-18
**Status:** Design Phase

---

## Document Overview

This document provides a comprehensive system description for the Keros Universal HVAC Controller, an extensible, modular control system designed for sophisticated heating, ventilation, and air conditioning management. The system is built around the ESP32 microcontroller platform and follows the Universal Coding Standard & Architecture Framework.

### Document Structure

1. **Introduction & System Overview** ✓
2. **System Architecture** ✓
3. **Hardware Specifications** ✓
4. Control Logic & Algorithms
5. Heat Storage Management
6. User Interface Requirements
7. Modular Design & Extensibility
8. Communication Protocols
9. Safety & Monitoring
10. Installation & Deployment

---

## 1. Introduction & System Overview

### 1.1 Project Vision

The Keros Universal HVAC Controller is designed to be a comprehensive, future-proof solution for intelligent climate control in residential, commercial, and industrial applications. Unlike traditional HVAC controllers that are limited to specific equipment or configurations, Keros provides a flexible platform that can adapt to diverse heating and cooling systems through its modular architecture.

### 1.2 Core Objectives

#### 1.2.1 Universal Compatibility
- Support for multiple heat sources (heat pumps, boilers, solar thermal, electric heating)
- Compatible with various heat distribution systems (radiators, underfloor heating, air handling units)
- Flexible sensor integration (temperature, humidity, pressure, flow rate, power consumption)
- Extensible actuator control (valves, pumps, compressors, fans, dampers)

#### 1.2.2 Intelligent Control
- Advanced control algorithms optimizing energy efficiency
- Predictive control based on weather forecasts and occupancy patterns
- Multi-source heat coordination and prioritization
- Dynamic heat storage management
- Adaptive learning from system behavior and user preferences

#### 1.2.3 User Experience
- Intuitive web-based interface accessible from any device
- Mobile application for remote monitoring and control
- Visual system status and performance dashboards
- Simple configuration and commissioning process
- Comprehensive diagnostics and troubleshooting tools

#### 1.2.4 Extensibility & Modularity
- Plugin architecture for adding new functionality
- Support for third-party integrations
- Over-the-air (OTA) firmware updates
- Backward compatible module updates
- Easy hardware expansion through standardized interfaces

### 1.3 Key Features

#### 1.3.1 Heat Pump Control
- **Compressor Management**: Variable speed control, soft-start protection, temperature-based modulation
- **Coefficient of Performance (COP) Optimization**: Real-time efficiency monitoring and optimization
- **Defrost Cycle Management**: Intelligent defrost scheduling based on ambient conditions
- **Source Temperature Monitoring**: Ground source, air source, or water source monitoring
- **Heat Pump Cascade Control**: Support for multiple heat pumps in parallel or cascade configuration

#### 1.3.2 Ventilation Control
- **Heat Recovery Ventilation (HRV/ERV)**: Automatic mode selection based on indoor/outdoor conditions
- **Variable Air Volume (VAV)**: Dynamic airflow adjustment based on occupancy and air quality
- **Air Quality Monitoring**: CO₂, humidity, VOC, and particulate matter sensing
- **Demand-Controlled Ventilation**: Occupancy-based ventilation optimization
- **Bypass Control**: Summer bypass for free cooling

#### 1.3.3 Circulation Pump Management
- **Variable Speed Control**: PWM or 0-10V control for efficient operation
- **Solar Thermal Integration**: Differential temperature control for solar collectors
- **Multi-Zone Distribution**: Individual zone control with balancing
- **Anti-Legionella Protection**: Scheduled high-temperature cycles
- **Pump Protection**: Anti-seizing, dry-run protection, overheat monitoring

#### 1.3.4 Heat Storage Systems
- **Stratification Management**: Multi-level temperature monitoring for optimal layering
- **Charge/Discharge Optimization**: Intelligent scheduling based on energy pricing and demand
- **Storage Capacity Calculation**: Real-time available energy estimation
- **Multiple Storage Tanks**: Support for hot water, buffer tanks, and phase-change storage
- **Heat Pump Integration**: Optimized extraction for enhanced COP

#### 1.3.5 Multi-Source Heat Management
- **Priority-Based Selection**: Automatic source selection based on efficiency, cost, and availability
- **Hybrid Operation**: Simultaneous use of multiple sources when beneficial
- **Seamless Switching**: Smooth transitions between heat sources
- **Load Balancing**: Distribute demand across sources for optimal efficiency
- **Renewable Energy Prioritization**: Maximize use of solar, geothermal, or other renewables

### 1.4 System Capabilities

#### 1.4.1 Control Complexity
The system handles complex decision-making through:
- **Multi-variable optimization**: Simultaneously considering efficiency, comfort, cost, and equipment lifespan
- **Predictive algorithms**: Weather-compensated control with forecast integration
- **Machine learning**: Adaptive control based on historical performance data
- **Rule-based logic**: User-defined and pre-configured control strategies
- **Safety interlocks**: Comprehensive protection for equipment and occupants

#### 1.4.2 Monitoring & Diagnostics
- **Real-time Performance Metrics**: Energy consumption, efficiency ratios, temperature differentials
- **Historical Data Logging**: Long-term trend analysis and performance tracking
- **Anomaly Detection**: Automatic identification of unusual behavior or degraded performance
- **Fault Diagnostics**: Root cause analysis with suggested remediation
- **Performance Benchmarking**: Comparison to expected values and industry standards

#### 1.4.3 Energy Management
- **Energy Consumption Tracking**: Per-source, per-zone, and total system monitoring
- **Cost Calculation**: Integration with utility pricing for real-time cost awareness
- **Peak Demand Management**: Load shifting to avoid peak pricing periods
- **Renewable Energy Integration**: Solar PV, wind, or other on-site generation coordination
- **Grid Services**: Participation in demand response programs (where applicable)

### 1.5 Design Principles

The Keros system is built on these foundational principles:

#### 1.5.1 Safety First
- All control logic includes comprehensive safety checks
- Hardware interlocks for critical functions
- Fail-safe defaults (e.g., compressor shutdown on sensor failure)
- Temperature and pressure limit enforcement
- Emergency shutdown procedures

#### 1.5.2 Reliability & Robustness
- Watchdog timers for system health monitoring
- Graceful degradation when components fail
- Redundant sensors for critical measurements
- Automatic recovery from transient faults
- Non-volatile storage for critical settings

#### 1.5.3 Efficiency Optimization
- Continuous optimization of control parameters
- Minimization of start/stop cycles to reduce wear
- Deadband and hysteresis tuning for stability
- Seasonal adaptation of control strategies
- Equipment runtime balancing for even wear

#### 1.5.4 User-Centric Design
- Clear, understandable interface language
- Sensible defaults requiring minimal configuration
- Progressive disclosure of advanced features
- Contextual help and guidance
- Mobile-first responsive design

#### 1.5.5 Open & Extensible
- Well-documented APIs for third-party integration
- Modular architecture allowing custom extensions
- Standard protocols (MQTT, Modbus, HTTP REST)
- Open-source core components where possible
- Community-driven development and support

### 1.6 Target Applications

#### 1.6.1 Residential
- Single-family homes with heat pump systems
- Multi-zone radiant heating installations
- Solar thermal + backup heating combinations
- Whole-house ventilation with heat recovery
- Smart home integration (Home Assistant, OpenHAB, etc.)

#### 1.6.2 Commercial
- Small office buildings with multiple zones
- Retail spaces with varying occupancy patterns
- Restaurants requiring precise climate control
- Warehouses with mixed-use areas
- Educational facilities with scheduled occupancy

#### 1.6.3 Industrial
- Manufacturing facilities with process heat recovery
- Agricultural applications (greenhouses, livestock facilities)
- Data centers with waste heat utilization
- Food processing with strict temperature requirements
- Chemical plants with zone-specific climate needs

### 1.7 System Constraints & Assumptions

#### 1.7.1 Hardware Constraints
- **ESP32 Platform**: 240 MHz dual-core processor, 520 KB SRAM, 4+ MB flash
- **Real-time Requirements**: Control loops execute at defined intervals (1-10 seconds typical)
- **I/O Limitations**: Expansion required for systems with >32 I/O points
- **Power Supply**: Stable 5V or 12V DC supply required, backup battery optional
- **Operating Environment**: -10°C to +60°C, 0-95% RH non-condensing

#### 1.7.2 Software Constraints
- **Memory Management**: Efficient use of limited RAM for data logging
- **Processing Overhead**: Background tasks must not interfere with control loops
- **Storage Limitations**: Configuration and logs stored in flash with wear leveling
- **Network Dependency**: Core control functions operate without network connectivity
- **Update Safety**: OTA updates must not disrupt critical control functions

#### 1.7.3 Regulatory Compliance
- **Electrical Safety**: Compliance with local electrical codes and standards
- **EMC Requirements**: Electromagnetic compatibility per applicable regulations
- **Refrigerant Regulations**: Adherence to F-Gas or equivalent regulations
- **Data Privacy**: GDPR or equivalent data protection compliance
- **Building Codes**: Integration with building management requirements

### 1.8 Success Criteria

The Keros system will be considered successful if it achieves:

1. **Energy Efficiency**: 15-30% reduction in heating/cooling energy consumption vs. traditional controls
2. **Reliability**: >99.9% uptime with automatic recovery from common faults
3. **User Satisfaction**: Intuitive operation requiring <1 hour training for basic use
4. **Installation Time**: <4 hours for typical residential installation by qualified technician
5. **Compatibility**: Support for >80% of common HVAC equipment types
6. **Extensibility**: Addition of new modules without core firmware changes
7. **Performance**: Control loop execution within 10% of target timing
8. **Safety**: Zero safety incidents related to control system failures

---

## 2. System Architecture

### 2.1 Architectural Overview

The Keros system follows a **Skeleton + Module** architecture as defined in the Universal Coding Standards. This design provides a robust, extensible framework where a central "skeleton" manages system resources and coordinates independent, hot-swappable modules that implement specific functionality.

```
┌─────────────────────────────────────────────────────────────────┐
│                         USER INTERFACES                          │
│   ┌──────────────┐  ┌──────────────┐  ┌──────────────┐         │
│   │ Web Dashboard│  │ Mobile App   │  │ REST API     │         │
│   └──────┬───────┘  └──────┬───────┘  └──────┬───────┘         │
└──────────┼──────────────────┼──────────────────┼────────────────┘
           │                  │                  │
┌──────────┼──────────────────┼──────────────────┼────────────────┐
│          └──────────────────┴──────────────────┘                 │
│                    ┌────────────────────┐                        │
│                    │  SKELETON CORE     │                        │
│                    │  - Module Registry │                        │
│                    │  - Health Monitor  │                        │
│                    │  - Event Bus       │                        │
│                    │  - Config Manager  │                        │
│                    │  - Safety Engine   │                        │
│                    └─────────┬──────────┘                        │
│                              │                                   │
│          ┌───────────────────┴───────────────────┐              │
│          │        MODULE COMMUNICATION            │              │
│          │        (Event Bus / Message Queue)     │              │
│          └───────────────┬───────────────────────┘              │
│                          │                                       │
│  ┌──────────┬───────────┬┴──────────┬───────────┬─────────┐    │
│  │          │           │            │           │         │    │
│  ▼          ▼           ▼            ▼           ▼         ▼    │
│ ┌──┐      ┌──┐        ┌──┐        ┌──┐       ┌──┐      ┌──┐   │
│ │HP│      │VE│        │PM│        │HS│       │MS│      │TC│   │
│ │ M│      │NT│        │ P│        │ M│       │ C│      │ M│   │
│ └──┘      └──┘        └──┘        └──┘       └──┘      └──┘   │
│Heat      Vent     Pump        Heat       Multi-    Telemetry   │
│Pump      Module   Module      Storage    Source    & Cloud     │
│Module                         Module     Ctrl      Module       │
└─────────────────────────────────┬───────────────────────────────┘
                                  │
┌─────────────────────────────────┴───────────────────────────────┐
│                    HARDWARE ABSTRACTION LAYER                    │
│  ┌────────┐  ┌────────┐  ┌────────┐  ┌────────┐  ┌────────┐   │
│  │ GPIO   │  │ ADC    │  │ PWM    │  │ I2C    │  │ Modbus │   │
│  │ Driver │  │ Driver │  │ Driver │  │ Driver │  │ Driver │   │
│  └────────┘  └────────┘  └────────┘  └────────┘  └────────┘   │
└─────────────────────────────────┬───────────────────────────────┘
                                  │
┌─────────────────────────────────┴───────────────────────────────┐
│                        PHYSICAL I/O                              │
│ ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐        │
│ │Temp    │ │Pressure│ │Flow    │ │Relays  │ │Variable│        │
│ │Sensors │ │Sensors │ │Meters  │ │        │ │Speed   │        │
│ └────────┘ └────────┘ └────────┘ └────────┘ │Drives  │        │
│                                   └────────┘          │        │
└─────────────────────────────────────────────────────────────────┘
```

### 2.2 Skeleton Core Components

The skeleton provides essential system-wide services that all modules depend upon.

#### 2.2.1 Module Registry & Lifecycle Manager

**Responsibilities:**
- Module discovery and registration at startup
- Dependency resolution with circular detection
- Ordered module initialization based on dependencies
- Hot-reload support for development and updates
- Version compatibility checking
- Module state management (INIT, RUNNING, PAUSED, FAILED, SHUTDOWN)

**Module Lifecycle:**
```
DISCOVERED → REGISTERED → INITIALIZED → RUNNING → SHUTDOWN
                                ↕
                            PAUSED/FAILED
```

**Interface Contract:**
Every module must implement the `ModuleInterface`:
```cpp
class ModuleInterface {
public:
    virtual ModuleMetadata get_metadata() = 0;
    virtual HealthStatus health_check() = 0;
    virtual bool initialize(Configuration config) = 0;
    virtual void shutdown() = 0;
    virtual String get_api_version() = 0;
    virtual void loop() = 0;  // Called every control cycle
};
```

#### 2.2.2 Health Monitor & Diagnostics

**Continuous Monitoring:**
- Module health checks (periodic polling)
- System resource monitoring (memory, CPU, I/O)
- Watchdog timer management
- Crash detection and recovery
- Performance metrics collection
- Anomaly detection

**Health Check Levels:**
1. **CRITICAL**: Module failure requiring immediate isolation
2. **WARNING**: Degraded performance, continue with monitoring
3. **HEALTHY**: Normal operation
4. **UNKNOWN**: Unable to determine status

**Fault Isolation:**
When a module fails:
1. Module is immediately isolated from control loops
2. Dependent modules are notified
3. Fallback/safe-mode operation is activated
4. Detailed diagnostic log is created
5. User notification is triggered
6. Automatic recovery is attempted after cooldown period

#### 2.2.3 Event Bus & Message Queue

**Purpose:**
Provides loosely-coupled communication between modules without direct dependencies.

**Event Types:**
- **State Changes**: Temperature setpoint changed, mode switched, etc.
- **Measurements**: New sensor readings, calculated values
- **Commands**: User requests, scheduler triggers
- **Alerts**: Warnings, errors, safety events
- **Telemetry**: Performance metrics, energy consumption

**Message Format:**
```json
{
  "event_type": "TEMPERATURE_READING",
  "source_module": "sensor_module_01",
  "timestamp": 1700000000,
  "priority": "NORMAL",
  "data": {
    "sensor_id": "T_SUPPLY_HP1",
    "value": 45.3,
    "unit": "celsius"
  }
}
```

**Priority Levels:**
- **CRITICAL**: Safety events, emergency shutdowns (immediate processing)
- **HIGH**: Control commands, alarms (next cycle)
- **NORMAL**: Sensor readings, status updates (queued processing)
- **LOW**: Telemetry, logging (background processing)

#### 2.2.4 Configuration Manager

**Configuration Hierarchy:**
```
System Configuration
├── Hardware Configuration
│   ├── I/O Mapping
│   ├── Sensor Calibration
│   └── Actuator Settings
├── Module Configuration
│   ├── Heat Pump Module Config
│   ├── Ventilation Module Config
│   └── [Other Module Configs]
├── Control Parameters
│   ├── Setpoints
│   ├── Schedules
│   └── Logic Rules
└── User Preferences
    ├── UI Settings
    ├── Notifications
    └── Units
```

**Configuration Features:**
- Schema validation (JSON Schema)
- Automatic backup before changes
- Configuration versioning and migration
- Rollback capability
- Import/export functionality
- Audit logging of changes

**Storage:**
- Active config: SPIFFS/LittleFS (non-volatile flash)
- Backup configs: Compressed, timestamped archives
- Default config: Embedded in firmware

#### 2.2.5 Safety Engine

The safety engine operates independently with highest priority to protect equipment and occupants.

**Safety Checks (every control cycle):**
```cpp
// Pseudo-code for safety engine
void SafetyEngine::check() {
    // Temperature limits
    if (temp_supply > MAX_SUPPLY_TEMP) {
        emergency_shutdown("OVER_TEMP_SUPPLY");
    }

    // Pressure limits
    if (pressure_system > MAX_PRESSURE) {
        emergency_shutdown("OVER_PRESSURE");
    }

    // Interlock verification
    if (compressor_running && !flow_detected) {
        emergency_shutdown("NO_FLOW_INTERLOCK");
    }

    // Watchdog timeout
    if (millis() - last_loop_time > MAX_LOOP_TIME) {
        emergency_shutdown("WATCHDOG_TIMEOUT");
    }
}
```

**Safety Interlocks:**
- Compressor cannot start without circulation pump running
- High-temperature limit cutoff for all heat sources
- Low-temperature freeze protection
- Pressure relief monitoring
- Power failure detection with graceful shutdown
- Sensor fault detection with safe defaults

**Emergency Shutdown Procedure:**
1. Immediately disable all outputs (relays, PWM)
2. Log detailed fault information with timestamp
3. Activate visual/audible alarms
4. Send notifications (local display, network alerts)
5. Enter safe mode (manual recovery required)
6. Preserve system state for diagnostics

### 2.3 Module Architecture

Each module is a self-contained, independently testable component implementing specific functionality.

#### 2.3.1 Standard Module Structure

```
module_name/
├── module_name.h              # Public interface
├── module_name.cpp            # Implementation
├── module_name_config.h       # Module-specific configuration
├── module_name.interface      # Module interface definition (JSON)
└── README.md                  # Module documentation
```

**Module Interface File (.interface):**
```json
{
  "module_name": "heat_pump_controller",
  "version": "1.0.0",
  "api_version": "1.0.0",
  "dependencies": {
    "modules": ["sensor_module", "safety_module"],
    "hardware": ["GPIO", "ADC", "PWM"],
    "minimum_firmware": "1.0.0"
  },
  "capabilities": [
    "compressor_control",
    "defrost_management",
    "cop_optimization"
  ],
  "resource_requirements": {
    "ram_bytes": 8192,
    "flash_bytes": 32768,
    "loop_frequency_hz": 1
  },
  "configuration_schema": {
    "type": "object",
    "properties": {
      "compressor_pin": {"type": "integer"},
      "min_runtime_seconds": {"type": "integer", "default": 300}
    }
  }
}
```

#### 2.3.2 Core Modules

##### Heat Pump Module (HPM)
**Purpose:** Manages heat pump compressor, valves, and optimization

**Responsibilities:**
- Compressor on/off or variable speed control
- Defrost cycle management
- COP calculation and optimization
- Temperature differential monitoring
- Protection algorithms (min runtime, max cycles/hour)

**Inputs:**
- Supply temperature, return temperature
- Outdoor temperature
- Flow rate
- Power consumption

**Outputs:**
- Compressor control signal
- Reversing valve (for cooling)
- Defrost initiation

##### Ventilation Module (VENT)
**Purpose:** Controls ventilation system with heat recovery

**Responsibilities:**
- Supply/exhaust fan speed control
- Heat recovery bypass management
- Air quality based modulation
- Filter maintenance reminders
- Frost protection for heat exchanger

**Inputs:**
- Indoor/outdoor temperature and humidity
- CO₂, VOC, PM2.5 levels
- Occupancy sensors

**Outputs:**
- Supply fan speed (PWM/0-10V)
- Exhaust fan speed
- Bypass damper position

##### Pump Module (PMP)
**Purpose:** Manages all circulation pumps

**Responsibilities:**
- Variable speed pump control
- Differential pressure/flow control
- Zone valve coordination
- Anti-seizing protection
- Dry-run detection

**Inputs:**
- Flow meters
- Differential pressure sensors
- Zone demand signals

**Outputs:**
- Pump speed control (PWM/0-10V)
- Zone valve positions

##### Heat Storage Module (HSM)
**Purpose:** Optimizes heat storage charging and discharging

**Responsibilities:**
- Multi-level temperature monitoring
- Available capacity calculation
- Charge/discharge scheduling
- Stratification preservation
- Anti-legionella cycles

**Inputs:**
- Temperature sensors (multiple levels)
- Energy price data
- Demand forecast

**Outputs:**
- Charging pump control
- Discharging valve control
- Priority signals to heat sources

##### Multi-Source Controller (MSC)
**Purpose:** Coordinates multiple heat sources for optimal efficiency

**Responsibilities:**
- Source priority selection (COP, cost, availability)
- Load distribution
- Seamless source switching
- Renewable energy maximization
- Backup source management

**Inputs:**
- All source efficiency data
- Energy pricing
- Weather forecast
- Demand profile

**Outputs:**
- Source enable/disable commands
- Load sharing percentages
- Operating mode selection

##### Telemetry & Cloud Module (TCM)
**Purpose:** External communication, logging, and cloud integration

**Responsibilities:**
- MQTT publishing for smart home integration
- Historical data logging
- Cloud synchronization
- Weather forecast retrieval
- Remote monitoring/control API

**Inputs:**
- All system data (sensors, states, performance)

**Outputs:**
- MQTT messages
- HTTP API responses
- Cloud data uploads

### 2.4 Hardware Abstraction Layer (HAL)

The HAL provides a consistent interface to hardware peripherals, enabling portability and testability.

#### 2.4.1 Supported Peripherals

**Digital I/O:**
- Relay control (on/off outputs)
- Digital input sensing (float switches, safety contacts)
- Interrupt-driven inputs for flow meters

**Analog Inputs:**
- Temperature sensors (NTC thermistors, PT1000, thermocouples)
- Pressure transducers (0-10V, 4-20mA)
- Current sensors (SCT-013, etc.)

**PWM Outputs:**
- Variable speed pump control
- Fan speed control
- Modulating valve control

**Communication Buses:**
- **I2C**: Environmental sensors (BME280, SCD40, etc.)
- **SPI**: SD card, display modules
- **UART/Modbus RTU**: External devices (energy meters, BMS)
- **1-Wire**: DS18B20 temperature sensors

**Wireless:**
- **WiFi**: Local network, internet connectivity
- **Bluetooth**: Mobile app pairing, local control

#### 2.4.2 HAL Interface Example

```cpp
// Temperature sensor HAL
class TemperatureSensor {
public:
    virtual float read_celsius() = 0;
    virtual bool is_available() = 0;
    virtual SensorError get_last_error() = 0;
};

// Concrete implementation for NTC thermistor
class NTCThermistor : public TemperatureSensor {
private:
    uint8_t adc_pin;
    float resistance_at_25c;
    float beta_coefficient;

public:
    float read_celsius() override {
        // ADC reading, Steinhart-Hart equation
    }
};
```

### 2.5 Data Flow

#### 2.5.1 Control Loop Execution

```
┌─────────────────── MAIN CONTROL LOOP (1 second cycle) ────────────────┐
│                                                                         │
│  1. Safety Engine Pre-Check                                           │
│     └─→ Verify all critical limits, interlocks                        │
│                                                                         │
│  2. Sensor Reading Phase                                              │
│     └─→ HAL reads all sensors                                         │
│     └─→ Sensor modules publish readings to event bus                  │
│                                                                         │
│  3. Module Processing Phase (priority order)                          │
│     └─→ Safety Module (highest priority)                              │
│     └─→ Heat Storage Module (calculate capacity)                      │
│     └─→ Multi-Source Controller (select sources)                      │
│     └─→ Heat Pump Module (control compressor)                         │
│     └─→ Pump Module (adjust flow)                                     │
│     └─→ Ventilation Module (adjust airflow)                           │
│                                                                         │
│  4. Output Update Phase                                               │
│     └─→ Modules command outputs (relays, PWM, etc.)                   │
│     └─→ HAL writes to hardware                                        │
│                                                                         │
│  5. Telemetry & Logging                                               │
│     └─→ Telemetry module logs data                                    │
│     └─→ Cloud sync (if connected)                                     │
│                                                                         │
│  6. Safety Engine Post-Check                                          │
│     └─→ Verify outputs within safe ranges                             │
│                                                                         │
│  7. Health Monitoring                                                  │
│     └─→ Check module health                                            │
│     └─→ Update system status                                           │
│                                                                         │
└─────────────────────────────────────────────────────────────────────────┘
```

#### 2.5.2 Event-Driven Communication

Modules communicate asynchronously via the event bus:

```cpp
// Module publishes event
void HeatPumpModule::loop() {
    float cop = calculate_cop();

    Event event;
    event.type = "COP_CALCULATED";
    event.source = "heat_pump_module";
    event.data["cop"] = cop;
    event.data["power_kw"] = power_consumption;
    event.data["heat_output_kw"] = heat_output;

    skeleton.publish_event(event);
}

// Multi-Source Controller subscribes to COP events
void MultiSourceController::on_event(Event& event) {
    if (event.type == "COP_CALCULATED") {
        float cop = event.data["cop"];
        update_source_efficiency("heat_pump", cop);
        recalculate_source_priority();
    }
}
```

### 2.6 Software Stack

```
┌────────────────────────────────────────────────┐
│         Application Layer                      │
│  - Modules (HPM, VENT, PMP, HSM, MSC, TCM)    │
└────────────────┬───────────────────────────────┘
                 │
┌────────────────┴───────────────────────────────┐
│         Skeleton Core                          │
│  - Module Registry                             │
│  - Event Bus                                   │
│  - Safety Engine                               │
│  - Configuration Manager                       │
└────────────────┬───────────────────────────────┘
                 │
┌────────────────┴───────────────────────────────┐
│    Hardware Abstraction Layer (HAL)            │
│  - GPIO, ADC, PWM Drivers                      │
│  - I2C, SPI, UART Drivers                      │
└────────────────┬───────────────────────────────┘
                 │
┌────────────────┴───────────────────────────────┐
│         ESP-IDF / Arduino Framework            │
│  - FreeRTOS                                    │
│  - WiFi Stack                                  │
│  - File System (SPIFFS/LittleFS)               │
└────────────────┬───────────────────────────────┘
                 │
┌────────────────┴───────────────────────────────┐
│         ESP32 Hardware                         │
└────────────────────────────────────────────────┘
```

### 2.7 Timing & Real-Time Constraints

#### 2.7.1 Task Priorities (FreeRTOS)

| Task | Priority | Period | Deadline |
|------|----------|--------|----------|
| Safety Monitor | 10 (highest) | 100ms | 100ms |
| Critical Control (Compressor) | 8 | 1s | 1s |
| Normal Control (Pumps, Valves) | 6 | 1s | 2s |
| Sensor Reading | 5 | 1s | 1s |
| Telemetry/Logging | 3 | 10s | 60s |
| UI/API Server | 2 | On-demand | 500ms |
| Cloud Sync | 1 (lowest) | 60s | No hard deadline |

#### 2.7.2 Memory Allocation

Estimated memory usage:

| Component | RAM (KB) | Flash (KB) |
|-----------|----------|------------|
| ESP-IDF/FreeRTOS | 50 | 500 |
| Skeleton Core | 30 | 100 |
| Heat Pump Module | 8 | 32 |
| Ventilation Module | 6 | 24 |
| Pump Module | 6 | 24 |
| Heat Storage Module | 10 | 40 |
| Multi-Source Controller | 12 | 48 |
| Telemetry Module | 15 | 60 |
| Web UI (SPIFFS) | - | 200 |
| Configuration/Logs | 20 | 100 |
| **Total Estimated** | **157 KB** | **1128 KB** |
| **ESP32 Available** | 520 KB | 4096 KB |
| **Margin** | **363 KB (70%)** | **2968 KB (72%)** |

### 2.8 Extensibility Mechanisms

#### 2.8.1 Adding New Modules

1. Create module implementing `ModuleInterface`
2. Create `.interface` file describing capabilities
3. Place in `/modules/` directory
4. Skeleton auto-discovers at next boot
5. User configures via UI

#### 2.8.2 Module Versioning

Modules use semantic versioning (MAJOR.MINOR.PATCH):
- **MAJOR**: Breaking changes to module interface
- **MINOR**: New features, backward compatible
- **PATCH**: Bug fixes

Skeleton enforces compatibility:
```cpp
bool is_compatible(ModuleVersion required, ModuleVersion available) {
    // Major version must match exactly
    if (required.major != available.major) return false;

    // Available minor must be >= required
    if (available.minor < required.minor) return false;

    return true;
}
```

#### 2.8.3 Hot Module Reload (Development Mode)

For development and testing:
1. Module is paused (removed from control loop)
2. Shutdown method called
3. New version loaded
4. Initialize method called
5. Health check performed
6. Module resumed if healthy

**Safety:** Hot reload disabled in production mode for critical modules.

---

## 3. Hardware Specifications

### 3.1 Core Controller Platform

#### 3.1.1 ESP32 Microcontroller

**Selected Model:** ESP32-WROOM-32 or ESP32-WROVER (with PSRAM for advanced features)

**Key Specifications:**
- **CPU**: Dual-core Xtensa LX6, 240 MHz
- **RAM**: 520 KB SRAM (WROOM) / 520 KB + 4-8 MB PSRAM (WROVER)
- **Flash**: 4 MB minimum, 8-16 MB recommended
- **WiFi**: 802.11 b/g/n, 2.4 GHz
- **Bluetooth**: BLE 4.2 and Classic Bluetooth
- **Operating Voltage**: 3.3V (with integrated voltage regulation from 5V)
- **Operating Temperature**: -40°C to +85°C (industrial grade)
- **Power Consumption**:
  - Active (WiFi): ~160-260 mA
  - Modem sleep: ~20-68 mA
  - Deep sleep: ~10 µA

**Peripheral Interfaces:**
- **GPIO**: 34 programmable pins (some strapping/input-only)
- **ADC**: 18 channels, 12-bit resolution (0-3.3V)
- **DAC**: 2 channels, 8-bit
- **Touch**: 10 capacitive touch sensors
- **PWM**: 16 channels via LED PWM controller
- **UART**: 3 ports
- **SPI**: 4 controllers
- **I2C**: 2 controllers
- **I2S**: 2 channels (for audio or high-speed data)
- **CAN**: 1 controller (via TWAI)

#### 3.1.2 Justification for ESP32

**Advantages:**
1. **Dual-core architecture**: Separate cores for real-time control and network/UI tasks
2. **Rich peripheral set**: Sufficient I/O for typical HVAC installations without expansion
3. **Wireless connectivity**: Built-in WiFi and BLE eliminate need for external modules
4. **Mature ecosystem**: Well-supported by ESP-IDF and Arduino frameworks
5. **Cost-effective**: ~$3-5 per unit in volume
6. **Low power options**: Deep sleep modes for battery backup scenarios
7. **Industrial temperature range**: Suitable for mechanical room installations
8. **Large community**: Extensive libraries and support

**Limitations & Mitigations:**
1. **Limited ADC accuracy**: Use external ADC (ADS1115) for critical measurements
2. **3.3V logic**: Level shifters required for 5V/12V/24V industrial sensors
3. **GPIO count**: Use I/O expanders (MCP23017) for systems requiring >30 I/O points
4. **No native 4-20mA support**: External interface circuits required

### 3.2 I/O Architecture

#### 3.2.1 Input Types

##### Temperature Sensors

**Primary Method: 1-Wire DS18B20**
- **Advantages**: Digital, calibrated, multiple sensors on one bus, up to 127 devices
- **Range**: -55°C to +125°C
- **Accuracy**: ±0.5°C (-10°C to +85°C)
- **Resolution**: 9-12 bit (0.5°C to 0.0625°C)
- **Interface**: 1-Wire protocol (one GPIO per bus, can daisy-chain)
- **Typical Use**: Supply/return temperatures, zone temperatures, tank levels

**Alternative: NTC Thermistors (10K, 3950 beta)**
- **Advantages**: Low cost, widely available
- **Interface**: Voltage divider → ADC
- **Accuracy**: ±1-2°C with calibration
- **Typical Use**: Non-critical temperature sensing

**Alternative: PT1000 RTD**
- **Advantages**: High accuracy, industrial standard
- **Interface**: Precision ADC (ADS1118, MAX31865)
- **Accuracy**: ±0.15°C (Class A)
- **Typical Use**: High-precision applications, regulatory compliance

##### Pressure Sensors

**Standard: 0-10V or 4-20mA Transducers**
- **Range**: Application specific (0-6 bar, 0-10 bar typical)
- **Interface**:
  - 0-10V: Voltage divider → ADC
  - 4-20mA: Precision resistor (250Ω) → ADC or dedicated receiver
- **Accuracy**: ±0.5-1% full scale
- **Typical Use**: System pressure, differential pressure, refrigerant pressure

**Digital Alternative: I2C Pressure Sensors (MS5837)**
- **Advantages**: Direct digital reading, high accuracy
- **Range**: Application specific
- **Typical Use**: Compact installations, prototype development

##### Flow Meters

**Pulse-Based Flow Meters**
- **Interface**: Digital input with interrupt
- **Output**: Frequency proportional to flow rate (pulses/liter)
- **Typical Use**: Heat meter integration, consumption monitoring
- **Processing**: ESP32 PCNT (Pulse Counter) peripheral

**Analog Flow Meters**
- **Interface**: 4-20mA or 0-10V → ADC
- **Typical Use**: Continuous flow monitoring

##### Air Quality Sensors (for Ventilation Module)

**CO₂ Sensor: SCD40 or MH-Z19**
- **Interface**: I2C (SCD40) or UART (MH-Z19)
- **Range**: 0-5000 ppm
- **Accuracy**: ±50 ppm typical
- **Typical Use**: Demand-controlled ventilation

**VOC Sensor: BME680 or SGP40**
- **Interface**: I2C
- **Output**: IAQ (Indoor Air Quality) index or TVOC ppb
- **Typical Use**: Air quality monitoring, ventilation control

**Particulate Matter: PMS5003 or SPS30**
- **Interface**: UART or I2C
- **Output**: PM1.0, PM2.5, PM10 concentrations
- **Typical Use**: Advanced ventilation control, filter monitoring

##### Digital Inputs

**Dry Contact Inputs** (thermostats, safety switches, float switches)
- **Interface**: Optocoupled input, pull-up resistor
- **Voltage Levels**: 5-24V AC/DC compatible
- **Typical Use**: External enable signals, safety interlocks, zone demand

**Binary Sensors** (reed switches, limit switches)
- **Interface**: Direct GPIO with internal pull-up
- **Typical Use**: Valve position feedback, access door sensors

#### 3.2.2 Output Types

##### Relay Outputs

**Solid State Relays (SSR) or Mechanical Relays**
- **Configuration**:
  - On-board: 4-8 relays rated for inductive loads
  - External: Via I2C relay modules for expansion
- **Ratings**:
  - SSR: 2A @ 240V AC (typical)
  - Mechanical: 10A @ 250V AC / 30V DC
- **Typical Use**:
  - Compressor enable (via contactor)
  - Backup heater control
  - Zone valves (ON/OFF type)
  - Circulation pumps (fixed speed)

**Protection:**
- Snubber circuits for inductive loads
- Optical isolation between logic and power
- LED indicators per relay
- Fuse or resettable fuse (PTC) per channel

##### PWM Outputs (0-10V or Direct PWM)

**0-10V Control Signal**
- **Generation**: PWM → Low-pass filter → Voltage follower (op-amp)
- **Accuracy**: ±0.1V typical
- **Load**: 1mA minimum (typical VFD input impedance)
- **Typical Use**:
  - Variable speed pump control
  - Fan speed control
  - Modulating valve control

**Direct PWM (High Current)**
- **Drive**: MOSFET driver circuit
- **Frequency**: 25 kHz (above audible range)
- **Current**: Up to 2A per channel
- **Typical Use**:
  - Direct pump control (PWM-capable circulators)
  - DC fan control

##### Analog Outputs (4-20mA)

**Current Loop Transmitter**
- **Circuit**: DAC or PWM → Voltage-to-current converter
- **Accuracy**: ±0.2 mA
- **Compliance Voltage**: 24V typical
- **Typical Use**: Industrial actuators, legacy BMS integration

##### High-Power Switching

**Compressor Control**
- **Method**: Relay output drives contactor (24V AC coil typical)
- **Interlock**: Safety circuit ensures proper conditions before engagement
- **Protection**: Minimum on/off times enforced in software

**Three-Phase Equipment**
- **Method**: Relay outputs to motor starters or VFDs
- **Safety**: Phase monitoring, overload protection via external devices

### 3.3 Reference Hardware Design

#### 3.3.1 Minimal Configuration (Small Residential)

**Use Case:** Single heat pump + buffer tank + 2 zones

**I/O Requirements:**
- **Temperature Inputs**: 6 (outdoor, supply, return, tank top/mid/bottom, 2x zones)
- **Digital Inputs**: 2 (safety thermostat, flow switch)
- **Relay Outputs**: 3 (compressor, pump primary, pump secondary)
- **PWM Outputs**: 1 (modulating valve or pump speed)

**Bill of Materials (BOM) - Core:**
| Component | Qty | Description |
|-----------|-----|-------------|
| ESP32-WROOM-32 | 1 | Main controller |
| DS18B20 | 6 | Temperature sensors |
| 4.7kΩ resistor | 1 | 1-Wire pull-up |
| SSR or Relay Module | 1 | 4-channel, optocoupled |
| 0-10V Output Circuit | 1 | PWM → voltage converter |
| 24V AC/DC Power Supply | 1 | DIN rail mount, 15W |
| 5V DC-DC Converter | 1 | Buck converter for ESP32 |
| Enclosure | 1 | DIN rail mountable, IP20 |
| Terminal Blocks | 1 set | Screw terminals for wiring |

**Estimated Cost (Prototype Quantities):** ~$50-70 USD

#### 3.3.2 Standard Configuration (Typical Residential/Small Commercial)

**Use Case:** Heat pump + backup heater + ventilation + solar thermal + 4 zones + heat storage

**I/O Requirements:**
- **Temperature Inputs**: 16 (extensive monitoring)
- **Pressure Inputs**: 2 (system pressure, differential)
- **Flow Inputs**: 2 (heat meter, solar circuit)
- **Air Quality Inputs**: 3 (CO₂, VOC, humidity)
- **Digital Inputs**: 4 (safety switches, zone demands)
- **Relay Outputs**: 8 (compressor, pumps, valves, backup heater)
- **PWM Outputs**: 4 (pump speeds, fan speed, modulating valve)

**Bill of Materials (BOM) - Core:**
| Component | Qty | Description |
|-----------|-----|-------------|
| ESP32-WROVER | 1 | Main controller (with PSRAM) |
| DS18B20 | 16 | Temperature sensors |
| ADS1115 | 1 | 16-bit ADC for precision analog inputs |
| SCD40 | 1 | CO₂ + temperature + humidity sensor |
| BME680 | 1 | VOC + pressure + temp + humidity |
| MCP23017 | 1 | I2C I/O expander (16 additional GPIOs) |
| SSR Module | 2 | 4-channel each (8 total) |
| 0-10V Output Circuit | 4 | For variable speed control |
| 4-20mA Input Circuit | 2 | Pressure transducer interface |
| Pulse Counter Input | 2 | Optocoupled, for flow meters |
| 24V AC/DC Power Supply | 1 | DIN rail mount, 30W |
| 5V DC-DC Converter | 1 | Buck converter, 2A |
| SD Card Module | 1 | For local data logging |
| RTC Module (DS3231) | 1 | Real-time clock with battery backup |
| OLED Display (Optional) | 1 | 128x64, I2C, for local status |
| Enclosure | 1 | DIN rail mountable, IP20 or IP54 |
| Terminal Blocks | 1 set | Screw terminals |

**Estimated Cost (Prototype Quantities):** ~$120-150 USD

#### 3.3.3 Advanced Configuration (Large Commercial/Industrial)

**Use Case:** Multiple heat sources, complex zoning, advanced features

**Expansion via:**
- **Multiple MCP23017 I/O Expanders**: Up to 64 additional GPIOs
- **Modbus RTU**: Integration with existing BMS, energy meters, VFDs
- **CAN Bus**: Industrial equipment communication
- **Ethernet Module (W5500)**: For installations requiring wired network

**Additional Hardware:**
- External watchdog timer (TPL5010)
- UPS or battery backup for graceful shutdown
- Industrial-grade components (wider temperature range)

### 3.4 Power Supply Architecture

#### 3.4.1 Power Distribution

```
24V AC/DC Supply (from HVAC control transformer or dedicated PSU)
  │
  ├──→ 5V DC-DC Buck Converter (2A)
  │     │
  │     ├──→ ESP32 (500mA typical)
  │     ├──→ Sensors (I2C, 1-Wire: ~100mA)
  │     ├──→ Display & SD Card (~50mA)
  │     └──→ Signal Conditioning Circuits (~50mA)
  │
  └──→ 24V Relay Coils / Actuators
        └──→ Individual fused channels
```

**Power Budget:**
| Component | Current @ 5V | Current @ 24V |
|-----------|--------------|---------------|
| ESP32 (active WiFi) | 250 mA | - |
| Sensors & Peripherals | 150 mA | - |
| Display & SD Card | 50 mA | - |
| Safety Margin | 50 mA | - |
| **Total 5V** | **500 mA** | - |
| Relay Coils (8x @ 20mA) | - | 160 mA |
| Actuators (varies) | - | ~500 mA |
| **Total 24V** | - | **~700 mA** |

**Power Supply Selection:**
- 5V Rail: 5V 2A DC-DC converter (buck) from 24V
- 24V Rail: 24V AC/DC supply, 30W (1.25A)

#### 3.4.2 Power Protection

**Features:**
- Reverse polarity protection (Schottky diode or MOSFET)
- Overvoltage protection (TVS diodes)
- Brownout detection (ESP32 monitors input voltage)
- Graceful shutdown on power loss (supercapacitor bank for ~5 seconds hold-up)
- Resettable fuses (PTC) on each output channel

### 3.5 Signal Conditioning

#### 3.5.1 Analog Input Conditioning

**Temperature (NTC Thermistor):**
```
VCC (3.3V) ──┬──[ 10kΩ ]──┬── ADC Input
              │            │
              │         [NTC 10K]
              │            │
             GND ─────────┴── GND
```

**Voltage Input (0-10V → 0-3.3V):**
```
0-10V Input ──[ R1: 20kΩ ]──┬── ADC Input
                             │
                         [ R2: 10kΩ ]
                             │
                            GND
```
(Voltage divider ratio: 1:3, maps 0-10V to 0-3.3V)

**Current Input (4-20mA → 0-3.3V):**
```
4-20mA Input ──[ 165Ω ]──┬── ADC Input
                          │
                         GND
```
(4mA × 165Ω = 0.66V, 20mA × 165Ω = 3.3V)

**Filtering:**
- RC low-pass filter on all analog inputs (fc = 10-100 Hz)
- Software averaging (moving average, 10 samples typical)

#### 3.5.2 PWM Output Conditioning (PWM → 0-10V)

```
ESP32 PWM ──[ R1: 10kΩ ]──┬── C1 (10µF) ──┬─┬─ Op-Amp Buffer ─→ 0-10V Output
                          GND             │ │
                                        [R2:10kΩ] (to ground)
                                          │
                                     ADC for feedback
```

**Op-Amp Requirements:**
- Rail-to-rail output
- Low offset voltage
- Example: MCP6002, TLV272

#### 3.5.3 Digital Input Conditioning (Optocoupler)

```
24V External ──[ R: 2.2kΩ ]──┬── Optocoupler LED
                              │
                       External Ground

Optocoupler Transistor:
  Collector ──[ Pull-up 10kΩ ]── 3.3V
  Collector ────────────────────── GPIO Input
  Emitter ──────────────────────── GND
```

**Isolation:** 2.5 kV typical (optocoupler rating)

### 3.6 Expansion Options

#### 3.6.1 I/O Expansion

**I2C I/O Expander (MCP23017)**
- 16 additional GPIOs per chip
- Up to 8 devices per I2C bus (128 GPIOs total)
- Interrupt capability for input monitoring
- Use Case: Large zoning systems, extensive sensor arrays

**I2C Relay Modules**
- 4-16 relays per module
- Optical isolation
- Stackable/chainable

#### 3.6.2 Communication Expansion

**Modbus RTU (RS485)**
- **Interface**: MAX485 or similar transceiver
- **Use Case**: Integration with energy meters, VFDs, existing BMS
- **Topology**: Multi-drop bus, up to 32 devices
- **Termination**: 120Ω resistors at both ends

**Ethernet (W5500 Module)**
- **Interface**: SPI
- **Use Case**: Installations requiring wired network
- **Advantage**: More reliable than WiFi in industrial environments

**CAN Bus (TWAI on ESP32)**
- **Interface**: MCP2551 or TJA1050 transceiver
- **Use Case**: Industrial equipment integration
- **Topology**: Multi-drop bus, up to 110 devices

#### 3.6.3 Display Options

**Local Display:**
- OLED (SSD1306): 128x64, I2C, low power, good readability
- TFT LCD (ILI9341): 240x320, SPI, color touchscreen
- E-Paper: Ultra-low power, excellent readability, slow refresh

**Use Case:** Local status display, basic configuration, fault indication

### 3.7 Environmental & Mechanical

#### 3.7.1 Operating Environment

**Temperature Range:**
- Standard: 0°C to +60°C (typical mechanical room)
- Industrial: -10°C to +70°C (with industrial-grade components)
- Extended: -40°C to +85°C (requires specialized parts)

**Humidity:**
- 0-95% RH, non-condensing
- Conformal coating recommended for high-humidity environments

**Enclosure Rating:**
- **IP20**: Indoor installation, protected against fingers
- **IP54**: Dustproof, splash-proof (for harsh environments)
- **DIN Rail Mounting**: Standard 35mm DIN rail

#### 3.7.2 Wiring & Installation

**Sensor Wiring:**
- 1-Wire (DS18B20): CAT5/CAT6 cable, up to 100m with proper topology
- Analog Sensors: Shielded twisted pair, shield grounded at controller end
- Digital Inputs: 2-wire, 18-22 AWG

**Power Wiring:**
- Low Voltage (<50V): 18-22 AWG, comply with local codes
- Line Voltage (>50V): Must be installed by licensed electrician
- Separation: Maintain 50mm minimum spacing between low and line voltage

**Connector Types:**
- Screw terminals: 22-14 AWG wire
- Plug-in terminal blocks: For easy service/replacement
- RJ45/RJ11: For sensor "home runs" in structured wiring

### 3.8 Reference Schematics

#### 3.8.1 Core Controller Board

```
┌─────────────────────────────────────────────────────────────────┐
│                       ESP32 CONTROLLER                           │
│                                                                   │
│  ┌──────────┐   ┌──────────┐   ┌──────────┐   ┌──────────┐    │
│  │  ESP32   │   │ ADS1115  │   │ MCP23017 │   │  DS3231  │    │
│  │ WROVER   │◄─►│  16-bit  │   │   I/O    │   │   RTC    │    │
│  │          │   │   ADC    │   │ Expander │   │          │    │
│  └────┬─────┘   └──────────┘   └──────────┘   └──────────┘    │
│       │                                                          │
│       │         ┌──────────┐   ┌──────────┐   ┌──────────┐    │
│       │         │  SD Card │   │   OLED   │   │  WiFi/BT │    │
│       └────────►│  Logger  │   │  Display │   │ Antenna  │    │
│                 └──────────┘   └──────────┘   └──────────┘    │
│                                                                  │
│  INPUT SECTION:                                                  │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ 1-Wire Bus (DS18B20 x16)                                 │  │
│  │ Analog In (0-10V / 4-20mA) via ADS1115 x4                │  │
│  │ Digital In (Optocoupled) x8                              │  │
│  │ Pulse Counter (Flow meters) x2                           │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                  │
│  OUTPUT SECTION:                                                 │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │ SSR/Relay Outputs (via driver, optical isolation) x8     │  │
│  │ PWM → 0-10V (via filter + op-amp) x4                     │  │
│  │ 4-20mA Output (via V-to-I converter) x2 (optional)       │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                  │
│  POWER:                                                          │
│  24V AC/DC In → Buck Converter → 5V 2A → ESP32 + Peripherals   │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

*(Note: Full detailed schematics to be provided in hardware design documentation)*

#### 3.8.2 Pin Assignment Example

**ESP32 GPIO Allocation (Reference):**

| GPIO | Function | Direction | Notes |
|------|----------|-----------|-------|
| 0 | Boot Mode | Input | Pull-up, bootstrap |
| 1 | UART0 TX | Output | Debug/programming |
| 2 | Built-in LED | Output | Status indicator |
| 3 | UART0 RX | Input | Debug/programming |
| 4 | 1-Wire Bus | I/O | DS18B20 sensors |
| 5 | Relay 1 | Output | Via driver |
| 12 | Relay 2 | Output | Via driver |
| 13 | Relay 3 | Output | Via driver |
| 14 | Relay 4 | Output | Via driver |
| 15 | PWM Out 1 | Output | For 0-10V conversion |
| 16 | PWM Out 2 | Output | For 0-10V conversion |
| 17 | PWM Out 3 | Output | For 0-10V conversion |
| 18 | SPI SCK | Output | SD card, display |
| 19 | SPI MISO | Input | SD card |
| 21 | I2C SDA | I/O | Sensors, expanders |
| 22 | I2C SCL | Output | Sensors, expanders |
| 23 | SPI MOSI | Output | SD card, display |
| 25 | Digital In 1 | Input | Optocoupled |
| 26 | Digital In 2 | Input | Optocoupled |
| 27 | Digital In 3 | Input | Optocoupled |
| 32 | Digital In 4 | Input | Optocoupled |
| 33 | Pulse In 1 | Input | Flow meter (PCNT) |
| 34 | Pulse In 2 | Input | Flow meter (PCNT) |
| 35 | ADC Direct 1 | Input | Voltage sense |
| 36 | ADC Direct 2 | Input | Voltage sense |

**Reserved/Special Pins:**
- GPIO 6-11: Connected to internal flash (do not use)
- GPIO 34-39: Input-only (no pull-up/pull-down)

### 3.9 Certification & Compliance

#### 3.9.1 Electrical Safety

**Standards:**
- IEC 60730: Automatic electrical controls for household and similar use
- UL 60730-1: Safety standard for control devices (North America)
- EN 60730: European standard for control devices

**Key Requirements:**
- Clearance and creepage distances between line and low voltage
- Insulation testing (Hi-Pot test)
- Protective earth (PE) connection for metal enclosures
- Fusing on all line voltage outputs

#### 3.9.2 EMC (Electromagnetic Compatibility)

**Standards:**
- EN 61000-6-1: Immunity for residential environments
- EN 61000-6-2: Immunity for industrial environments
- EN 61000-6-3: Emissions for residential environments
- EN 61000-6-4: Emissions for industrial environments

**Design Considerations:**
- Shielded enclosure with proper grounding
- Filtering on all inputs/outputs crossing enclosure boundary
- Suppression components on relay coils (snubbers, flyback diodes)
- Twisted pair wiring for differential signals
- Ferrite beads on cables

#### 3.9.3 Radio (WiFi/BLE)

**Standards:**
- FCC Part 15 (USA)
- CE RED (Europe)
- IC (Canada)

**Compliance:**
- ESP32 modules with pre-certified radio (FCC/CE markings)
- Antenna design per module manufacturer specifications
- Final product testing recommended for commercial deployment

---

## Document Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0.0 | 2025-11-18 | System Architect | Initial release - Section 1: Introduction & System Overview |
| 1.0.1 | 2025-11-18 | System Architect | Added Section 2: System Architecture |
| 1.0.2 | 2025-11-18 | System Architect | Added Section 3: Hardware Specifications |

---

**Next Section:** Control Logic & Algorithms (Coming soon)
