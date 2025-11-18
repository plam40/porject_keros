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
4. **Control Logic & Algorithms** ✓
5. **Heat Storage Management** ✓
6. **User Interface Requirements** ✓
7. **Modular Design & Extensibility** ✓
8. **Communication Protocols** ✓
9. **Safety & Monitoring** ✓
10. **Installation & Deployment** ✓

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

#### 1.3.6 Humidity Control & Dehumidification
- **Multi-Zone Humidity Monitoring**: Individual zone humidity tracking with high-precision sensors
- **Active Dehumidification**: Integration with dedicated dehumidifiers, heat pump cooling, or ERV systems
- **Humidification Control**: Coordination with steam or evaporative humidifiers for optimal comfort
- **Condensation Prevention**: Dew point monitoring to prevent condensation on cold surfaces
- **Seasonal Adjustment**: Automatic humidity setpoint adaptation based on outdoor conditions
- **Health & Comfort Optimization**: Maintain optimal 40-60% RH range for health and comfort
- **Mold Prevention**: Continuous monitoring and control to prevent high-humidity conditions
- **Integration with Ventilation**: Coordinated control between humidity and ventilation systems

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
│  ┌────────┬──────────┬──┴────┬──────────┬──────────┬──────────┬────┐
│  │        │          │       │          │          │          │    │
│  ▼        ▼          ▼       ▼          ▼          ▼          ▼    │
│ ┌──┐    ┌──┐      ┌──┐    ┌──┐      ┌──┐      ┌──┐      ┌──┐    │
│ │HP│    │VE│      │PM│    │HS│      │MS│      │HC│      │TC│    │
│ │ M│    │NT│      │ P│    │ M│      │ C│      │ M│      │ M│    │
│ └──┘    └──┘      └──┘    └──┘      └──┘      └──┘      └──┘    │
│Heat    Vent    Pump    Heat     Multi-   Humidity Telemetry    │
│Pump    Module  Module  Storage  Source   Control  & Cloud      │
│Module                  Module   Ctrl     Module   Module        │
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

##### Humidity Control Module (HCM)
**Purpose:** Manages indoor humidity levels for comfort, health, and building protection

**Responsibilities:**
- Multi-zone humidity monitoring and control
- Dew point calculation and condensation prevention
- Dehumidifier/humidifier coordination
- Integration with cooling and ventilation systems
- Seasonal humidity setpoint adjustment
- Mold risk assessment and prevention

**Inputs:**
- Indoor humidity sensors (per zone)
- Outdoor humidity and temperature
- Surface temperature sensors (cold surfaces)
- Occupancy data
- Weather forecast

**Outputs:**
- Dehumidifier control (on/off or variable speed)
- Humidifier control (steam, evaporative)
- Cooling mode activation for dehumidification
- Ventilation adjustment commands
- ERV/HRV mode selection

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
│  - Modules (HPM, VENT, PMP, HSM, MSC, HCM, TCM)│
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
| Humidity Control Module | 8 | 28 |
| Telemetry Module | 15 | 60 |
| Web UI (SPIFFS) | - | 200 |
| Configuration/Logs | 20 | 100 |
| **Total Estimated** | **165 KB** | **1156 KB** |
| **ESP32 Available** | 520 KB | 4096 KB |
| **Margin** | **355 KB (68%)** | **2940 KB (72%)** |

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

##### Humidity Sensors (for Humidity Control Module)

**Primary: Capacitive Humidity Sensors (SHT31, SHT40, BME280, BME680)**
- **Interface**: I2C
- **Range**: 0-100% RH
- **Accuracy**: ±2-3% RH typical (±1.5% RH for SHT40)
- **Response Time**: <8 seconds (τ63%)
- **Temperature Compensation**: Built-in
- **Typical Use**: Zone humidity monitoring, dew point calculation
- **Notes**: BME680 includes VOC sensor; BME280 includes pressure sensor

**Alternative: Resistive Humidity Sensors**
- **Interface**: Analog (resistance change)
- **Range**: 20-90% RH
- **Accuracy**: ±5% RH
- **Typical Use**: Low-cost installations, non-critical zones

**Surface Condensation Sensors**
- **Type**: Temperature + Humidity sensor pair
- **Purpose**: Monitor cold surfaces (windows, walls) for condensation risk
- **Calculation**: Dew point vs. surface temperature differential
- **Typical Use**: Condensation prevention, mold risk assessment

**Dew Point Calculation:**
```
Dew Point (°C) ≈ T - ((100 - RH) / 5)
Where: T = temperature (°C), RH = relative humidity (%)
More accurate: Magnus-Tetens formula or lookup tables
```

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

##### Humidity Control Outputs

**Dehumidifier Control**
- **Type**: Relay output (on/off) or variable speed (PWM/0-10V)
- **Options**:
  - Standalone dehumidifier: Relay for power control
  - Variable capacity: 0-10V speed control
  - Heat pump cooling mode: Relay + temperature control
- **Typical Capacity**: 30-70 liters/day (residential)
- **Integration**: Coordinate with cooling system to avoid conflicts

**Humidifier Control**
- **Steam Humidifier**:
  - Control: Relay (on/off) or modulating (0-10V)
  - Power: 120-240V AC, 500-3000W typical
  - Response Time: Fast (minutes)
  - Safety: Auto-shutoff on low water, high-limit humidistat

- **Evaporative Humidifier**:
  - Control: Relay for water valve, fan speed (PWM/0-10V)
  - Power: 24V water valve, 120V fan
  - Response Time: Moderate (10-30 minutes)
  - Maintenance: Regular cleaning, filter replacement

**ERV/HRV Humidity Management**
- **Winter Mode**: Heat recovery, minimal outdoor air for humidity control
- **Summer Mode**: Bypass or ERV mode to reduce humidity
- **Control**: Damper position (0-10V) or mode selection (relay)

**Condensate Drain Management**
- **Drain Pump**: Relay control for condensate removal
- **Safety**: Float switch for overflow protection
- **Typical Use**: Dehumidifier, cooling coil condensate

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

## 4. Control Logic & Algorithms

### 4.1 Overview

The Keros system employs sophisticated control algorithms to optimize comfort, efficiency, and equipment longevity. Control strategies range from simple PID loops to advanced model predictive control (MPC) with weather forecasting integration.

**Control Objectives (Priority Order):**
1. **Safety**: Protect equipment and occupants (highest priority)
2. **Comfort**: Maintain setpoints within acceptable deadbands
3. **Efficiency**: Minimize energy consumption while meeting comfort requirements
4. **Equipment Protection**: Reduce wear through optimized operation
5. **Cost Optimization**: Leverage time-of-use pricing and demand response

### 4.2 Temperature Control Algorithms

#### 4.2.1 PID Control Fundamentals

The system uses PID (Proportional-Integral-Derivative) control as the foundation for temperature regulation.

**PID Equation:**
```
Output(t) = Kp × e(t) + Ki × ∫e(t)dt + Kd × de(t)/dt

Where:
  e(t) = Setpoint - Measured_Value (error)
  Kp = Proportional gain
  Ki = Integral gain
  Kd = Derivative gain
```

**Tuning Parameters (Typical Values):**

| Application | Kp | Ki | Kd | Notes |
|-------------|----|----|----|----|
| Zone Temperature | 2.0-5.0 | 0.01-0.05 | 0.1-0.5 | Slow response acceptable |
| Supply Temperature | 5.0-10.0 | 0.05-0.1 | 0.5-1.0 | Faster response needed |
| Storage Tank | 1.0-3.0 | 0.005-0.02 | 0.05-0.2 | Avoid oscillation |
| Humidity Control | 0.5-2.0 | 0.001-0.01 | 0.0-0.1 | Very slow process |

**Anti-Windup:**
```cpp
// Integral term limiting to prevent windup
float integral_term = integral_accumulator * Ki;
if (integral_term > MAX_INTEGRAL_OUTPUT) {
    integral_term = MAX_INTEGRAL_OUTPUT;
    integral_accumulator = MAX_INTEGRAL_OUTPUT / Ki;
}
```

**Derivative Filtering:**
```cpp
// Low-pass filter on derivative to reduce noise sensitivity
derivative_filtered = ALPHA * derivative_raw + (1 - ALPHA) * derivative_previous;
// Typical ALPHA = 0.1 to 0.3
```

#### 4.2.2 Adaptive PID

The system automatically adjusts PID parameters based on operating conditions:

**Load-Based Adaptation:**
```cpp
void adapt_pid_gains(float current_load_percent) {
    // Increase aggressiveness at higher loads
    Kp_effective = Kp_base * (1.0 + 0.5 * current_load_percent / 100.0);

    // Reduce integral action at low loads to prevent overshoot
    Ki_effective = Ki_base * (0.5 + 0.5 * current_load_percent / 100.0);
}
```

**Seasonal Adaptation:**
- Winter (heating): Increase Kp for faster response to cold outdoor conditions
- Summer (cooling): Decrease Kd to reduce oscillation from solar gains
- Shoulder seasons: Balanced tuning for mixed heating/cooling

#### 4.2.3 Cascaded Control

