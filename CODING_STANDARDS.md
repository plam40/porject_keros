# Universal Coding Standard & Architecture Framework

## 📋 Table of Contents

1. [Core Principles](#core-principles)
2. [Documentation Standards](#1-documentation-standards)
3. [Skeleton + Module Architecture](#2-skeleton--module-architecture)
4. [Optimization & Refactoring Protocol](#3-optimization--refactoring-protocol)
5. [Deployment Architecture](#4-deployment-architecture)
6. [Dependency Minimization Strategy](#5-dependency-minimization-strategy)
7. [C++ Performance Modules](#6-c-performance-modules)
8. [Configuration Management & Backup System](#7-configuration-management--backup-system)
9. [Testing & Validation Requirements](#8-testing--validation-requirements)
10. [Performance Monitoring & Telemetry](#9-performance-monitoring--telemetry)
11. [Implementation Checklists](#implementation-checklists)
12. [AI-Assisted Development Prompt](#standard-prompt-for-ai-assisted-development)

---

## Core Principles

Every project must adhere to these foundational principles:

- **Explanatory Documentation**: All code includes comprehensive header comments
- **Modular Architecture**: Built on skeleton + modules principle
- **Self-Diagnostic**: Skeleton validates and isolates faulty modules
- **Standardized Interfaces**: Exact pairing standards between skeleton and modules
- **Continuous Optimization**: Iterative refinement until error-free and performant
- **Single-Command Deployment**: User-friendly deployment with detailed error handling
- **Minimal Dependencies**: Only essential dependencies, fully justified
- **Performance-Critical C++**: Use C++ monoliths where significant speed gains exist
- **Automatic Backups**: Configuration versioning with migration support

---

## 1. Documentation Standards

### Header Comments - Mandatory Structure

Every file must begin with:

```python
"""
@file: [filename]
@purpose: [clear description of module purpose]
@dependencies: [list all external dependencies]
@version: [semantic versioning: MAJOR.MINOR.PATCH]
@last_modified: [ISO date: YYYY-MM-DD]
@author: [identifier]
@performance_notes: [complexity, bottlenecks, optimization history]
@module_type: [SKELETON | MODULE | HYBRID]
"""
```

For C++ files:

```cpp
/**
 * @file: [filename]
 * @purpose: [clear description of module purpose]
 * @dependencies: [list all external dependencies]
 * @version: [semantic versioning: MAJOR.MINOR.PATCH]
 * @last_modified: [ISO date: YYYY-MM-DD]
 * @author: [identifier]
 * @performance_notes: [complexity, bottlenecks, optimization history]
 * @module_type: [SKELETON | MODULE | HYBRID]
 */
```

### Inline Documentation Requirements

- **Complex logic blocks**: Explain WHY, not just WHAT
- **Performance-critical sections**: Include benchmark data and optimization rationale
- **All public interfaces**: Document input/output contracts and error conditions
- **Decision points**: Note alternatives considered and selection rationale

---

## 2. Skeleton + Module Architecture

### Skeleton Core Responsibilities

The skeleton is the foundation that manages the entire system:

#### 1. Module Registry & Lifecycle Management

- Dynamic module discovery and loading
- Dependency resolution with circular detection
- Graceful initialization order management
- Hot-reload capability for development

#### 2. Self-Diagnostic System

```
Pre-flight Checks:
  ✓ Validate all module interfaces before loading
  ✓ Check resource availability (memory, disk, ports)
  ✓ Verify dependencies are satisfied

Runtime Monitoring:
  ✓ Continuous health monitoring (memory, CPU, I/O)
  ✓ Module interface validation (contract verification)
  ✓ Automatic isolation of faulty modules
  ✓ Fallback/safe-mode operation
  ✓ Comprehensive logging with severity levels

Error Handling:
  ✓ Graceful degradation when modules fail
  ✓ Detailed diagnostic messages
  ✓ Automatic recovery attempts
  ✓ Safe shutdown procedures
```

#### 3. Module Communication Protocol

- Standardized message passing interface
- Event bus for loose coupling
- Versioned API contracts
- Backward compatibility layer

### Module Standards

Every module **MUST** implement this interface:

```python
class ModuleInterface:
    """Standard interface that all modules must implement"""

    def get_metadata(self) -> dict:
        """
        Returns module metadata

        Returns:
            dict: {
                'name': str,
                'version': str,
                'dependencies': list[str],
                'capabilities': list[str],
                'resource_requirements': dict
            }
        """
        pass

    def health_check(self) -> tuple[bool, str]:
        """
        Performs module health check

        Returns:
            tuple: (status: bool, diagnostic_message: str)
        """
        pass

    def initialize(self, config: dict) -> bool:
        """
        Initialize module with configuration

        Args:
            config: Configuration dictionary

        Returns:
            bool: True if initialization successful
        """
        pass

    def shutdown(self) -> None:
        """Graceful cleanup and resource release"""
        pass

    def get_api_version(self) -> str:
        """
        Returns the API version this module implements

        Returns:
            str: Semantic version (e.g., "1.2.3")
        """
        pass
```

### Module Pairing Standard

To ensure skeleton and modules work together:

1. **Interface Definition Files** (`.interface`)
   - JSON Schema for validation
   - Clear contract specification
   - Version compatibility rules

2. **Semantic Versioning**
   - MAJOR: Breaking changes
   - MINOR: Backward-compatible additions
   - PATCH: Backward-compatible fixes

3. **Capability Advertisement**
   - Declare what the module can do
   - Specify resource requirements
   - List dependencies explicitly

4. **Contract Testing**
   - Each module includes test suite
   - Validates interface compliance
   - Ensures skeleton compatibility

**Example Module Interface File** (`module_name.interface`):

```json
{
  "interface_version": "1.0.0",
  "module_api_version": "2.1.0",
  "capabilities": [
    "data_processing",
    "batch_operations"
  ],
  "dependencies": {
    "system": ["python>=3.9"],
    "modules": ["core_utils>=1.0.0"],
    "external": ["numpy>=1.21.0"]
  },
  "resource_requirements": {
    "memory_mb": 512,
    "cpu_cores": 2,
    "disk_mb": 100
  },
  "configuration_schema": {
    "type": "object",
    "properties": {
      "batch_size": {"type": "integer", "minimum": 1},
      "timeout_seconds": {"type": "integer", "minimum": 1}
    },
    "required": ["batch_size"]
  }
}
```

---

## 3. Optimization & Refactoring Protocol

### Iterative Refinement Process

#### Step 1: Baseline Establishment

```
1. Profile initial implementation
   - Execution time (use appropriate profiler)
   - Memory consumption (peak and average)
   - I/O operations (disk, network)

2. Document bottlenecks
   - Identify top 10 slowest functions
   - Measure with real-world data
   - Record specific metrics

3. Set quantified performance targets
   - Target execution time
   - Maximum memory usage
   - Acceptable latency ranges
```

#### Step 2: Alternative Exploration

For each critical path, explore at least **3 alternative approaches**:

- Different algorithms (e.g., sorting: quicksort vs mergesort vs heapsort)
- Data structure changes (e.g., list vs dict vs set)
- Parallelization strategies (threading, multiprocessing, async)
- Caching strategies (memoization, LRU cache, distributed cache)

**Document trade-offs:**

```markdown
## Approach Comparison: [Feature Name]

### Approach 1: [Name]
- Speed: [metric]
- Memory: [metric]
- Complexity: [O-notation]
- Pros: [list]
- Cons: [list]

### Approach 2: [Name]
- Speed: [metric]
- Memory: [metric]
- Complexity: [O-notation]
- Pros: [list]
- Cons: [list]

### Selected: [Approach X]
**Rationale**: [detailed explanation]
```

#### Step 3: Incremental Optimization

- Apply optimizations **one at a time**
- Measure impact of each change
- Maintain performance log with before/after metrics
- **Rollback** if complexity increase > performance gain

#### Step 4: Validation Gates

Before considering optimization complete:

- ✅ All tests pass (unit, integration, system)
- ✅ Performance targets met (or exceptions documented)
- ✅ Code review approved
- ✅ Static analysis clean (linting, type checking)
- ✅ Security scan passed

### Python vs C++ Decision Matrix

#### Use C++ For:

- Tight loops processing **>1M iterations**
- Real-time processing requirements (**<10ms latency**)
- CPU-intensive algorithms (video/audio processing, ML inference)
- **Expected performance gain: 10-100x** for compute-bound tasks

#### Use Python For:

- I/O bound operations (network, database, file operations)
- Rapid prototyping and business logic
- Integration and orchestration
- Glue code between systems
- When development speed > execution speed

#### Hybrid Approach:

- Python wrapper with C++ core (via pybind11, ctypes, or Cython)
- **Profile first, optimize hotspots only**
- Document FFI (Foreign Function Interface) overhead in benchmarks
- Maintain Python fallback for portability

**Performance Benchmark Template:**

```python
# performance_benchmark.py
"""
Benchmark: [Feature Name]
Date: [YYYY-MM-DD]

Python Implementation:
  - Time: X.XX seconds
  - Memory: XXX MB

C++ Implementation:
  - Time: X.XX seconds
  - Memory: XXX MB

Speedup: XXx faster
Decision: [Use C++ | Stay with Python]
Rationale: [explanation]
"""
```

---

## 4. Deployment Architecture

### Standard Directory Structure

```
project_root/
├── deploy/
│   ├── deploy.sh (or deploy.py)           # Main deployment script
│   ├── config/
│   │   ├── deployment_profiles/           # dev, staging, prod
│   │   │   ├── dev.yml
│   │   │   ├── staging.yml
│   │   │   └── prod.yml
│   │   └── environment_templates/         # Template configs
│   ├── scripts/
│   │   ├── pre_deploy_checks.sh          # System validation
│   │   ├── health_check.sh               # Post-deploy verification
│   │   └── rollback.sh                   # Emergency rollback
│   ├── docker/                            # Container configs (if applicable)
│   │   ├── Dockerfile
│   │   └── docker-compose.yml
│   └── README_DEPLOY.md                   # Deployment documentation
├── src/                                   # Source code
├── tests/                                 # Test suites
├── docs/                                  # Documentation
├── backups/                               # Configuration backups
└── logs/                                  # Application logs
```

### Deployment Script Requirements

The deployment script must be **intelligent and user-friendly**:

#### 1. Pre-flight System Checks

```bash
# Must verify before deployment:
✓ OS compatibility (Ubuntu 20.04+, CentOS 8+, etc.)
✓ Required dependencies with correct versions
✓ Port availability (check if ports are already in use)
✓ Disk space requirements (minimum X GB free)
✓ User permissions (sudo access if needed)
✓ Conflicting process detection
✓ Network connectivity (if external services required)
```

#### 2. Interactive & Non-Interactive Modes

```bash
# Interactive mode (prompts for confirmation)
./deploy.sh --interactive --profile=production

# Automated mode (for CI/CD)
./deploy.sh --automated --profile=production --auto-backup
```

#### 3. Comprehensive Error Handling

Every error must provide:

```
❌ Deployment failed at step 5/10: Database migration

Reason: Connection timeout to database server (db.example.com:5432)

Possible causes:
  1. Database server is down
     → Check: systemctl status postgresql

  2. Firewall blocking port 5432
     → Check: sudo ufw status
     → Fix: sudo ufw allow 5432/tcp

  3. Incorrect credentials in config/database.yml
     → Verify: username, password, host

  4. Network connectivity issue
     → Check: ping db.example.com
     → Check: telnet db.example.com 5432

Logs available at: /var/log/deploy/deployment_20250118_143022.log

Rollback command:
  ./deploy.sh --rollback=deployment_20250118_143022
```

#### 4. Progress Reporting

```bash
# Clear progress indication
[1/10] ✓ Checking system requirements...        (2s)
[2/10] ✓ Validating configuration files...      (1s)
[3/10] ⚙ Installing dependencies...             (45s)
[4/10] ⚙ Building application...                (120s)
[5/10] ✓ Running database migrations...         (15s)
[6/10] ✓ Deploying application files...         (8s)
[7/10] ✓ Starting services...                   (5s)
[8/10] ✓ Running health checks...               (3s)
[9/10] ✓ Configuring monitoring...              (2s)
[10/10] ✓ Deployment complete!                  (1s)

Total time: 3m 42s
Application URL: https://app.example.com
```

#### 5. Verification Steps

```bash
# Post-deployment must verify:
✓ All services running (check process status)
✓ Health endpoints responding (HTTP 200)
✓ Configuration loaded correctly
✓ Database connections successful
✓ Required files/directories present
✓ Smoke tests passed
```

#### 6. Single-Command Deployment

```bash
# Complete deployment in one command
./deploy.sh --profile=production --auto-backup

# With additional options
./deploy.sh \
  --profile=production \
  --auto-backup \
  --skip-tests \
  --notify-slack=#deployments
```

#### 7. Failure Transparency Example

```
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
❌ DEPLOYMENT FAILED
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Step: 5/10 - Database Migration
Time: 2025-01-18 14:30:22 UTC
Duration: 15s before failure

ERROR: Connection timeout to database server

Details:
  Host: db.example.com
  Port: 5432
  Timeout after: 10 seconds

Root Cause Analysis:
  ⚠ Network connectivity: OK (ping successful)
  ⚠ DNS resolution: OK (resolves to 10.0.1.50)
  ❌ Port accessibility: FAILED (connection refused)

Likely Issues:
  1. PostgreSQL service not running on target server
  2. PostgreSQL not configured to accept remote connections
  3. pg_hba.conf missing entry for this IP
  4. Firewall rule blocking port 5432

Recommended Actions:
  1. SSH to database server: ssh admin@db.example.com
  2. Check service: systemctl status postgresql
  3. Start if needed: systemctl start postgresql
  4. Check pg_hba.conf for connection rules
  5. Verify firewall: sudo ufw status | grep 5432

System State:
  ✓ Previous deployment: Still active and healthy
  ✓ Rollback available: Yes
  ✓ Data integrity: Preserved (no migrations applied)

Logs:
  Full log: /var/log/deploy/deployment_20250118_143022.log
  Error log: /var/log/deploy/error_20250118_143022.log

Rollback:
  ./deploy.sh --rollback=deployment_20250118_143022

Support:
  Documentation: ./deploy/README_DEPLOY.md
  Contact: devops@example.com
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

---

## 5. Dependency Minimization Strategy

### Core Rules

#### 1. Audit Before Adding Any Dependency

Ask these questions:

- ❓ Is this functionality critical to the project?
- ❓ Can we implement it in <100 lines of code?
- ❓ What's the dependency's maintenance status?
  - Last commit date
  - Number of open issues
  - Response time to issues
- ❓ Security track record
  - Known vulnerabilities
  - Security audit history
  - Update frequency
- ❓ License compatibility
- ❓ Size and complexity (adds how much to bundle?)

#### 2. Dependency Budget

Per module limits:

- **Maximum 10 direct dependencies** per module
- Each dependency requires written justification
- Regular dependency review (quarterly audit)
- Automated vulnerability scanning (daily)

**Dependency Justification Template:**

```markdown
## Dependency: [package-name] v[version]

### Purpose
[One-line description of what this solves]

### Why Not Implement In-House?
[Specific reasons - complexity, time, maintenance burden]

### Alternatives Considered
1. [Alternative 1] - Rejected because: [reason]
2. [Alternative 2] - Rejected because: [reason]
3. In-house implementation - Rejected because: [reason]

### Maintenance Commitment
- Active: [Yes/No]
- Last update: [date]
- Security record: [Clean / X vulnerabilities in Y years]
- Community: [Z GitHub stars, W contributors]

### Size Impact
- Package size: [X KB/MB]
- Dependencies pulled: [Y additional packages]
- Total size impact: [Z KB/MB]

### Approved By
[Name] on [Date]
```

#### 3. Vendoring Strategy

Consider vendoring for:

- Critical, small libraries (<500 lines)
- Unmaintained but stable code
- Security-sensitive components

**Vendoring Rules:**

- Pin exact versions (no `^` or `~` in version specs)
- Include original license file
- Document original source and version
- Track with `VENDORED.md` file
- Regular security reviews

#### 4. Standard Library First

Priority order:

1. **Language standard library** (Python `stdlib`, C++ STL)
2. **System libraries** (OpenSSL, zlib, etc.)
3. **Well-established frameworks** (NumPy, Pandas for data)
4. **Specialized libraries** (only if no alternative)
5. **Avoid** convenience libraries for simple tasks

**Examples:**

```python
# ❌ BAD: Adding a dependency for simple tasks
import inflection
plural = inflection.pluralize('word')

# ✅ GOOD: Simple implementation
def pluralize(word):
    """Simple English pluralization (covers 90% of cases)"""
    if word.endswith('y'):
        return word[:-1] + 'ies'
    elif word.endswith(('s', 'x', 'z', 'ch', 'sh')):
        return word + 'es'
    else:
        return word + 's'
```

### Dependency Audit Checklist

Run quarterly:

```markdown
## Dependency Audit - [Date]

For each dependency:
- [ ] Still necessary? (Try removing, run tests)
- [ ] Latest version? (Check for updates)
- [ ] Vulnerabilities? (Run security scan)
- [ ] Better alternatives? (Research replacements)
- [ ] Justification still valid? (Review documentation)
- [ ] License still compatible? (Check for changes)
- [ ] Maintenance status? (Check commit activity)

Actions Required:
- [ ] Update: [package1, package2]
- [ ] Remove: [package3]
- [ ] Replace: [package4 with alternative]
- [ ] Monitor: [package5 - maintainer inactive]
```

---

## 6. C++ Performance Modules

### When to Create C++ Modules

Use C++ when profiling shows:

- Function consuming **>15% total execution time**
- **Expected speedup: minimum 10x**
- Function is stable (not changing frequently)
- Pure computation (not I/O bound)

### Typical Performance Gains

```
Python vs C++ Performance:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Task Type                  Speedup
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Tight loops               10-100x
Matrix operations         20-50x
String processing         5-15x
Sorting large arrays      8-25x
Image processing          50-200x
Cryptographic operations  30-80x
I/O operations            1-2x (minimal gain)
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

### C++ Module Template

```cpp
/**
 * @file: fast_processor.cpp
 * @purpose: Performance-critical data processing module
 * @replaces: python/slow_processor.py::process_data()
 * @benchmark: Python: 450ms, C++: 28ms, Speedup: 16x
 * @version: 1.0.0
 * @last_modified: 2025-01-18
 * @author: [name]
 */

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <vector>
#include <stdexcept>

namespace py = pybind11;

/**
 * Performance monitoring for C++ module
 */
class PerformanceMonitor {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::string operation_name;

public:
    PerformanceMonitor(const std::string& name) : operation_name(name) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~PerformanceMonitor() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time
        ).count();

        // Log performance metric
        py::print(py::str("C++ {}: {} μs").format(operation_name, duration));
    }
};

/**
 * Fast data processing function
 *
 * @param data Input numpy array
 * @param threshold Processing threshold
 * @return Processed numpy array
 * @throws std::runtime_error if input validation fails
 */
py::array_t<double> process_data(
    py::array_t<double> data,
    double threshold
) {
    PerformanceMonitor perf("process_data");

    // Input validation
    if (data.ndim() != 2) {
        throw std::runtime_error("Input must be 2D array");
    }

    auto buf = data.request();
    double* ptr = static_cast<double*>(buf.ptr);

    // Create output array
    auto result = py::array_t<double>(buf.size);
    auto result_buf = result.request();
    double* result_ptr = static_cast<double*>(result_buf.ptr);

    // Fast processing logic
    size_t size = buf.size;
    for (size_t i = 0; i < size; ++i) {
        result_ptr[i] = ptr[i] > threshold ? ptr[i] * 2.0 : ptr[i] * 0.5;
    }

    result.resize({buf.shape[0], buf.shape[1]});
    return result;
}

/**
 * Python module definition
 */
PYBIND11_MODULE(fast_processor, m) {
    m.doc() = "High-performance data processing module";

    m.def("process_data", &process_data,
          py::arg("data"),
          py::arg("threshold") = 0.5,
          "Process data array with given threshold");

    m.attr("__version__") = "1.0.0";
}
```

### Python Wrapper with Fallback

```python
"""
@file: processor.py
@purpose: Data processor with C++ acceleration and Python fallback
@version: 1.0.0
"""

try:
    # Try to import C++ module
    from . import fast_processor
    HAS_CPP = True
    print("✓ Using C++ accelerated processor (16x faster)")
except ImportError as e:
    HAS_CPP = False
    print(f"⚠ C++ module not available, using Python fallback: {e}")

import numpy as np

def process_data_python(data: np.ndarray, threshold: float = 0.5) -> np.ndarray:
    """
    Pure Python implementation (fallback)

    Args:
        data: 2D numpy array
        threshold: Processing threshold

    Returns:
        Processed numpy array
    """
    if data.ndim != 2:
        raise ValueError("Input must be 2D array")

    result = np.where(data > threshold, data * 2.0, data * 0.5)
    return result

def process_data(data: np.ndarray, threshold: float = 0.5) -> np.ndarray:
    """
    Process data array with automatic C++/Python selection

    Automatically uses C++ implementation if available (16x faster),
    falls back to Python if not.

    Args:
        data: 2D numpy array
        threshold: Processing threshold

    Returns:
        Processed numpy array

    Raises:
        ValueError: If input validation fails
    """
    if HAS_CPP:
        try:
            return fast_processor.process_data(data, threshold)
        except Exception as e:
            print(f"⚠ C++ module failed: {e}, falling back to Python")
            return process_data_python(data, threshold)
    else:
        return process_data_python(data, threshold)
```

### Build Configuration

**CMakeLists.txt:**

```cmake
cmake_minimum_required(VERSION 3.12)
project(fast_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python and pybind11
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# Create the Python module
pybind11_add_module(fast_processor fast_processor.cpp)

# Optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(fast_processor PRIVATE -O3 -march=native)
endif()

# Install
install(TARGETS fast_processor LIBRARY DESTINATION .)
```

**setup.py:**

```python
from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "fast_processor",
        ["fast_processor.cpp"],
        extra_compile_args=["-O3", "-march=native"],
    ),
]

setup(
    name="processor",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
)
```

---

## 7. Configuration Management & Backup System

### Configuration Architecture

```
config/
├── schema/
│   ├── config_schema_v1.0.0.json       # JSON Schema validation
│   ├── config_schema_v1.1.0.json
│   └── config_schema_v2.0.0.json
├── active/
│   └── current_config.yml               # Active configuration
├── backups/
│   ├── auto/                            # Automatic backups
│   │   ├── config_20250118_143022_v1.2.3.yml.gz
│   │   └── config_20250117_100511_v1.2.3.yml.gz
│   └── manual/                          # Manual backups
│       └── config_before_migration_v1.2.3.yml.gz
└── migrations/
    ├── migrate_1.0.0_to_1.1.0.py       # Migration scripts
    ├── migrate_1.1.0_to_2.0.0.py
    └── README_MIGRATIONS.md
```

### Automatic Backup Triggers

Backups are created automatically:

- ✅ Before any configuration change
- ✅ Pre-deployment (always)
- ✅ Scheduled daily backups (retention: 30 days)
- ✅ Before system updates
- ✅ On manual request

### Backup Format

```yaml
# Backup file format
meta:
  version: "2.1.3"                    # Config schema version
  timestamp: "2025-01-18T14:30:22Z"   # ISO 8601 format
  backup_type: "auto"                 # auto | manual | pre_deploy
  code_version: "1.5.2"               # Application version
  checksum: "sha256:abc123..."        # For integrity verification
  hostname: "prod-server-01"          # Where backup was created
  created_by: "deploy_script"         # What created it

configuration:
  # Actual configuration data
  database:
    host: "db.example.com"
    port: 5432
    name: "myapp"

  application:
    debug: false
    max_connections: 100

  features:
    new_ui: true
    beta_api: false
```

### Restore Logic with Version Compatibility

```python
"""
@file: config_restore.py
@purpose: Configuration restore with version migration support
@version: 1.0.0
"""

import yaml
import json
from pathlib import Path
from typing import Dict, List, Optional
from datetime import datetime

class ConfigRestore:
    """Handles configuration restoration with version migration"""

    def restore_config(self, backup_file: Path) -> bool:
        """
        Restore configuration from backup with automatic migration

        Args:
            backup_file: Path to backup file

        Returns:
            bool: True if restore successful
        """
        # Load backup
        backup = self._load_backup(backup_file)
        current_version = self._get_current_code_version()
        backup_version = backup['meta']['version']

        print(f"\n{'='*60}")
        print(f"Configuration Restore")
        print(f"{'='*60}")
        print(f"Current code version: v{current_version}")
        print(f"Backup config version: v{backup_version}")
        print(f"Backup created: {backup['meta']['timestamp']}")
        print(f"{'='*60}\n")

        # Handle version compatibility
        if backup_version == current_version:
            return self._apply_direct_restore(backup)

        elif self._version_less_than(backup_version, current_version):
            return self._restore_older_config(backup, backup_version, current_version)

        else:  # backup_version > current_version
            return self._handle_newer_config(backup, backup_version, current_version)

    def _restore_older_config(
        self,
        backup: Dict,
        backup_version: str,
        current_version: str
    ) -> bool:
        """Restore older config to newer code with migrations"""

        print(f"⚠️  Restoring older config (v{backup_version}) to newer code (v{current_version})")
        print(f"📋 Migration required\n")

        # Get migration chain
        migrations = self._get_migration_chain(backup_version, current_version)

        if not migrations:
            print(f"❌ No migration path found from v{backup_version} to v{current_version}")
            return False

        print(f"Migration path: {' → '.join(migrations)}\n")

        # Confirm with user (unless automated mode)
        if not self._is_automated():
            response = input("Proceed with migration? (yes/no): ")
            if response.lower() != 'yes':
                print("Migration cancelled by user")
                return False

        # Apply migrations one by one
        current_config = backup
        for i, migration in enumerate(migrations, 1):
            print(f"[{i}/{len(migrations)}] Applying: {migration}")

            try:
                current_config = self._apply_migration(current_config, migration)
                self._validate_schema(current_config, migration.target_version)
                print(f"  ✓ Success")
            except Exception as e:
                print(f"  ❌ Failed: {e}")
                print(f"\nRollback: Restore aborted at migration step {i}")
                return False

        print(f"\n✓ All migrations completed successfully")
        return self._apply_restore(current_config)

    def _handle_newer_config(
        self,
        backup: Dict,
        backup_version: str,
        current_version: str
    ) -> bool:
        """Handle case where backup is newer than current code"""

        print(f"❌ Cannot restore newer config (v{backup_version}) to older code (v{current_version})\n")
        print(f"This would cause compatibility issues and potential data loss.\n")
        print(f"Options:")
        print(f"  1. Update code to v{backup_version} or later")
        print(f"     → git checkout tags/v{backup_version}")
        print(f"     → ./deploy.sh --profile=production")
        print(f"")
        print(f"  2. Use an older backup compatible with v{current_version}")

        # List compatible backups
        compatible = self._list_compatible_backups(current_version)
        if compatible:
            print(f"\n  Compatible backups found:")
            for backup_file in compatible[:5]:  # Show top 5
                print(f"    - {backup_file}")
            print(f"\n  Restore with:")
            print(f"    ./restore_config.sh --backup={compatible[0]}")
        else:
            print(f"\n  ⚠️  No compatible backups found")

        print(f"")
        print(f"  3. Manual migration (NOT RECOMMENDED)")
        print(f"     → Contact DevOps team for assistance")
        print(f"")

        return False

    def _apply_direct_restore(self, backup: Dict) -> bool:
        """Apply restore when versions match exactly"""

        print(f"✓ Version match - applying direct restore\n")

        # Validate backup integrity
        if not self._verify_checksum(backup):
            print(f"❌ Checksum verification failed - backup may be corrupted")
            return False

        # Create safety backup of current config
        print(f"Creating safety backup of current configuration...")
        self._create_backup("pre_restore")

        # Apply configuration
        try:
            self._write_config(backup['configuration'])
            print(f"✓ Configuration restored successfully")

            # Verify the restored config
            if self._verify_config():
                print(f"✓ Configuration validation passed")
                return True
            else:
                print(f"❌ Configuration validation failed")
                print(f"Rolling back to previous configuration...")
                self._rollback_last()
                return False

        except Exception as e:
            print(f"❌ Restore failed: {e}")
            print(f"Rolling back to previous configuration...")
            self._rollback_last()
            return False

    def _get_migration_chain(
        self,
        from_version: str,
        to_version: str
    ) -> List[str]:
        """
        Get ordered list of migrations needed

        Args:
            from_version: Starting version
            to_version: Target version

        Returns:
            List of migration script names in order
        """
        migrations = []
        migration_dir = Path("config/migrations")

        # Find all migration files
        for migration_file in sorted(migration_dir.glob("migrate_*.py")):
            # Parse version from filename: migrate_1.0.0_to_1.1.0.py
            parts = migration_file.stem.split("_")
            if len(parts) >= 4:
                source_ver = parts[1]
                target_ver = parts[3]

                # Check if this migration is in our path
                if (self._version_less_than_or_equal(from_version, source_ver) and
                    self._version_less_than_or_equal(target_ver, to_version)):
                    migrations.append(migration_file.name)

        return migrations

    def _list_compatible_backups(self, version: str) -> List[Path]:
        """List backups compatible with given version"""

        compatible = []
        backup_dirs = [
            Path("config/backups/auto"),
            Path("config/backups/manual")
        ]

        for backup_dir in backup_dirs:
            if not backup_dir.exists():
                continue

            for backup_file in sorted(backup_dir.glob("*.yml.gz"), reverse=True):
                backup = self._load_backup(backup_file)
                backup_version = backup['meta']['version']

                # Compatible if backup version <= current version
                if self._version_less_than_or_equal(backup_version, version):
                    compatible.append(backup_file)

        return compatible

    def _version_less_than(self, v1: str, v2: str) -> bool:
        """Compare semantic versions"""
        v1_parts = [int(x) for x in v1.split('.')]
        v2_parts = [int(x) for x in v2.split('.')]
        return v1_parts < v2_parts

    def _version_less_than_or_equal(self, v1: str, v2: str) -> bool:
        """Compare semantic versions"""
        v1_parts = [int(x) for x in v1.split('.')]
        v2_parts = [int(x) for x in v2.split('.')]
        return v1_parts <= v2_parts


# Command-line interface
if __name__ == "__main__":
    import sys

    if len(sys.argv) < 2:
        print("Usage: python config_restore.py <backup_file>")
        sys.exit(1)

    backup_file = Path(sys.argv[1])

    if not backup_file.exists():
        print(f"Error: Backup file not found: {backup_file}")
        sys.exit(1)

    restorer = ConfigRestore()
    success = restorer.restore_config(backup_file)

    sys.exit(0 if success else 1)
```

### Migration Script Template

```python
"""
@file: migrate_1.0.0_to_1.1.0.py
@purpose: Migrate configuration from v1.0.0 to v1.1.0
@changes:
  - Added: database.pool_size (default: 10)
  - Renamed: application.max_connections -> application.max_worker_connections
  - Removed: features.old_feature (deprecated)
@version: 1.0.0
"""

def migrate(config: dict) -> dict:
    """
    Migrate configuration from v1.0.0 to v1.1.0

    Args:
        config: Configuration dict at v1.0.0

    Returns:
        Configuration dict at v1.1.0
    """
    print("  Migrating from v1.0.0 to v1.1.0...")

    # Add new database pool_size setting
    if 'database' not in config:
        config['database'] = {}
    config['database']['pool_size'] = 10
    print("    ✓ Added database.pool_size = 10")

    # Rename max_connections
    if 'application' in config and 'max_connections' in config['application']:
        old_value = config['application'].pop('max_connections')
        config['application']['max_worker_connections'] = old_value
        print(f"    ✓ Renamed max_connections -> max_worker_connections ({old_value})")

    # Remove deprecated feature
    if 'features' in config and 'old_feature' in config['features']:
        config['features'].pop('old_feature')
        print("    ✓ Removed deprecated features.old_feature")

    # Update version
    config['meta']['version'] = '1.1.0'

    return config

def rollback(config: dict) -> dict:
    """
    Rollback configuration from v1.1.0 to v1.0.0

    Args:
        config: Configuration dict at v1.1.0

    Returns:
        Configuration dict at v1.0.0
    """
    print("  Rolling back from v1.1.0 to v1.0.0...")

    # Remove added pool_size
    if 'database' in config and 'pool_size' in config['database']:
        config['database'].pop('pool_size')
        print("    ✓ Removed database.pool_size")

    # Rename back
    if 'application' in config and 'max_worker_connections' in config['application']:
        old_value = config['application'].pop('max_worker_connections')
        config['application']['max_connections'] = old_value
        print(f"    ✓ Renamed max_worker_connections -> max_connections ({old_value})")

    # Update version
    config['meta']['version'] = '1.0.0'

    return config

if __name__ == "__main__":
    # Test migration
    test_config = {
        'meta': {'version': '1.0.0'},
        'application': {'max_connections': 100},
        'features': {'old_feature': True}
    }

    print("Testing migration...")
    migrated = migrate(test_config.copy())
    print("\nTesting rollback...")
    rolled_back = rollback(migrated.copy())

    print("\n✓ Migration tests passed")
```

---

## 8. Testing & Validation Requirements

### Mandatory Test Coverage

```
Test Pyramid:

    /\                E2E Tests (5%)
   /  \              - Full system workflows
  /____\             - User scenarios
 /      \           Integration Tests (15%)
/________\          - Module interfaces
/          \        - External services
/____________\     Unit Tests (80%)
               - Individual functions
               - Edge cases
               - Error handling
```

#### Test Coverage Requirements

- **Unit Tests**: >80% code coverage
- **Integration Tests**: All module interfaces
- **System Tests**: End-to-end workflows
- **Performance Tests**: Regression prevention
- **Security Tests**: OWASP Top 10
- **Chaos Tests**: Fault injection for skeleton diagnostics

### Test Organization

```
tests/
├── unit/
│   ├── test_module_a.py
│   ├── test_module_b.py
│   └── test_skeleton.py
├── integration/
│   ├── test_module_interfaces.py
│   └── test_external_services.py
├── system/
│   ├── test_end_to_end.py
│   └── test_deployment.py
├── performance/
│   ├── test_benchmarks.py
│   └── test_load.py
├── security/
│   ├── test_authentication.py
│   └── test_authorization.py
└── chaos/
    ├── test_module_failures.py
    └── test_network_failures.py
```

### Unit Test Template

```python
"""
@file: test_processor.py
@purpose: Unit tests for data processor module
@coverage_target: >80%
"""

import unittest
import numpy as np
from unittest.mock import Mock, patch
from processor import process_data

class TestProcessor(unittest.TestCase):
    """Unit tests for data processor"""

    def setUp(self):
        """Set up test fixtures"""
        self.test_data = np.array([[1.0, 2.0], [3.0, 4.0]])
        self.threshold = 0.5

    def test_basic_processing(self):
        """Test basic data processing functionality"""
        result = process_data(self.test_data, self.threshold)

        # Verify shape preserved
        self.assertEqual(result.shape, self.test_data.shape)

        # Verify processing logic
        expected = np.array([[2.0, 4.0], [6.0, 8.0]])
        np.testing.assert_array_almost_equal(result, expected)

    def test_invalid_input_dimension(self):
        """Test error handling for invalid dimensions"""
        invalid_data = np.array([1.0, 2.0, 3.0])  # 1D array

        with self.assertRaises(ValueError) as context:
            process_data(invalid_data, self.threshold)

        self.assertIn("2D array", str(context.exception))

    def test_edge_case_zero_threshold(self):
        """Test edge case with zero threshold"""
        result = process_data(self.test_data, 0.0)

        # All values should be doubled (all > 0)
        expected = self.test_data * 2.0
        np.testing.assert_array_almost_equal(result, expected)

    def test_performance_benchmark(self):
        """Test performance meets requirements"""
        import time

        large_data = np.random.rand(1000, 1000)

        start = time.time()
        result = process_data(large_data, 0.5)
        duration = time.time() - start

        # Should complete in <100ms for 1M elements
        self.assertLess(duration, 0.1,
                       f"Performance regression: took {duration:.3f}s")

    def test_cpp_fallback(self):
        """Test Python fallback when C++ unavailable"""
        with patch('processor.HAS_CPP', False):
            result = process_data(self.test_data, self.threshold)

            # Should still work correctly
            self.assertEqual(result.shape, self.test_data.shape)

if __name__ == '__main__':
    unittest.main()
```

### Integration Test Template

```python
"""
@file: test_module_integration.py
@purpose: Integration tests for module interfaces
"""

import unittest
from skeleton import Skeleton
from modules.processor import ProcessorModule
from modules.storage import StorageModule

class TestModuleIntegration(unittest.TestCase):
    """Test module integration with skeleton"""

    def setUp(self):
        """Set up test environment"""
        self.skeleton = Skeleton()
        self.processor = ProcessorModule()
        self.storage = StorageModule()

    def test_module_registration(self):
        """Test module can register with skeleton"""
        success = self.skeleton.register_module(self.processor)
        self.assertTrue(success)

        # Verify module appears in registry
        modules = self.skeleton.get_registered_modules()
        self.assertIn('processor', modules)

    def test_module_initialization_order(self):
        """Test modules initialize in correct dependency order"""
        # Storage has no dependencies
        # Processor depends on storage
        self.skeleton.register_module(self.storage)
        self.skeleton.register_module(self.processor)

        init_order = self.skeleton.initialize_all()

        # Storage should initialize before processor
        storage_index = init_order.index('storage')
        processor_index = init_order.index('processor')
        self.assertLess(storage_index, processor_index)

    def test_module_communication(self):
        """Test modules can communicate via skeleton"""
        self.skeleton.register_module(self.processor)
        self.skeleton.register_module(self.storage)
        self.skeleton.initialize_all()

        # Processor sends data to storage
        test_data = {'key': 'value'}
        self.processor.process_and_store(test_data)

        # Verify data reached storage
        stored = self.storage.retrieve('key')
        self.assertEqual(stored, 'value')

    def test_faulty_module_isolation(self):
        """Test skeleton isolates faulty modules"""
        # Create module that fails health check
        faulty_module = Mock()
        faulty_module.get_metadata.return_value = {
            'name': 'faulty',
            'version': '1.0.0'
        }
        faulty_module.health_check.return_value = (False, "Simulated failure")

        self.skeleton.register_module(faulty_module)

        # Skeleton should isolate the faulty module
        self.skeleton.run_health_checks()

        active_modules = self.skeleton.get_active_modules()
        self.assertNotIn('faulty', active_modules)

if __name__ == '__main__':
    unittest.main()
```

### Continuous Validation

#### Pre-commit Hooks

```bash
# .git/hooks/pre-commit
#!/bin/bash

echo "Running pre-commit checks..."

# 1. Linting
echo "→ Running linter..."
pylint src/ || exit 1

# 2. Type checking
echo "→ Running type checker..."
mypy src/ || exit 1

# 3. Code formatting
echo "→ Checking code format..."
black --check src/ || exit 1

# 4. Import sorting
echo "→ Checking import order..."
isort --check-only src/ || exit 1

# 5. Quick unit tests
echo "→ Running quick unit tests..."
pytest tests/unit/ -x -q || exit 1

echo "✓ All pre-commit checks passed"
```

#### CI Pipeline

```yaml
# .github/workflows/ci.yml
name: Continuous Integration

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v2

    - name: Set up Python
      uses: actions/setup-python@v2
      with:
        python-version: '3.9'

    - name: Install dependencies
      run: |
        pip install -r requirements.txt
        pip install -r requirements-dev.txt

    - name: Run linter
      run: pylint src/

    - name: Run type checker
      run: mypy src/

    - name: Run unit tests
      run: pytest tests/unit/ --cov=src --cov-report=xml

    - name: Run integration tests
      run: pytest tests/integration/

    - name: Run system tests
      run: pytest tests/system/

    - name: Check coverage
      run: |
        coverage report --fail-under=80

    - name: Security scan
      run: bandit -r src/

    - name: Upload coverage
      uses: codecov/codecov-action@v2
      with:
        file: ./coverage.xml

  performance:
    runs-on: ubuntu-latest
    needs: test

    steps:
    - uses: actions/checkout@v2

    - name: Run performance benchmarks
      run: pytest tests/performance/ --benchmark-only

    - name: Check for regressions
      run: python scripts/check_performance_regression.py
```

---

## 9. Performance Monitoring & Telemetry

### Built-in Metrics Collection

Every module should collect:

```python
"""
@file: telemetry.py
@purpose: Built-in performance monitoring and telemetry
"""

import time
import psutil
import functools
from typing import Callable, Dict, Any
from collections import defaultdict
from datetime import datetime

class PerformanceMonitor:
    """Centralized performance monitoring"""

    def __init__(self):
        self.metrics = defaultdict(list)
        self.start_time = time.time()

    def record_execution_time(self, function_name: str, duration: float):
        """Record function execution time"""
        self.metrics[f"{function_name}_time"].append({
            'timestamp': datetime.utcnow().isoformat(),
            'duration_ms': duration * 1000
        })

    def record_memory_usage(self, module_name: str):
        """Record current memory usage for module"""
        process = psutil.Process()
        memory_mb = process.memory_info().rss / 1024 / 1024

        self.metrics[f"{module_name}_memory"].append({
            'timestamp': datetime.utcnow().isoformat(),
            'memory_mb': memory_mb
        })

    def record_error(self, module_name: str, error_type: str):
        """Record error occurrence"""
        self.metrics[f"{module_name}_errors"].append({
            'timestamp': datetime.utcnow().isoformat(),
            'error_type': error_type
        })

    def get_summary(self) -> Dict[str, Any]:
        """Get performance summary"""
        summary = {}

        # Top 10 slowest functions
        time_metrics = {k: v for k, v in self.metrics.items()
                       if k.endswith('_time')}

        hotspots = []
        for func_name, timings in time_metrics.items():
            avg_time = sum(t['duration_ms'] for t in timings) / len(timings)
            hotspots.append({
                'function': func_name.replace('_time', ''),
                'avg_ms': avg_time,
                'call_count': len(timings)
            })

        summary['hotspots'] = sorted(hotspots,
                                     key=lambda x: x['avg_ms'],
                                     reverse=True)[:10]

        # Memory usage trends
        memory_metrics = {k: v for k, v in self.metrics.items()
                         if k.endswith('_memory')}

        summary['memory'] = {}
        for module_name, readings in memory_metrics.items():
            current = readings[-1]['memory_mb']
            peak = max(r['memory_mb'] for r in readings)

            summary['memory'][module_name.replace('_memory', '')] = {
                'current_mb': current,
                'peak_mb': peak
            }

        # Error rates
        error_metrics = {k: v for k, v in self.metrics.items()
                        if k.endswith('_errors')}

        summary['errors'] = {}
        for module_name, errors in error_metrics.items():
            error_types = defaultdict(int)
            for error in errors:
                error_types[error['error_type']] += 1

            summary['errors'][module_name.replace('_errors', '')] = dict(error_types)

        # Uptime
        summary['uptime_seconds'] = time.time() - self.start_time

        return summary

# Global monitor instance
_monitor = PerformanceMonitor()

def monitor_performance(func: Callable) -> Callable:
    """Decorator to automatically monitor function performance"""

    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        start_time = time.time()

        try:
            result = func(*args, **kwargs)
            return result
        except Exception as e:
            _monitor.record_error(func.__module__, type(e).__name__)
            raise
        finally:
            duration = time.time() - start_time
            _monitor.record_execution_time(
                f"{func.__module__}.{func.__name__}",
                duration
            )

    return wrapper

def get_performance_summary() -> Dict[str, Any]:
    """Get current performance summary"""
    return _monitor.get_summary()
```

### Usage in Modules

```python
"""
@file: processor_module.py
@purpose: Example module with telemetry
"""

from telemetry import monitor_performance, _monitor

class ProcessorModule:

    def __init__(self):
        self.name = "processor"

    @monitor_performance
    def process_data(self, data):
        """Process data with automatic performance monitoring"""
        # Processing logic here
        result = self._heavy_computation(data)

        # Record memory usage
        _monitor.record_memory_usage(self.name)

        return result

    @monitor_performance
    def _heavy_computation(self, data):
        """Compute-intensive operation"""
        # ... heavy work ...
        pass
```

### Alerting Thresholds

```python
"""
@file: alerting.py
@purpose: Performance alerting system
"""

from typing import Dict, Any, List
from telemetry import get_performance_summary

class PerformanceAlerter:
    """Monitor performance and raise alerts"""

    THRESHOLDS = {
        'performance_degradation_percent': 20,  # >20% slower
        'error_rate_percent': 1,                 # >1% errors
        'memory_growth_mb_per_hour': 100,        # >100MB/hour growth
        'health_check_failure_count': 3          # 3 consecutive failures
    }

    def check_alerts(self) -> List[Dict[str, Any]]:
        """Check for alert conditions"""
        alerts = []
        summary = get_performance_summary()

        # Check performance degradation
        for hotspot in summary.get('hotspots', []):
            if hasattr(self, f"_baseline_{hotspot['function']}"):
                baseline = getattr(self, f"_baseline_{hotspot['function']}")
                current = hotspot['avg_ms']

                if current > baseline * (1 + self.THRESHOLDS['performance_degradation_percent'] / 100):
                    alerts.append({
                        'type': 'PERFORMANCE_DEGRADATION',
                        'severity': 'WARNING',
                        'message': f"{hotspot['function']} is {((current/baseline - 1) * 100):.1f}% slower",
                        'current': current,
                        'baseline': baseline
                    })

        # Check error rates
        for module, errors in summary.get('errors', {}).items():
            total_errors = sum(errors.values())
            # Assuming we track call counts (simplified here)
            if total_errors > 10:  # More than 10 errors
                alerts.append({
                    'type': 'HIGH_ERROR_RATE',
                    'severity': 'ERROR',
                    'message': f"Module {module} has {total_errors} errors",
                    'errors': errors
                })

        # Check memory growth
        for module, memory_info in summary.get('memory', {}).items():
            current = memory_info['current_mb']
            peak = memory_info['peak_mb']

            # Simple heuristic: if current is close to peak, might be growing
            if current > peak * 0.9:
                alerts.append({
                    'type': 'MEMORY_GROWTH',
                    'severity': 'WARNING',
                    'message': f"Module {module} memory usage is {current:.1f}MB (near peak: {peak:.1f}MB)",
                    'current_mb': current,
                    'peak_mb': peak
                })

        return alerts
```

---

## Implementation Checklists

### New Project Setup Checklist

```markdown
## Initial Project Setup

- [ ] Initialize Git repository
- [ ] Create standard directory structure
      - [ ] src/
      - [ ] tests/ (unit, integration, system, performance, security, chaos)
      - [ ] docs/
      - [ ] deploy/
      - [ ] config/ (schema, active, backups, migrations)
      - [ ] logs/

- [ ] Skeleton Implementation
      - [ ] Create skeleton core with module registry
      - [ ] Implement self-diagnostic system
      - [ ] Add health monitoring
      - [ ] Implement module isolation logic
      - [ ] Add logging system

- [ ] Module Interface Specification
      - [ ] Define ModuleInterface base class
      - [ ] Create .interface file schema
      - [ ] Implement version checking
      - [ ] Add contract testing framework

- [ ] Deployment Setup
      - [ ] Create deploy/ directory structure
      - [ ] Write deploy.sh with all features:
            - [ ] Pre-flight checks
            - [ ] Interactive/automated modes
            - [ ] Progress reporting
            - [ ] Error handling with remediation
            - [ ] Health verification
            - [ ] Rollback capability
      - [ ] Write README_DEPLOY.md
      - [ ] Test deployment in dev environment

- [ ] Configuration Management
      - [ ] Configure automatic backup system
      - [ ] Set up backup schedules
      - [ ] Create first config schema
      - [ ] Test backup and restore
      - [ ] Write first migration script template

- [ ] Performance Baseline
      - [ ] Set up performance monitoring
      - [ ] Run initial benchmarks
      - [ ] Document baseline metrics
      - [ ] Define performance targets

- [ ] Dependency Management
      - [ ] Create requirements.txt / package.json
      - [ ] Document each dependency with justification
      - [ ] Set up vulnerability scanning
      - [ ] Configure dependency update alerts

- [ ] CI/CD Pipeline
      - [ ] Set up pre-commit hooks
      - [ ] Configure CI workflow
      - [ ] Add automated tests
      - [ ] Add security scanning
      - [ ] Configure coverage reporting
      - [ ] Set up nightly performance tests

- [ ] Documentation
      - [ ] Copy CODING_STANDARDS.md to repo
      - [ ] Write project README.md
      - [ ] Document architecture decisions
      - [ ] Create API documentation
      - [ ] Write contribution guidelines

- [ ] Initial Testing
      - [ ] Create first backup
      - [ ] Test restore procedure
      - [ ] Test deployment script (dry run)
      - [ ] Verify all health checks work
```

### Per-Module Development Checklist

```markdown
## Module: [Module Name]

### Documentation
- [ ] Header comment complete with all required fields
      - [ ] @file, @purpose, @dependencies
      - [ ] @version, @last_modified, @author
      - [ ] @performance_notes, @module_type

- [ ] Inline documentation for complex logic
- [ ] API documentation generated

### Implementation
- [ ] Implements standard ModuleInterface
      - [ ] get_metadata()
      - [ ] health_check()
      - [ ] initialize()
      - [ ] shutdown()
      - [ ] get_api_version()

- [ ] .interface file created
- [ ] Dependencies documented and minimized
      - [ ] Each dependency justified in DEPENDENCIES.md
      - [ ] Total count: _____ (max 10)

- [ ] Error handling implemented
      - [ ] All exceptions caught and logged
      - [ ] Graceful degradation
      - [ ] Clear error messages

### Testing
- [ ] Unit tests written
      - [ ] Coverage: ____% (target: >80%)
      - [ ] Edge cases covered
      - [ ] Error conditions tested
      - [ ] Performance test included

- [ ] Integration test with skeleton passed
- [ ] Health check function tested
- [ ] Module isolation tested (fault injection)

### Performance
- [ ] Baseline performance established
      - [ ] Execution time: _____ ms
      - [ ] Memory usage: _____ MB
      - [ ] Bottlenecks identified: _____

- [ ] Alternative approaches considered
      - [ ] Approach 1: _____ (rejected because: _____)
      - [ ] Approach 2: _____ (rejected because: _____)
      - [ ] Selected: _____ (rationale: _____)

- [ ] C++ consideration evaluated
      - [ ] Profiling shows >15% time: Yes / No
      - [ ] Expected speedup: ____x
      - [ ] Decision: Python / C++ / Hybrid
      - [ ] Rationale: _____

- [ ] Performance benchmarks pass
      - [ ] No regression from baseline
      - [ ] Meets target metrics

### Quality Gates
- [ ] Linting passed (pylint / eslint)
- [ ] Type checking passed (mypy / typescript)
- [ ] Code formatting passed (black / prettier)
- [ ] Security scan clean (bandit / npm audit)
- [ ] Code review approved

### Integration
- [ ] Module registered with skeleton successfully
- [ ] Initialization order verified
- [ ] Communication protocol tested
- [ ] Resource requirements declared
- [ ] API version specified: _____

### Deployment
- [ ] Added to deployment script
- [ ] Configuration schema updated
- [ ] Migration script created (if config changes)
- [ ] Deployment tested in dev environment
```

### Pre-Deployment Checklist

```markdown
## Pre-Deployment Verification

### Code Quality
- [ ] All tests passing
      - [ ] Unit tests: ✓
      - [ ] Integration tests: ✓
      - [ ] System tests: ✓
      - [ ] Performance tests: ✓
      - [ ] Security tests: ✓

- [ ] Code coverage ≥80%
- [ ] No critical security vulnerabilities
- [ ] No linting errors
- [ ] Type checking clean

### Performance
- [ ] Performance targets met
      - [ ] Target 1: _____ (Status: _____)
      - [ ] Target 2: _____ (Status: _____)
- [ ] No performance regressions
- [ ] Exceptions documented (if any)

### Configuration
- [ ] Configuration backup created
      - [ ] Backup file: _____
      - [ ] Verified: ✓
- [ ] Config migration tested (if applicable)
- [ ] Environment variables documented

### Deployment Readiness
- [ ] Deployment script tested (dry-run)
      - [ ] Pre-flight checks passed: ✓
      - [ ] All dependencies available: ✓
      - [ ] Target environment accessible: ✓

- [ ] Rollback procedure tested
      - [ ] Rollback script works: ✓
      - [ ] Recovery time: _____ minutes

- [ ] Monitoring configured
      - [ ] Metrics collection: ✓
      - [ ] Alerts configured: ✓
      - [ ] Dashboard updated: ✓

### Documentation
- [ ] CHANGELOG.md updated
- [ ] API documentation current
- [ ] Deployment README updated
- [ ] Known issues documented

### Communication
- [ ] Stakeholders notified
- [ ] Maintenance window scheduled (if needed)
- [ ] Rollback plan communicated
- [ ] Support team briefed

### Final Checks
- [ ] Production database backed up
- [ ] SSL certificates valid
- [ ] DNS records verified
- [ ] Load balancer configured
- [ ] Firewall rules updated

### Post-Deployment Plan
- [ ] Health check verification steps defined
- [ ] Smoke test suite ready
- [ ] Monitoring dashboard ready
- [ ] On-call engineer assigned
- [ ] Communication channel established (#deployments)

### Sign-off
- [ ] Developer: _____ (Date: _____)
- [ ] Tech Lead: _____ (Date: _____)
- [ ] DevOps: _____ (Date: _____)
```

---

## Standard Prompt for AI-Assisted Development

When starting any new project or feature, use this prompt with AI assistants:

```markdown
I'm implementing [FEATURE/PROJECT NAME] following the Universal Coding Standard Framework from CODING_STANDARDS.md.

## Project Context
- Project Type: [web app / CLI tool / library / service]
- Primary Language: [Python / C++ / JavaScript / etc.]
- Target Environment: [Linux / Docker / Cloud / etc.]
- Performance Requirements: [real-time / batch / standard]

## Requirements
Please help me design and implement this following ALL standards including:

### 1. Architecture Design
- Design skeleton-module separation with clear boundaries
- Define module interfaces with versioning (ModuleInterface compliance)
- Specify self-diagnostic requirements
  - Health checks for each module
  - Resource monitoring
  - Fault isolation strategy
- Plan module communication protocol (event bus / message passing)
- Identify performance-critical paths

### 2. Implementation
- Write complete header documentation for each file
  - @file, @purpose, @dependencies, @version
  - @last_modified, @author, @performance_notes, @module_type
- Implement standard ModuleInterface:
  - get_metadata(), health_check(), initialize()
  - shutdown(), get_api_version()
- Include comprehensive inline documentation (WHY not just WHAT)
- Minimize dependencies (justify each one, max 10 per module)
- Add error handling with specific, actionable messages

### 3. Optimization Strategy
- Profile initial implementation (establish baseline)
- Suggest 3+ alternative approaches for critical paths
- Benchmark each alternative with trade-off analysis
- Identify C++ candidates:
  - Functions consuming >15% execution time
  - Expected speedup: minimum 10x
  - Compute-bound operations
- Document all performance decisions with data

### 4. Deployment
- Create single-command deployment script (deploy.sh)
- Include comprehensive pre-flight checks:
  - OS compatibility, dependencies, ports, disk space
  - Permissions, conflicting processes, network
- Implement detailed error messages:
  - Specific root cause
  - Multiple possible causes
  - Actionable remediation steps
  - Commands to diagnose
  - Rollback procedure
- Add progress reporting (step X/N, time estimates)
- Include health verification post-deployment
- Write troubleshooting guide (README_DEPLOY.md)

### 5. Configuration & Backup
- Design configuration schema (JSON Schema)
- Implement automatic backup system:
  - Trigger before changes, deployments, daily
  - Retention policy (30 days)
  - Integrity verification (checksums)
- Create version migration logic:
  - Forward migration (old → new)
  - Backward compatibility handling
  - Rollback support
- Handle version mismatches gracefully

### 6. Testing
- Unit tests (target: >80% coverage)
  - Test happy path and edge cases
  - Test error conditions
  - Test performance (no regression)
- Integration tests:
  - Module interface compliance
  - Communication protocol
  - Dependency resolution
- System tests:
  - End-to-end workflows
  - Deployment procedures
- Chaos tests:
  - Fault injection
  - Module isolation verification

### 7. Performance Monitoring
- Built-in telemetry:
  - Execution time tracking
  - Memory usage monitoring
  - Error rate tracking
- Alert thresholds:
  - >20% performance degradation
  - >1% error rate
  - Abnormal memory growth
- Performance summary dashboard

## Deliverables
1. Complete implementation with all files
2. Deployment script with all safety features
3. Configuration management system
4. Comprehensive test suite
5. Performance benchmarks and comparisons
6. Documentation:
   - Architecture decisions
   - API documentation
   - Deployment guide
   - Troubleshooting guide

## Constraints
- MUST follow ModuleInterface standard
- MUST implement health checks
- MUST minimize dependencies (justify each)
- MUST include performance benchmarks
- MUST have single-command deployment
- MUST handle errors transparently
- MUST support configuration versioning

Please start with the architecture design and show me the skeleton-module breakdown before proceeding with implementation.
```

### Example Usage

```markdown
I'm implementing a high-performance log processing system following the Universal Coding Standard Framework from CODING_STANDARDS.md.

## Project Context
- Project Type: Service
- Primary Language: Python with C++ for hot paths
- Target Environment: Docker on Linux
- Performance Requirements: Process 100K logs/sec

[Include all sections from standard prompt above]

Specific requirements for this project:
- Must parse JSON logs
- Must support multiple output formats (Elasticsearch, S3, stdout)
- Must handle log rotation
- Must support filtering rules
- Must be horizontally scalable

Please start with the architecture design...
```

---

## Version History

```
v1.0.0 (2025-01-18)
- Initial release of Universal Coding Standard Framework
- Core principles established
- Documentation standards defined
- Skeleton + Module architecture specified
- Optimization protocol documented
- Deployment architecture standardized
- Dependency minimization strategy defined
- C++ performance module guidelines added
- Configuration management system designed
- Testing requirements established
- Performance monitoring framework created
```

---

## Contributing

To propose changes to these standards:

1. Create a feature branch
2. Update this document with proposed changes
3. Provide rationale for changes
4. Submit pull request with examples
5. Discuss in team review
6. Update version number if approved

---

## License

[Specify your license here]

---

## Support

For questions or clarifications about these standards:
- Create an issue in the repository
- Contact the architecture team
- Refer to example implementations in `/examples` directory

---

**Remember**: These standards exist to ensure consistency, maintainability, and quality across all projects. They should be followed but can be adapted when there's a clear, documented reason to do so.
