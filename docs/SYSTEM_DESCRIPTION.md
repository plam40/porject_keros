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
3. Hardware Specifications
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

## Document Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0.0 | 2025-11-18 | System Architect | Initial release - Section 1: Introduction & System Overview |
| 1.0.1 | 2025-11-18 | System Architect | Added Section 2: System Architecture |

---

**Next Section:** Hardware Specifications (Coming soon)