Multi-stage control for improved performance:

```
┌────────────────────────────────────────────────────┐
│  Zone Temperature Controller (Outer Loop)          │
│  Setpoint: 21°C, Measured: 20.5°C                  │
│  Output: Supply Temperature Setpoint = 45°C        │
└────────────────┬───────────────────────────────────┘
                 │
                 ▼
┌────────────────────────────────────────────────────┐
│  Supply Temperature Controller (Inner Loop)        │
│  Setpoint: 45°C, Measured: 42°C                    │
│  Output: Heat Source Demand = 75%                  │
└────────────────┬───────────────────────────────────┘
                 │
                 ▼
          [Heat Source Actuation]
```

**Benefits:**
- Faster disturbance rejection
- Improved stability
- Separation of concerns (comfort vs. equipment control)

### 4.3 Heat Pump Control Algorithms

#### 4.3.1 Compressor Control Strategies

**On/Off Control (Fixed Capacity Heat Pumps):**

```cpp
void control_on_off_compressor() {
    // Minimum runtime protection
    if (compressor_on && (millis() - compressor_start_time < MIN_RUNTIME_MS)) {
        return;  // Cannot turn off yet
    }

    // Minimum off-time protection
    if (!compressor_on && (millis() - compressor_stop_time < MIN_OFF_TIME_MS)) {
        return;  // Cannot turn on yet
    }

    // Hysteresis control
    float temp_error = supply_setpoint - supply_temperature;

    if (temp_error > DEADBAND_ON) {
        compressor_enable = true;
    } else if (temp_error < -DEADBAND_OFF) {
        compressor_enable = false;
    }
    // Else: maintain current state (hysteresis)
}

// Typical values:
// MIN_RUNTIME_MS = 300000 (5 minutes)
// MIN_OFF_TIME_MS = 180000 (3 minutes)
// DEADBAND_ON = 2.0°C
// DEADBAND_OFF = -0.5°C
```

**Variable Capacity Control (Inverter Heat Pumps):**

```cpp
void control_variable_compressor() {
    // PID control for capacity modulation
    float error = supply_setpoint - supply_temperature;

    float capacity_demand = pid_controller.calculate(error);

    // Apply rate limiting (prevent rapid changes)
    float max_change_per_second = 10.0;  // % per second
    float max_change = max_change_per_second * loop_time_seconds;

    capacity_demand = constrain_rate_of_change(
        capacity_demand,
        previous_capacity,
        max_change
    );

    // Apply operational limits
    capacity_output = constrain(capacity_demand, MIN_CAPACITY_PERCENT, 100.0);

    // Convert to 0-10V or PWM signal
    set_compressor_speed(capacity_output);
}
```

#### 4.3.2 COP Optimization

**Real-Time COP Calculation:**

```cpp
float calculate_cop() {
    // Measure heat output
    float flow_rate_lps = read_flow_meter();  // liters per second
    float delta_t = supply_temp - return_temp;  // °C
    float heat_output_kw = flow_rate_lps * 4.186 * delta_t;  // kW

    // Measure electrical input
    float power_input_kw = read_power_meter();  // kW

    // Calculate COP
    float cop = heat_output_kw / power_input_kw;

    // Apply sanity checks
    if (cop < 1.0 || cop > 8.0) {
        log_error("COP out of range: " + String(cop));
        return -1;  // Invalid
    }

    return cop;
}
```

**COP-Based Operating Point Optimization:**

```cpp
void optimize_operating_point() {
    float current_cop = calculate_cop();

    // Adjust supply temperature to maximize COP while meeting load
    if (current_cop > 0 && load_met) {
        // Lower supply temp improves COP (if load still met)
        if (current_cop < target_cop_threshold) {
            supply_setpoint -= 0.5;  // Decrease by 0.5°C
            supply_setpoint = max(supply_setpoint, MIN_SUPPLY_TEMP);
        }
    } else if (!load_met) {
        // Increase supply temp to meet load
        supply_setpoint += 0.5;
        supply_setpoint = min(supply_setpoint, MAX_SUPPLY_TEMP);
    }
}
```

**Weather-Compensated Supply Temperature:**

```cpp
float calculate_supply_setpoint(float outdoor_temp, float indoor_setpoint) {
    // Heating curve: lower outdoor temp → higher supply temp
    // Typical curve: supply = 45°C at 0°C outdoor, 25°C at 20°C outdoor

    float slope = (MAX_SUPPLY_TEMP - MIN_SUPPLY_TEMP) /
                  (MIN_OUTDOOR_TEMP - MAX_OUTDOOR_TEMP);

    float supply_setpoint = MAX_SUPPLY_TEMP + slope * (outdoor_temp - MIN_OUTDOOR_TEMP);

    // Apply limits
    supply_setpoint = constrain(supply_setpoint, MIN_SUPPLY_TEMP, MAX_SUPPLY_TEMP);

    return supply_setpoint;
}
```

#### 4.3.3 Defrost Control

**Frost Detection:**

```cpp
bool is_defrost_needed() {
    // Method 1: Time-temperature integration
    if (outdoor_temp < FROST_THRESHOLD &&
        runtime_minutes > DEFROST_INTERVAL_MIN) {
        return true;
    }

    // Method 2: Temperature differential
    float delta_t = evaporator_temp - outdoor_temp;
    if (delta_t > DEFROST_DELTA_T_THRESHOLD) {
        return true;  // Frost buildup indicated
    }

    // Method 3: Pressure differential (if sensors available)
    if (has_pressure_sensors) {
        if (evap_pressure_drop > FROST_PRESSURE_THRESHOLD) {
            return true;
        }
    }

    return false;
}
```

**Defrost Execution:**

```cpp
void execute_defrost_cycle() {
    // 1. Switch to defrost mode
    set_reversing_valve(COOLING_MODE);  // Reverse refrigerant flow
    compressor_enable = true;
    outdoor_fan_enable = false;  // Stop outdoor fan

    // 2. Monitor defrost progress
    while (defrost_active) {
        if (evaporator_temp > DEFROST_COMPLETE_TEMP ||
            defrost_time > MAX_DEFROST_TIME) {
            // Defrost complete
            break;
        }
        delay(1000);
    }

    // 3. Return to normal operation
    set_reversing_valve(HEATING_MODE);
    outdoor_fan_enable = true;

    // 4. Log defrost metrics
    log_defrost_cycle(defrost_duration, energy_used);
}
```

### 4.4 Ventilation Control Algorithms

#### 4.4.1 Demand-Controlled Ventilation (DCV)

**CO₂-Based Control:**

```cpp
void control_ventilation_co2() {
    float co2_ppm = read_co2_sensor();

    // Setpoint: 800-1000 ppm (ASHRAE 62.1 recommendation)
    float co2_setpoint = 1000;
    float co2_error = co2_ppm - co2_setpoint;

    // Calculate airflow demand (% of max)
    float airflow_percent;

    if (co2_ppm < 600) {
        airflow_percent = MIN_AIRFLOW_PERCENT;  // Minimum ventilation
    } else if (co2_ppm > 1500) {
        airflow_percent = 100.0;  // Maximum ventilation
    } else {
        // Proportional band: 600-1500 ppm
        airflow_percent = map_float(co2_ppm, 600, 1500, MIN_AIRFLOW_PERCENT, 100.0);
    }

    set_fan_speed(airflow_percent);
}
```

**Occupancy-Based Control:**

```cpp
void control_ventilation_occupancy() {
    int occupant_count = get_occupancy_count();

    // ASHRAE 62.1: 15 CFM per person + base ventilation
    float base_airflow_cfm = FLOOR_AREA_SQF * 0.06;  // Area component
    float occupancy_airflow_cfm = occupant_count * 15;  // People component

    float total_required_cfm = base_airflow_cfm + occupancy_airflow_cfm;

    // Convert to fan speed percentage
    float airflow_percent = (total_required_cfm / MAX_AIRFLOW_CFM) * 100.0;
    airflow_percent = constrain(airflow_percent, MIN_AIRFLOW_PERCENT, 100.0);

    set_fan_speed(airflow_percent);
}
```

#### 4.4.2 Heat Recovery Optimization

**ERV/HRV Mode Selection:**

```cpp
void select_hrv_erv_mode() {
    float indoor_temp = read_indoor_temp();
    float outdoor_temp = read_outdoor_temp();
    float indoor_humidity = read_indoor_humidity();
    float outdoor_humidity = read_outdoor_humidity();

    // Summer: High outdoor humidity → ERV to remove moisture
    if (outdoor_temp > 25 && outdoor_humidity > 70) {
        set_mode(ERV_MODE);  // Energy Recovery Ventilation
    }
    // Winter: Dry outdoor air → HRV to retain indoor moisture
    else if (outdoor_temp < 5 && outdoor_humidity < 40) {
        set_mode(HRV_MODE);  // Heat Recovery Ventilation only
    }
    // Shoulder seasons: Use bypass if beneficial
    else if (outdoor_temp > indoor_temp && cooling_desired) {
        set_bypass(OPEN);  // Free cooling
    } else {
        set_mode(HRV_MODE);
        set_bypass(CLOSED);
    }
}
```

### 4.5 Humidity Control Algorithms

#### 4.5.1 Dehumidification Control

**Priority-Based Dehumidification:**

```cpp
void control_dehumidification() {
    float indoor_rh = read_humidity_sensor();
    float rh_setpoint = get_humidity_setpoint();  // Typically 50-55%
    float rh_error = indoor_rh - rh_setpoint;

    // Priority 1: Use existing cooling system if active
    if (cooling_mode_active && rh_error > 5) {
        // Reduce supply temp to enhance dehumidification
        reduce_supply_temp_for_dehumidification();
    }

    // Priority 2: Activate dedicated dehumidifier
    else if (rh_error > 10) {
        dehumidifier_enable = true;

        // Variable capacity control if supported
        if (has_variable_dehumidifier) {
            float capacity = constrain(rh_error * 5.0, 30.0, 100.0);
            set_dehumidifier_capacity(capacity);
        }
    }

    // Priority 3: Increase ventilation if outdoor humidity is lower
    else if (rh_error > 5 && outdoor_humidity < indoor_rh - 10) {
        increase_ventilation_rate();
    }

    // Turn off if setpoint reached with hysteresis
    if (rh_error < -5) {
        dehumidifier_enable = false;
    }
}
```

**Condensation Prevention:**

```cpp
void prevent_condensation() {
    float indoor_temp = read_indoor_temp();
    float indoor_rh = read_humidity_sensor();

    // Calculate dew point
    float dew_point = calculate_dew_point(indoor_temp, indoor_rh);

    // Monitor cold surfaces
    for (auto sensor : surface_temp_sensors) {
        float surface_temp = sensor.read();

        // Alert if surface is approaching dew point
        if (surface_temp < dew_point + 2.0) {
            log_warning("Condensation risk on surface: " + sensor.name);

            // Corrective actions
            activate_dehumidifier();
            // Or increase local heating
            // Or increase air circulation
        }
    }
}

float calculate_dew_point(float temp_c, float rh_percent) {
    // Magnus-Tetens approximation
    float a = 17.27;
    float b = 237.7;

    float alpha = ((a * temp_c) / (b + temp_c)) + log(rh_percent / 100.0);
    float dew_point = (b * alpha) / (a - alpha);

    return dew_point;
}
```

#### 4.5.2 Humidification Control

```cpp
void control_humidification() {
    float indoor_rh = read_humidity_sensor();
    float rh_setpoint = get_humidity_setpoint();  // Typically 40-45% in winter
    float rh_error = rh_setpoint - indoor_rh;

    // Only humidify if below setpoint
    if (rh_error > 5) {
        humidifier_enable = true;

        // Modulating control for steam humidifiers
        if (has_modulating_humidifier) {
            float capacity = constrain(rh_error * 10.0, 30.0, 100.0);
            set_humidifier_output(capacity);
        }

        // Ensure adequate ventilation is maintained
        ensure_minimum_ventilation();
    } else if (rh_error < -2) {
        humidifier_enable = false;
    }

    // Safety limit: prevent over-humidification
    if (indoor_rh > MAX_SAFE_HUMIDITY) {
        humidifier_enable = false;
        log_warning("Maximum humidity limit reached");
    }
}
```

### 4.6 Multi-Source Coordination

#### 4.6.1 Source Priority Selection

```cpp
struct HeatSource {
    String name;
    float cop;  // Current efficiency
    float cost_per_kwh;
    float max_capacity_kw;
    float current_output_kw;
    bool available;
    int priority_score;
};

void select_heat_sources(float heat_demand_kw) {
    std::vector<HeatSource> sources = get_all_heat_sources();

    // Calculate priority score for each source
    for (auto& source : sources) {
        if (!source.available) {
            source.priority_score = -1;
            continue;
        }

        // Score = f(efficiency, cost, capacity)
        // Higher score = higher priority
        source.priority_score =
            source.cop * EFFICIENCY_WEIGHT +
            (1.0 / source.cost_per_kwh) * COST_WEIGHT +
            (source.max_capacity_kw / 50.0) * CAPACITY_WEIGHT;
    }

    // Sort sources by priority
    std::sort(sources.begin(), sources.end(),
              [](HeatSource& a, HeatSource& b) {
                  return a.priority_score > b.priority_score;
              });

    // Allocate load to sources in priority order
    float remaining_demand = heat_demand_kw;

    for (auto& source : sources) {
        if (remaining_demand <= 0) break;

        float allocated_output = min(remaining_demand, source.max_capacity_kw);
        source.current_output_kw = allocated_output;
        remaining_demand -= allocated_output;

        activate_heat_source(source.name, allocated_output);
    }
}

// Typical weights:
// EFFICIENCY_WEIGHT = 50
// COST_WEIGHT = 30
// CAPACITY_WEIGHT = 20
```

### 4.7 Predictive Control

#### 4.7.1 Weather-Compensated Control

```cpp
void weather_compensated_control() {
    // Get weather forecast
    WeatherForecast forecast = get_weather_forecast(6);  // 6 hours ahead

    // Adjust heating curve based on forecast
    if (forecast.temp_dropping && forecast.temp_change < -5) {
        // Pre-heat in anticipation of cold front
        supply_setpoint_adjustment = +5.0;
    } else if (forecast.temp_rising && forecast.temp_change > 5) {
        // Reduce heating in anticipation of warm-up
        supply_setpoint_adjustment = -3.0;
    }

    // Adjust thermal mass charging
    if (forecast.sunny && has_solar_thermal) {
        // Prioritize solar charging before sun sets
        increase_storage_tank_charging();
    }
}
```

#### 4.7.2 Occupancy Prediction

```cpp
void occupancy_predictive_control() {
    // Learn occupancy patterns
    OccupancyPattern pattern = get_learned_occupancy_pattern();

    // Pre-condition space before expected occupancy
    int minutes_before_occupancy = 60;  // 1 hour lead time

    if (pattern.next_occupied_time - current_time < minutes_before_occupancy) {
        // Ramp up to comfort setpoint
        transition_to_occupied_setpoint();
    } else if (pattern.next_unoccupied_time - current_time < 30) {
        // Begin setback
        transition_to_unoccupied_setpoint();
    }
}
```

### 4.8 Safety Limits & Interlocks

#### 4.8.1 Temperature Limits

```cpp
void enforce_temperature_limits() {
    // High limit protection
    if (supply_temp > MAX_SUPPLY_TEMP_LIMIT) {
        emergency_shutdown("OVER_TEMPERATURE");
        log_critical("Supply temperature exceeded limit: " + String(supply_temp));
    }

    // Low limit freeze protection
    if (return_temp < FREEZE_PROTECTION_TEMP) {
        activate_freeze_protection();
        log_warning("Freeze protection activated");
    }

    // Rate of change limit (detect sensor failure)
    float temp_rate_of_change = abs(current_temp - previous_temp) / loop_time;
    if (temp_rate_of_change > MAX_TEMP_CHANGE_RATE) {
        log_error("Abnormal temperature rate of change detected");
        sensor_fault_detected = true;
    }
}
```

#### 4.8.2 Equipment Protection Interlocks

```cpp
bool check_compressor_start_conditions() {
    // Flow interlock
    if (!is_flow_detected()) {
        log_error("Cannot start compressor: No flow detected");
        return false;
    }

    // Temperature limits
    if (supply_temp > MAX_COMPRESSOR_START_TEMP) {
        log_error("Cannot start compressor: Supply temp too high");
        return false;
    }

    // Minimum off-time
    if (millis() - last_compressor_stop < MIN_OFF_TIME_MS) {
        log_info("Compressor off-time not met");
        return false;
    }

    // Outdoor temperature limits
    if (outdoor_temp < MIN_OUTDOOR_TEMP_FOR_OPERATION) {
        log_error("Outdoor temp too low for heat pump operation");
        return false;
    }

    return true;  // All conditions met
}
```

---

## 5. Heat Storage Management

### 5.1 Overview

Heat storage systems are critical for maximizing the efficiency and flexibility of HVAC systems. The Keros controller implements sophisticated algorithms to optimize thermal storage for:
- **Load Shifting**: Charge storage during off-peak hours or when renewable energy is available
- **COP Optimization**: Extract heat from storage at temperatures that maximize heat pump efficiency
- **Demand Smoothing**: Buffer rapid load changes to reduce equipment cycling
- **Renewable Integration**: Store excess solar thermal or heat pump output for later use

### 5.2 Storage Tank Types

#### 5.2.1 Hot Water Storage (DHW)

**Purpose:** Domestic hot water production and storage

**Configuration:**
- Single or dual tank (preheat + finish)
- Volume: 100-500 liters typical (residential)
- Temperature range: 45-65°C (anti-legionella cycles to 60-70°C)
- Sensors: 3-5 temperature sensors (top, mid-top, mid, mid-bottom, bottom)

**Control Strategy:**
```cpp
void control_dhw_storage() {
    float temp_top = read_temp_sensor(DHW_TOP);
    float temp_bottom = read_temp_sensor(DHW_BOTTOM);
    float setpoint = 55.0;  // Target DHW temperature

    // Charge tank from bottom
    if (temp_top < setpoint - 5.0) {
        // Activate heat source
        enable_dhw_charging_pump();
        activate_heat_source_for_dhw();
    } else if (temp_top > setpoint + 2.0) {
        // Tank fully charged
        disable_dhw_charging();
    }

    // Anti-legionella cycle (weekly)
    if (legionella_cycle_due()) {
        heat_tank_to_temperature(65.0);
        maintain_temperature_for_duration(65.0, 30);  // 30 minutes
    }
}
```

#### 5.2.2 Buffer Tank (Heating/Cooling)

**Purpose:** Hydraulic separation, thermal mass, load buffering

**Configuration:**
- Volume: 300-2000 liters typical
- Temperature range: 25-55°C (heating), 6-12°C (cooling)
- Sensors: 4-6 temperature sensors for stratification monitoring
- Multiple connections: Heat sources (top), loads (variable height), return (bottom)

**Stratification Monitoring:**
```cpp
struct TankStratification {
    float temps[6];  // Temperature at each level
    float avg_temp;
    float stratification_index;  // 0 = fully mixed, 1 = perfect stratification
};

TankStratification analyze_tank_stratification() {
    TankStratification strat;

    // Read all temperature sensors
    for (int i = 0; i < 6; i++) {
        strat.temps[i] = read_temp_sensor(BUFFER_TANK_SENSOR[i]);
    }

    // Calculate average
    strat.avg_temp = 0;
    for (int i = 0; i < 6; i++) {
        strat.avg_temp += strat.temps[i];
    }
    strat.avg_temp /= 6;

    // Calculate stratification index
    float max_delta = strat.temps[0] - strat.temps[5];  // Top - bottom
    float ideal_delta = 20.0;  // Ideal stratification gradient
    strat.stratification_index = constrain(max_delta / ideal_delta, 0.0, 1.0);

    return strat;
}
```

#### 5.2.3 Phase Change Material (PCM) Storage

**Purpose:** High energy density storage using latent heat

**Characteristics:**
- Phase change temperature: Selected based on application (e.g., 28°C for cooling, 58°C for heating)
- Higher energy density than water (2-3x per unit volume)
- Nearly isothermal charging/discharging

**Control Considerations:**
- Monitor temperature differential across PCM to detect phase change
- Adjust flow rates to match phase change heat transfer rate
- Account for hysteresis in phase change temperature

### 5.3 Capacity Calculation

#### 5.3.1 Available Energy Estimation

**Sensible Heat Storage (Water):**

```cpp
float calculate_available_energy_kwh(TankType tank) {
    float volume_liters = tank.volume;
    float specific_heat = 4.186;  // kJ/(kg·K) for water
    float density = 1.0;  // kg/L for water

    // Read temperature distribution
    TankStratification strat = analyze_tank_stratification();

    // Method 1: Simple (using average temperature)
    float temp_avg = strat.avg_temp;
    float temp_min_useful = tank.min_discharge_temp;  // e.g., 30°C for heating

    float energy_kwh_simple = volume_liters * density * specific_heat *
                               (temp_avg - temp_min_useful) / 3600.0;

    // Method 2: Detailed (layer-by-layer integration)
    float energy_kwh_detailed = 0;
    float layer_volume = volume_liters / 6;

    for (int i = 0; i < 6; i++) {
        if (strat.temps[i] > temp_min_useful) {
            float layer_energy = layer_volume * density * specific_heat *
                                  (strat.temps[i] - temp_min_useful) / 3600.0;
            energy_kwh_detailed += layer_energy;
        }
    }

    return energy_kwh_detailed;
}
```

**State of Charge (SOC):**

```cpp
float calculate_soc_percent(TankType tank) {
    float energy_available = calculate_available_energy_kwh(tank);
    float energy_max = tank.volume * 1.0 * 4.186 *
                        (tank.max_temp - tank.min_discharge_temp) / 3600.0;

    float soc = (energy_available / energy_max) * 100.0;
    return constrain(soc, 0.0, 100.0);
}
```

### 5.4 Charging Strategies

#### 5.4.1 Optimized Charging Schedule

**Objective:** Charge storage when energy is cheapest or most available

```cpp
void schedule_storage_charging() {
    // Get energy pricing for next 24 hours
    EnergyPricing pricing = get_energy_pricing_forecast();

    // Get solar forecast if solar thermal is available
    SolarForecast solar = get_solar_forecast();

    // Identify optimal charging windows
    std::vector<TimeWindow> charging_windows;

    for (int hour = 0; hour < 24; hour++) {
        bool should_charge = false;

        // Priority 1: Solar availability (free energy)
        if (solar.irradiance[hour] > 400) {  // W/m²
            should_charge = true;
        }
        // Priority 2: Off-peak pricing
        else if (pricing.price[hour] < pricing.avg_price * 0.7) {
            should_charge = true;
        }
        // Priority 3: Demand forecast
        else if (demand_forecast[hour + 6] > storage_capacity * 0.8) {
            should_charge = true;  // Pre-charge before high demand
        }

        if (should_charge) {
            charging_windows.push_back({hour, hour + 1});
        }
    }

    // Execute charging schedule
    schedule_charging_events(charging_windows);
}
```

#### 5.4.2 Stratification-Preserving Charging

**Goal:** Maintain temperature layers for maximum usable energy

```cpp
void charge_tank_stratified(float heat_source_temp) {
    TankStratification strat = analyze_tank_stratification();

    // Determine injection height based on source temperature
    int injection_level = 0;

    for (int i = 0; i < 6; i++) {
        if (heat_source_temp > strat.temps[i] + 2.0) {
            injection_level = i;  // Inject above this level
            break;
        }
    }

    // Control valve to inject at appropriate height
    set_injection_valve_position(injection_level);

    // Adjust flow rate to preserve stratification
    // Lower flow = better stratification, but slower charging
    float flow_rate = calculate_optimal_flow_rate(
        heat_source_temp,
        strat.temps[injection_level],
        tank_geometry
    );

    set_charging_pump_speed(flow_rate);
}
```

### 5.5 Discharging Strategies

#### 5.5.1 Variable Extraction Height

**Objective:** Extract hottest water while preserving stratification

```cpp
void discharge_tank_optimized(float required_temp) {
    TankStratification strat = analyze_tank_stratification();

    // Find highest level meeting temperature requirement
    int extraction_level = -1;

    for (int i = 5; i >= 0; i--) {  // Bottom to top
        if (strat.temps[i] >= required_temp) {
            extraction_level = i;
            break;
        }
    }

    if (extraction_level == -1) {
        // No level meets requirement
        log_warning("Tank temperature insufficient");
        // Activate heat source for direct heating
        enable_direct_heating_mode();
        return;
    }

    // Extract from identified level
    set_extraction_valve_position(extraction_level);

    // Monitor and adjust extraction height as tank discharges
    monitor_extraction_temp();
}
```

#### 5.5.2 Heat Pump Source Temperature Optimization

**Use Case:** Extract from tank at optimal temperature for heat pump COP

```cpp
void optimize_hp_source_temp() {
    TankStratification strat = analyze_tank_stratification();

    // Heat pump COP improves with higher source temperature
    // But we want to preserve high-temperature water for direct use

    // Strategy: Extract from mid-levels for heat pump
    float optimal_source_temp = 35.0;  // Target for good COP

    // Find level closest to optimal
    int best_level = 0;
    float min_diff = 100.0;

    for (int i = 1; i < 5; i++) {  // Avoid top and bottom
        float diff = abs(strat.temps[i] - optimal_source_temp);
        if (diff < min_diff) {
            min_diff = diff;
            best_level = i;
        }
    }

    // Extract from best level for heat pump evaporator
    set_hp_source_extraction_level(best_level);

    // Return cooled water to bottom
    // This maintains stratification and efficiency
}
```

### 5.6 Multi-Tank Management

#### 5.6.1 Tank Prioritization

**Scenario:** Multiple tanks (DHW, buffer, solar preheat)

```cpp
void manage_multiple_tanks() {
    // Priority order for charging
    std::vector<Tank> tanks = {dhw_tank, buffer_tank, solar_preheat_tank};

    // Assess state of charge for each tank
    for (auto& tank : tanks) {
        tank.soc = calculate_soc_percent(tank);
        tank.priority = calculate_priority(tank);
    }

    // Sort by priority
    std::sort(tanks.begin(), tanks.end(),
              [](Tank& a, Tank& b) { return a.priority > b.priority; });

    // Allocate heat source to highest priority tank
    if (tanks[0].soc < 80.0) {  // Tank needs charging
        charge_tank(tanks[0]);
    }
}

int calculate_priority(Tank& tank) {
    int priority = 0;

    // DHW has highest base priority
    if (tank.type == DHW) {
        priority += 100;
    }

    // Low SOC increases priority
    priority += (100 - tank.soc);

    // Time-of-use pricing consideration
    if (currently_off_peak()) {
        priority += 50;
    }

    // Predicted demand
    if (high_demand_forecast_next_hours(tank.type, 6)) {
        priority += 30;
    }

    return priority;
}
```

### 5.7 Solar Thermal Integration

#### 5.7.1 Differential Temperature Control

**Classic Solar Thermal Control:**

```cpp
void control_solar_thermal_pump() {
    float temp_collector = read_temp_sensor(SOLAR_COLLECTOR);
    float temp_tank_bottom = read_temp_sensor(SOLAR_TANK_BOTTOM);

    float delta_t = temp_collector - temp_tank_bottom;

    // Hysteresis control
    static bool pump_running = false;

    if (!pump_running) {
        if (delta_t > DELTA_T_ON_THRESHOLD) {  // e.g., 8°C
            pump_running = true;
            enable_solar_pump();
        }
    } else {
        if (delta_t < DELTA_T_OFF_THRESHOLD) {  // e.g., 3°C
            pump_running = false;
            disable_solar_pump();
        }
    }

    // Variable speed for efficiency
    if (pump_running) {
        float pump_speed = map_float(delta_t, 5.0, 30.0, 30.0, 100.0);
        pump_speed = constrain(pump_speed, 30.0, 100.0);
        set_solar_pump_speed(pump_speed);
    }
}
```

#### 5.7.2 Overheating Protection

```cpp
void protect_solar_system_from_overheating() {
    float temp_collector = read_temp_sensor(SOLAR_COLLECTOR);
    float temp_tank_top = read_temp_sensor(SOLAR_TANK_TOP);

    // Collector too hot (stagnation risk)
    if (temp_collector > 95.0) {
        // Emergency heat dump
        if (temp_tank_top < 70.0) {
            // Force circulation to dump heat into tank
            set_solar_pump_speed(100.0);
        } else {
            // Tank also too hot - activate heat dump radiator
            activate_heat_dump_radiator();
        }
    }

    // Tank overheating prevention
    if (temp_tank_top > 75.0) {
        // Stop solar charging
        disable_solar_pump();

        // Cool tank by forced circulation to loads
        activate_cooling_circulation();
    }
}
```

### 5.8 Predictive Storage Management

#### 5.8.1 Demand Forecasting

```cpp
struct DemandForecast {
    float hourly_demand_kwh[24];
    float confidence;
};

DemandForecast forecast_demand() {
    DemandForecast forecast;

    // Method 1: Historical pattern matching
    int day_of_week = get_day_of_week();
    HistoricalData hist = get_historical_demand(day_of_week, 4);  // Last 4 weeks

    for (int hour = 0; hour < 24; hour++) {
        forecast.hourly_demand_kwh[hour] = hist.avg_demand[hour];
    }

    // Method 2: Weather adjustment
    WeatherForecast weather = get_weather_forecast();
    for (int hour = 0; hour < 24; hour++) {
        float outdoor_temp = weather.temperature[hour];

        // Heating degree days adjustment
        if (outdoor_temp < 18.0) {
            float hdd = 18.0 - outdoor_temp;
            forecast.hourly_demand_kwh[hour] *= (1.0 + hdd * 0.05);
        }
    }

    // Method 3: Occupancy prediction
    OccupancyPattern occupancy = get_learned_occupancy();
    for (int hour = 0; hour < 24; hour++) {
        if (occupancy.occupied[hour]) {
            forecast.hourly_demand_kwh[hour] *= 1.2;  // 20% increase when occupied
        }
    }

    forecast.confidence = calculate_forecast_confidence(hist.variance);

    return forecast;
}
```

#### 5.8.2 Optimal SOC Target

```cpp
float calculate_optimal_soc_target(int hours_ahead) {
    DemandForecast demand = forecast_demand();

    // Calculate cumulative demand for forecast period
    float cumulative_demand = 0;
    for (int i = 0; i < hours_ahead; i++) {
        cumulative_demand += demand.hourly_demand_kwh[i];
    }

    // Calculate required storage capacity
    float tank_capacity_kwh = calculate_tank_capacity_kwh();
    float required_soc = (cumulative_demand / tank_capacity_kwh) * 100.0;

    // Add safety margin
    required_soc *= 1.2;  // 20% safety margin

    // Constrain to realistic values
    required_soc = constrain(required_soc, 30.0, 95.0);

    return required_soc;
}
```

### 5.9 Efficiency Metrics & Monitoring

#### 5.9.1 Storage Efficiency Tracking

```cpp
struct StorageEfficiency {
    float energy_in_kwh;
    float energy_out_kwh;
    float losses_kwh;
    float efficiency_percent;
    float cycle_count;
};

void track_storage_efficiency() {
    static StorageEfficiency metrics_daily;
    static unsigned long last_reset = 0;

    // Measure energy into storage
    if (charging_active) {
        float power_in = measure_charging_power_kw();
        metrics_daily.energy_in_kwh += power_in * (loop_time / 3600.0);
    }

    // Measure energy out of storage
    if (discharging_active) {
        float power_out = measure_discharging_power_kw();
        metrics_daily.energy_out_kwh += power_out * (loop_time / 3600.0);
    }

    // Calculate losses (standing + cycling)
    TankStratification strat = analyze_tank_stratification();
    float tank_avg_temp = strat.avg_temp;
    float ambient_temp = read_ambient_temp();
    float temp_diff = tank_avg_temp - ambient_temp;

    // Standing loss (W) = U-value × Area × ΔT
    float standing_loss_w = TANK_U_VALUE * TANK_SURFACE_AREA * temp_diff;
    metrics_daily.losses_kwh += standing_loss_w / 1000.0 * (loop_time / 3600.0);

    // Daily reset and reporting
    if (millis() - last_reset > 86400000) {  // 24 hours
        metrics_daily.efficiency_percent =
            (metrics_daily.energy_out_kwh /
             (metrics_daily.energy_in_kwh + 0.001)) * 100.0;

        log_storage_efficiency(metrics_daily);

        // Reset counters
        metrics_daily = {0};
        last_reset = millis();
    }
}
```

#### 5.9.2 Stratification Quality Monitoring

```cpp
void monitor_stratification_quality() {
    TankStratification strat = analyze_tank_stratification();

    // Log stratification index over time
    log_stratification_index(strat.stratification_index);

    // Alert if stratification is degrading
    if (strat.stratification_index < 0.3) {
        log_warning("Poor tank stratification detected");
        log_info("Consider: reducing flow rates, checking baffles, reviewing injection points");
    }

    // Identify mixing events
    static float prev_index = 0;
    if (strat.stratification_index < prev_index - 0.2) {
        log_event("Stratification disruption detected");
        // Investigate: rapid charging/discharging, pump issues, valve problems
    }

    prev_index = strat.stratification_index;
}
```

### 5.10 Advanced Control Strategies

#### 5.10.1 Model Predictive Control (MPC) for Storage

**Concept:** Optimize storage operation over a prediction horizon

```cpp
void mpc_storage_optimization() {
    // Prediction horizon: 24 hours
    const int HORIZON = 24;

    // Get forecasts
    DemandForecast demand = forecast_demand();
    EnergyPricing pricing = get_energy_pricing_forecast();
    WeatherForecast weather = get_weather_forecast();

    // Define optimization problem
    // Minimize cost while meeting demand and respecting constraints

    float total_cost = 0;
    float soc[HORIZON];
    soc[0] = current_soc;

    for (int hour = 0; hour < HORIZON; hour++) {
        // Decision variables: charge/discharge rate
        float charge_rate = 0;  // kW (to be optimized)

        // Constraints
        // 1. SOC limits
        soc[hour + 1] = soc[hour] + charge_rate - demand.hourly_demand_kwh[hour];
        soc[hour + 1] = constrain(soc[hour + 1], 20.0, 95.0);

        // 2. Charging/discharging rate limits
        charge_rate = constrain(charge_rate, -MAX_DISCHARGE_RATE, MAX_CHARGE_RATE);

        // 3. Heat source capacity
        // charge_rate limited by available heat source capacity

        // Objective: minimize cost
        if (charge_rate > 0) {
            // Charging
            total_cost += charge_rate * pricing.price[hour];
        }

        // (Simplified - full implementation would use optimization library)
    }

    // Apply optimal control action for current hour
    execute_optimal_charging_rate(charge_rate_optimal[0]);
}
```

---

## 6. User Interface Requirements

### 6.1 Overview

The Keros system provides multiple user interfaces designed for different user types and use cases:
- **Web Dashboard**: Primary interface for configuration, monitoring, and control
- **Mobile App**: Remote access and notifications
- **Local Display**: Basic status and emergency access
- **REST API**: Programmatic access for integrations

### 6.2 Web Dashboard

#### 6.2.1 Dashboard Layout

**Home Screen:**
```
┌──────────────────────────────────────────────────────────────┐
│  Keros HVAC Controller             [User] [Settings] [Help] │
├──────────────────────────────────────────────────────────────┤
│                                                               │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐            │
│  │  Indoor    │  │  Outdoor   │  │  System    │            │
│  │   21.5°C   │  │   5.2°C    │  │  Running   │            │
│  │   50% RH   │  │   85% RH   │  │  COP 3.2   │            │
│  └────────────┘  └────────────┘  └────────────┘            │
│                                                               │
│  System Status: ██████████░░░░  Power: 2.3 kW               │
│                                                               │
│  ┌─────────────── Zone Control ──────────────────┐          │
│  │  Living Room:  21°C  [+][-]  Mode: Auto       │          │
│  │  Bedroom:      19°C  [+][-]  Mode: Eco        │          │
│  │  Kitchen:      20°C  [+][-]  Mode: Comfort    │          │
│  └────────────────────────────────────────────────┘          │
│                                                               │
│  ┌─────────────── Quick Actions ──────────────────┐         │
│  │  [Boost Heat]  [Away Mode]  [Schedule]        │         │
│  └────────────────────────────────────────────────┘         │
│                                                               │
│  Recent Alerts: None                                         │
│  Energy Today: 12.5 kWh  Cost: $1.50                        │
└──────────────────────────────────────────────────────────────┘
```

**Key Features:**
- Real-time data updates (every 5 seconds)
- Responsive design (mobile, tablet, desktop)
- Dark/light theme support
- Accessible (WCAG 2.1 AA compliant)

#### 6.2.2 Configuration Interface

**Setup Wizard (First-Time Configuration):**
1. **Hardware Detection**: Auto-discover sensors and actuators
2. **System Type**: Select HVAC configuration (heat pump, boiler, etc.)
3. **Zone Setup**: Define zones and assign sensors
4. **Heat Sources**: Configure available heat sources
5. **Schedules**: Set up basic heating/cooling schedules
6. **Calibration**: Sensor calibration and validation

**Advanced Configuration:**
- PID tuning parameters
- Safety limits and interlocks
- Module enable/disable
- I/O mapping
- Network settings

#### 6.2.3 Monitoring & Diagnostics

**Real-Time Monitoring:**
- Live sensor readings with graphs
- Equipment status (on/off, speed, mode)
- Energy consumption (instantaneous and cumulative)
- System performance metrics (COP, efficiency)

**Historical Data:**
- Temperature trends (hour/day/week/month/year)
- Energy consumption reports
- Equipment runtime logs
- Fault history

**System Diagnostics:**
- Module health status
- Communication diagnostics
- Sensor validation
- Performance analysis

### 6.3 Mobile Application

#### 6.3.1 Features

**Core Functions:**
- Temperature setpoint adjustment
- Mode selection (auto/comfort/eco/away)
- Schedule management
- Real-time status monitoring
- Push notifications for alerts

**Notifications:**
- Critical alerts (system faults, safety events)
- Maintenance reminders
- Energy reports
- Temperature threshold alerts

#### 6.3.2 Platform Support

- **iOS**: Native app (Swift/SwiftUI) or progressive web app
- **Android**: Native app (Kotlin) or progressive web app
- **Cross-platform**: React Native or Flutter option

### 6.4 Local Display (Optional)

**Hardware:**
- OLED (128x64) for basic status
- TFT touchscreen (320x240) for interactive control

**Display Content:**
- Current temperatures (indoor/outdoor)
- System status (heating/cooling/idle)
- Active alarms
- Touch controls for setpoint adjustment

### 6.5 REST API

#### 6.5.1 API Endpoints

**Authentication:**
```
POST /api/auth/login
POST /api/auth/refresh
POST /api/auth/logout
```

**System Status:**
```
GET /api/status
GET /api/sensors
GET /api/zones
GET /api/equipment
```

**Control:**
```
POST /api/zones/{id}/setpoint
POST /api/system/mode
POST /api/equipment/{id}/command
```

**Configuration:**
```
GET /api/config
PUT /api/config
GET /api/schedules
POST /api/schedules
```

**Data & Analytics:**
```
GET /api/data/historical?start={timestamp}&end={timestamp}
GET /api/data/energy
GET /api/data/performance
```

#### 6.5.2 Authentication & Security

- JWT-based authentication
- Role-based access control (admin, user, read-only)
- API rate limiting
- HTTPS/TLS enforcement
- API key management

### 6.6 User Experience (UX) Principles

**Simplicity:**
- Default to "auto" mode for hands-off operation
- Progressive disclosure of advanced features
- Contextual help and tooltips

**Feedback:**
- Immediate visual feedback for user actions
- Clear indication of system state changes
- Progress indicators for long operations

**Safety:**
- Confirmation dialogs for critical actions
- Undo capability where applicable
- Warnings for out-of-range values

**Performance:**
- Fast page loads (<2 seconds)
- Smooth animations (60 fps)
- Offline capability for cached data

---

## 7. Modular Design & Extensibility

### 7.1 Module Development Framework

#### 7.1.1 Creating a New Module

**Step 1: Define Module Interface**

Create `my_module.interface`:
```json
{
  "module_name": "my_custom_module",
  "version": "1.0.0",
  "api_version": "1.0.0",
  "description": "Custom functionality for specific application",
  "author": "Developer Name",
  "dependencies": {
    "modules": [],
    "hardware": ["GPIO"],
    "minimum_firmware": "1.0.0"
  },
  "capabilities": [
    "custom_control",
    "data_logging"
  ],
  "resource_requirements": {
    "ram_bytes": 4096,
    "flash_bytes": 16384,
    "loop_frequency_hz": 1
  },
  "configuration_schema": {
    "type": "object",
    "properties": {
      "enabled": {"type": "boolean", "default": true},
      "update_interval": {"type": "integer", "default": 60}
    },
    "required": ["enabled"]
  }
}
```

**Step 2: Implement Module Class**

Create `my_module.h`:
```cpp
#ifndef MY_MODULE_H
#define MY_MODULE_H

#include "module_interface.h"

class MyCustomModule : public ModuleInterface {
private:
    bool enabled;
    unsigned long last_update;

public:
    MyCustomModule();

    // Required interface methods
    ModuleMetadata get_metadata() override;
    HealthStatus health_check() override;
    bool initialize(Configuration config) override;
    void shutdown() override;
    String get_api_version() override;
    void loop() override;

    // Custom methods
    void process_data();
};

#endif
```

Create `my_module.cpp`:
```cpp
#include "my_module.h"

MyCustomModule::MyCustomModule() {
    enabled = false;
    last_update = 0;
}

ModuleMetadata MyCustomModule::get_metadata() {
    ModuleMetadata meta;
    meta.name = "my_custom_module";
    meta.version = "1.0.0";
    meta.description = "Custom functionality";
    return meta;
}

HealthStatus MyCustomModule::health_check() {
    if (!enabled) return HEALTH_DISABLED;

    // Perform health checks
    if (/* some error condition */) {
        return HEALTH_CRITICAL;
    }

    return HEALTH_HEALTHY;
}

bool MyCustomModule::initialize(Configuration config) {
    enabled = config.get_bool("enabled", true);

    // Initialize resources

    log_info("MyCustomModule initialized");
    return true;
}

void MyCustomModule::shutdown() {
    // Clean up resources
    enabled = false;
    log_info("MyCustomModule shutdown");
}

String MyCustomModule::get_api_version() {
    return "1.0.0";
}

void MyCustomModule::loop() {
    if (!enabled) return;

    unsigned long now = millis();
    if (now - last_update > 1000) {  // Update every second
        process_data();
        last_update = now;
    }
}

void MyCustomModule::process_data() {
    // Custom module logic here
}
```

**Step 3: Register Module**

Add to `modules/modules.cpp`:
```cpp
#include "my_module.h"

void register_modules() {
    // Existing modules...

    skeleton.register_module(new MyCustomModule());
}
```

#### 7.1.2 Module Communication

**Publishing Events:**
```cpp
void MyCustomModule::loop() {
    // Create and publish event
    Event event;
    event.type = "CUSTOM_DATA_UPDATE";
    event.source = "my_custom_module";
    event.priority = PRIORITY_NORMAL;
    event.data["value"] = read_sensor();

    skeleton.publish_event(event);
}
```

**Subscribing to Events:**
```cpp
void MyCustomModule::initialize(Configuration config) {
    // Subscribe to temperature readings
    skeleton.subscribe("TEMPERATURE_READING",
        [this](Event& event) {
            float temp = event.data["value"];
            process_temperature(temp);
        });
}
```

### 7.2 Plugin System

#### 7.2.1 Dynamic Module Loading

**Module Discovery:**
- Scan `/modules/` directory at startup
- Load `.interface` files
- Validate compatibility
- Resolve dependencies
- Initialize in correct order

**Hot Reload (Development Mode):**
```cpp
bool reload_module(String module_name) {
    // 1. Pause module
    Module* mod = skeleton.get_module(module_name);
    if (!mod) return false;

    mod->set_state(MODULE_PAUSED);

    // 2. Shutdown
    mod->shutdown();

    // 3. Reload code (from updated binary)
    // (Platform-specific implementation)

    // 4. Re-initialize
    if (!mod->initialize(mod->get_config())) {
        log_error("Module reload failed");
        return false;
    }

    // 5. Resume
    mod->set_state(MODULE_RUNNING);

    return true;
}
```

### 7.3 Integration Patterns

#### 7.3.1 Third-Party Smart Home Integration

**Home Assistant Integration:**
- MQTT Auto-discovery
- Entity definitions (climate, sensor, switch)
- State reporting
- Command handling

**Example MQTT Auto-Discovery:**
```json
{
  "name": "Keros Living Room",
  "unique_id": "keros_zone_living_room",
  "device_class": "temperature",
  "state_topic": "homeassistant/climate/keros/living_room/state",
  "command_topic": "homeassistant/climate/keros/living_room/set",
  "temperature_state_topic": "homeassistant/sensor/keros/living_room_temp/state",
  "temperature_command_topic": "homeassistant/climate/keros/living_room/set_temp",
  "modes": ["off", "heat", "cool", "auto"],
  "min_temp": 15,
  "max_temp": 30
}
```

#### 7.3.2 Custom Scripting (Lua/Python)

**Allow users to define custom control logic:**

```python
# Example: Custom away mode script
def on_away_mode_activate():
    set_all_zones_temperature(16.0)  # Energy saving
    enable_frost_protection()
    disable_dhw_heating()  # No hot water needed

def on_away_mode_deactivate():
    restore_previous_settings()
    enable_dhw_heating()
```

### 7.4 Module Marketplace (Future)

**Vision:** Community-developed modules

- Module repository with version control
- Automated testing and validation
- User ratings and reviews
- One-click installation
- Secure sandboxing

---

## 8. Communication Protocols

### 8.1 Wi-Fi & Network

#### 8.1.1 Network Configuration

**Connection Modes:**
- **Station Mode (STA)**: Connect to existing Wi-Fi network
- **Access Point Mode (AP)**: Create temporary hotspot for setup
- **AP+STA**: Simultaneous modes for advanced users

**Initial Setup (Captive Portal):**
1. Device boots in AP mode ("Keros-Setup-XXXX")
2. User connects to AP
3. Captive portal presents Wi-Fi selection
4. User enters credentials
5. Device connects and switches to STA mode

**Network Security:**
- WPA2/WPA3 support
- Static IP or DHCP
- mDNS for local discovery (`keros.local`)
- Fallback to AP mode if connection fails

#### 8.1.2 Protocol Support

**HTTP/HTTPS:**
- Web server for dashboard (port 80/443)
- REST API endpoints
- WebSocket for real-time updates
- TLS/SSL with self-signed or custom certificates

**MQTT:**
- Publish system state
- Subscribe to commands
- QoS levels (0, 1, 2 supported)
- Retained messages for persistent state
- Last Will and Testament (LWT) for availability

**Modbus TCP:**
- Optional for BMS integration
- Standard Modbus register mapping
- Configurable slave address

### 8.2 Serial Communication

#### 8.2.1 Modbus RTU

**Configuration:**
- Baud rate: 9600-115200 (configurable)
- Parity: None, Even, Odd
- Stop bits: 1 or 2
- Slave address: 1-247

**Register Map (Example):**
| Address | Type | Description | Unit |
|---------|------|-------------|------|
| 0-9 | Input | Zone temperatures | 0.1°C |
| 10-19 | Input | Humidity levels | 0.1% RH |
| 20 | Input | Outdoor temperature | 0.1°C |
| 100 | Holding | Zone 1 setpoint | 0.1°C |
| 101 | Holding | Zone 2 setpoint | 0.1°C |
| 200 | Coil | System enable | Boolean |
| 201 | Coil | Heating mode | Boolean |

#### 8.2.2 Debug/Programming Interface

**UART0 (USB):**
- Console output for debugging
- Firmware upload
- Diagnostic commands
- Log streaming

### 8.3 I2C & SPI Sensors

**I2C Bus Management:**
- Bus scanning for device discovery
- Address conflict detection
- Clock stretching support
- Error recovery (bus reset)

**Example: Multi-sensor polling:**
```cpp
void poll_i2c_sensors() {
    // Iterate through configured sensors
    for (auto& sensor : i2c_sensors) {
        if (sensor.is_available()) {
            sensor_data data = sensor.read();
            publish_sensor_data(sensor.id, data);
        } else {
            log_warning("Sensor unavailable: " + sensor.name);
            attempt_recovery(sensor);
        }
    }
}
```

### 8.4 1-Wire Protocol

**DS18B20 Temperature Sensors:**
- Multiple sensors on single GPIO
- Parasitic power mode support
- 9-12 bit resolution (user selectable)
- Asynchronous conversion for efficiency

**Topology Considerations:**
- Star topology preferred for reliability
- Bus length: up to 100m with proper wiring
- Pull-up resistor: 4.7kΩ typical
- Avoid long stub lengths

### 8.5 BLE (Bluetooth Low Energy)

#### 8.5.1 Use Cases

**Commissioning:**
- Mobile app pairing during installation
- Secure parameter transfer
- Faster than Wi-Fi setup for some users

**Proximity Detection:**
- Detect when user is home (beacon mode)
- Automatic mode switching
- Presence-based control

**Sensor Integration:**
- BLE thermometers (e.g., Xiaomi Mi Temperature)
- Wireless room sensors
- Energy monitoring devices

#### 8.5.2 BLE Services & Characteristics

**Custom GATT Service:**
```
Service UUID: 0000180a-0000-1000-8000-00805f9b34fb

Characteristics:
- Temperature Setpoint (R/W): Set zone temperature
- Current Temperature (R/Notify): Read current temp
- System Mode (R/W): Heat/Cool/Auto mode
- System Status (R/Notify): Running state
```

### 8.6 Cloud Connectivity

#### 8.6.1 Cloud Services

**Remote Access:**
- Secure tunnel for remote dashboard access
- Encrypted communication (TLS 1.2+)
- NAT traversal
- Optional: VPN integration

**Data Analytics:**
- Historical data upload
- Performance benchmarking
- Predictive maintenance
- Firmware update distribution

**Alexa/Google Home Integration:**
- Cloud-to-cloud integration
- Voice command handling
- Status queries

#### 8.6.2 Data Privacy

**User Control:**
- Opt-in for cloud features
- Local-only operation mode
- Data retention policies
- GDPR compliance

---

## 9. Safety & Monitoring

### 9.1 Safety Systems

#### 9.1.1 Multi-Layer Safety Architecture

**Hardware Safety:**
- Independent temperature limit switches
- Pressure relief valves
- Flow switches for compressor protection
- Emergency stop button (optional)

**Software Safety:**
- Watchdog timer (ESP32 hardware watchdog)
- Safety task at highest priority
- Sanity checks on all sensor readings
- Fail-safe defaults

**Redundant Sensors (Critical Points):**
- Dual temperature sensors on critical measurements
- Cross-validation between sensors
- Fault detection via sensor disagreement

#### 9.1.2 Safety Checks

**Every Control Cycle (1 second):**
```cpp
void safety_checks() {
    // Temperature limits
    check_temperature_limits();

    // Pressure limits
    check_pressure_limits();

    // Flow interlocks
    check_flow_interlocks();

    // Sensor health
    check_sensor_health();

    // Watchdog
    feed_watchdog();
}
```

**Fault Response Matrix:**
| Fault Type | Response | Recovery |
|------------|----------|----------|
| Over-temperature | Emergency shutdown | Manual |
| Sensor failure | Use redundant sensor, safe mode | Automatic after repair |
| Communication loss | Continue with last valid data (60s limit) | Automatic |
| Power brownout | Graceful shutdown, save state | Automatic on power restore |
| Watchdog timeout | System reset | Automatic |

#### 9.1.3 Emergency Shutdown Procedure

```cpp
void emergency_shutdown(String reason) {
    // 1. Disable all outputs immediately
    disable_all_outputs();

    // 2. Log fault with timestamp
    log_critical("EMERGENCY SHUTDOWN: " + reason);
    log_system_state();  // Capture state for diagnostics

    // 3. Activate alarms
    activate_local_alarm();
    send_emergency_notification();

    // 4. Enter safe mode
    system_state = STATE_EMERGENCY_SHUTDOWN;

    // 5. Require manual recovery
    require_manual_reset = true;
}
```

### 9.2 Monitoring & Diagnostics

#### 9.2.1 System Health Monitoring

**Module Health Checks:**
- CPU usage per module
- Memory usage per module
- Execution time tracking
- Error rate monitoring

**Performance Metrics:**
```cpp
struct SystemMetrics {
    float cpu_usage_percent;
    float ram_usage_percent;
    int loop_time_ms;
    int loop_overruns;
    int event_queue_depth;
    int wifi_signal_strength;
};
```

#### 9.2.2 Predictive Maintenance

**Equipment Runtime Tracking:**
- Compressor run hours
- Pump run hours
- Valve cycle counts
- Filter usage (air and water)

**Maintenance Alerts:**
- Service reminder at configured intervals
- Degraded performance detection
- Unusual behavior patterns

**Example: Pump Performance Degradation Detection**
```cpp
void monitor_pump_performance() {
    float current_flow = read_flow_meter();
    float pump_speed = get_pump_speed();

    // Calculate flow per unit speed
    float flow_efficiency = current_flow / (pump_speed + 0.1);

    // Compare to historical baseline
    float baseline_efficiency = get_historical_avg_efficiency();

    if (flow_efficiency < baseline_efficiency * 0.8) {
        log_warning("Pump efficiency degraded - check for blockage or wear");
        schedule_maintenance("circulation_pump");
    }
}
```

#### 9.2.3 Data Logging

**Log Levels:**
- **CRITICAL**: System failures, safety events
- **ERROR**: Operational errors, recoverable faults
- **WARNING**: Abnormal conditions, performance issues
- **INFO**: State changes, important events
- **DEBUG**: Detailed diagnostic information

**Log Storage:**
- RAM ring buffer (last 100 entries)
- SD card (persistent, rotated daily)
- Cloud upload (optional, aggregated)

**Log Format:**
```
[2025-11-18 14:32:15.234] [INFO] [heat_pump_module] Compressor started, target COP: 3.5
[2025-11-18 14:32:16.123] [DEBUG] [pid_controller] Kp=5.0, Ki=0.1, Kd=0.5, error=-2.3
```

### 9.3 Fault Detection & Recovery

#### 9.3.1 Sensor Fault Detection

**Methods:**
- Range checking (values within physical limits)
- Rate-of-change limits
- Redundant sensor comparison
- Stuck sensor detection (no change over time)

**Recovery Actions:**
- Switch to redundant sensor
- Use estimated value from model
- Enter safe mode with reduced functionality
- Alert user for service

#### 9.3.2 Communication Fault Handling

**Wi-Fi Disconnection:**
```cpp
void handle_wifi_disconnection() {
    // Continue local control
    log_warning("Wi-Fi disconnected, continuing local operation");

    // Attempt reconnection
    wifi_reconnect_attempts = 0;
    while (wifi_reconnect_attempts < MAX_RECONNECTS) {
        if (wifi.reconnect()) {
            log_info("Wi-Fi reconnected");
            return;
        }
        wifi_reconnect_attempts++;
        delay(5000);  // Wait 5 seconds between attempts
    }

    // If all attempts fail, continue offline
    log_error("Wi-Fi reconnection failed, running offline");
}
```

### 9.4 Cybersecurity

#### 9.4.1 Security Measures

**Authentication:**
- Strong password requirements
- Account lockout after failed attempts
- Session timeout
- Multi-factor authentication (optional)

**Network Security:**
- HTTPS/TLS for all web traffic
- Encrypted MQTT (if used)
- Firewall rules (if using Ethernet)
- Regular security updates

**Code Security:**
- Input validation on all API endpoints
- SQL injection prevention (if database used)
- XSS protection in web interface
- Signed firmware updates

#### 9.4.2 Firmware Updates

**Over-The-Air (OTA) Updates:**
```cpp
bool perform_ota_update(String firmware_url) {
    // 1. Verify user authorization
    if (!user_authorized_for_update()) {
        return false;
    }

    // 2. Download and verify signature
    if (!download_and_verify_firmware(firmware_url)) {
        log_error("Firmware verification failed");
        return false;
    }

    // 3. Enter safe mode (minimal control)
    enter_safe_mode();

    // 4. Perform update
    if (!flash_new_firmware()) {
        log_critical("Firmware update failed");
        rollback_to_previous();
        return false;
    }

    // 5. Reboot
    ESP.restart();
    return true;
}
```

**Update Safety:**
- Rollback capability
- Backup of previous firmware
- Staged rollout (beta testers first)
- Version compatibility checks

---

## 10. Installation & Deployment

### 10.1 Installation Process

#### 10.1.1 Pre-Installation Planning

**Site Survey:**
- Identify heat sources (heat pump, boiler, solar, etc.)
- Count heating/cooling zones
- Locate sensor installation points
- Plan controller mounting location
- Assess power supply requirements
- Check network connectivity

**Bill of Materials:**
- Controller board (appropriate configuration)
- Sensors (temperature, humidity, pressure, flow)
- Actuators and relays
- Wiring and connectors
- Mounting hardware
- Power supply

#### 10.1.2 Physical Installation

**Controller Mounting:**
- Location: Near existing HVAC controls, mechanical room
- Mounting: DIN rail, wall mount, or panel mount
- Clearance: Adequate ventilation, access for service
- Protection: IP20 minimum, IP54 for harsh environments

**Wiring Installation:**
1. **Power Wiring:**
   - Disconnect main power
   - Install dedicated 24V power supply
   - Connect to controller power input
   - Verify polarity

2. **Sensor Wiring:**
   - Run shielded cables for analog sensors
   - Use CAT5/6 for 1-Wire sensor networks
   - Label all wires clearly
   - Test continuity before connection

3. **Actuator Wiring:**
   - Connect relays to HVAC equipment
   - Verify voltage and current ratings
   - Install contactors for high-power loads
   - Add safety interlocks

**Safety Verification:**
- Insulation resistance test
- Ground continuity check
- Voltage level verification
- Polarity confirmation

#### 10.1.3 Software Configuration

**Step 1: Initial Startup**
- Power on controller
- Connect to "Keros-Setup-XXXX" Wi-Fi AP
- Access setup wizard at http://192.168.4.1

**Step 2: Network Setup**
- Select home Wi-Fi network
- Enter credentials
- Set static IP (optional)
- Configure mDNS hostname

**Step 3: Hardware Configuration**
- Run auto-discovery for I2C sensors
- Assign sensors to zones
- Configure I/O mapping
- Calibrate sensors

**Step 4: System Configuration**
- Define zones (names, areas, types)
- Configure heat sources
- Set up heating curves
- Define safety limits

**Step 5: Schedule Programming**
- Create weekly schedules
- Set comfort/eco temperatures
- Configure holiday mode

**Step 6: Testing & Commissioning**
- Manual control test (each zone, each actuator)
- Sensor reading validation
- Safety interlock test
- Auto mode test
- Performance verification

### 10.2 Commissioning Checklist

```
□ Physical Installation
  □ Controller securely mounted
  □ All wiring properly terminated
  □ Polarity checked on all connections
  □ Grounding verified

□ Sensor Configuration
  □ All sensors detected
  □ Sensor readings reasonable
  □ Calibration performed
  □ Redundant sensors validated

□ Actuator Testing
  □ Each relay tested individually
  □ Pump speeds adjustable
  □ Valves open/close correctly
  □ No unwanted actuator activation

□ Safety Systems
  □ High temperature cutoff tested
  □ Low temperature alarm tested
  □ Flow interlock verified
  □ Emergency stop functional

□ Control Logic
  □ Heating mode tested
  □ Cooling mode tested (if applicable)
  □ Auto mode functional
  □ Zone control working

□ Network & Connectivity
  □ Wi-Fi connection stable
  □ Dashboard accessible
  □ API responding
  □ MQTT publishing (if used)

□ User Training
  □ Dashboard navigation explained
  □ Mobile app demonstrated
  □ Schedule programming shown
  □ Maintenance procedures reviewed

□ Documentation
  □ Wiring diagram provided
  □ Configuration backup created
  □ Contact information recorded
  □ Warranty registered
```

### 10.3 Maintenance & Support

#### 10.3.1 Routine Maintenance

**Monthly:**
- Check system logs for errors
- Verify all sensors reading correctly
- Inspect physical connections

**Quarterly:**
- Clean filters (air and water)
- Inspect pump operations
- Check for leaks
- Calibrate sensors if needed

**Annually:**
- Full system inspection
- Professional HVAC service
- Firmware updates
- Performance optimization

#### 10.3.2 Troubleshooting Guide

**Common Issues:**

| Problem | Possible Cause | Solution |
|---------|----------------|----------|
| No heating | Compressor interlock not met | Check flow sensor, verify circulation pump running |
| Temperature not reaching setpoint | Insufficient capacity | Increase heat source output, check heating curve |
| Sensors reading incorrectly | Calibration drift | Recalibrate sensors, check wiring |
| Wi-Fi disconnection | Weak signal | Relocate controller, use Wi-Fi extender |
| High energy consumption | Inefficient operation | Review control settings, check equipment performance |

#### 10.3.3 Support Resources

**Built-in Help:**
- Contextual help in web interface
- Video tutorials (embedded links)
- FAQ section
- Diagnostic wizard

**External Support:**
- Online documentation (docs.keros.com)
- Community forum
- Email support
- Professional installer network

### 10.4 Upgrade Path

**Hardware Upgrades:**
- Additional I/O expanders
- More sensors
- Larger display
- Ethernet module
- Battery backup

**Software Upgrades:**
- Feature updates via OTA
- New modules from marketplace
- Integration plugins
- Enhanced algorithms

---

## Document Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0.0 | 2025-11-18 | System Architect | Initial release - Section 1: Introduction & System Overview |
| 1.0.1 | 2025-11-18 | System Architect | Added Section 2: System Architecture |
| 1.0.2 | 2025-11-18 | System Architect | Added Section 3: Hardware Specifications |
| 1.0.3 | 2025-11-18 | System Architect | Added Section 4: Control Logic & Algorithms |
| 1.0.4 | 2025-11-18 | System Architect | Added Section 5: Heat Storage Management |
| 1.0.5 | 2025-11-18 | System Architect | Added Sections 6-10: Complete System Description |

---

**Document Status:** COMPLETE - All 10 sections finalized
