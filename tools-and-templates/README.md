# 🛠️ Tools and Templates

Reusable templates, project scaffolding, and common patterns to accelerate your unit testing adoption. Use these tools to get started quickly and maintain consistency across projects.

## 📁 Directory Structure

```
tools-and-templates/
├── project-template/        # Complete Ceedling project template
├── test-templates/          # Common test file templates
├── common-patterns/         # Reusable code patterns
└── README.md               # This file
```

## 🚀 Project Template

**Path:** `project-template/`

A complete Ceedling project skeleton that you can copy and customize for new projects.

### What's Included
- ✅ **Pre-configured project.yml** - Optimized Ceedling settings
- ✅ **Directory structure** - Standard embedded project layout
- ✅ **Sample source files** - Template for your modules
- ✅ **Sample test files** - Test templates with common patterns
- ✅ **Build scripts** - Automation helpers
- ✅ **Documentation** - README and getting started guide

### Quick Start
```bash
# Copy template to new project
cp -r tools-and-templates/project-template/ my-new-project/
cd my-new-project/

# Customize for your project
# Edit project.yml, README.md, and source files

# Verify everything works
ceedling test:all
```

### Customization Points
- **Project name** in `project.yml` and documentation
- **Source directories** for your specific architecture
- **Compiler flags** for your target microcontroller
- **Plugin selection** based on your needs (coverage, mocking, etc.)

## 📝 Test Templates

**Path:** `test-templates/`

Common test file patterns that you can copy and modify for your modules.

### Available Templates

#### 🧮 `basic_module_test.c`
- Simple module testing template
- Basic setup/teardown
- Common assertion patterns
- **Use for:** Simple utility modules

#### 🎭 `hardware_driver_test.c`
- Hardware abstraction testing
- Mock setup patterns
- Error condition testing
- **Use for:** Device drivers, HAL modules

#### 🔄 `state_machine_test.c`
- State machine testing patterns
- Transition validation
- State persistence testing
- **Use for:** Protocol handlers, control logic

#### 🔗 `integration_test.c`
- Multi-module testing template
- Component interaction patterns
- End-to-end scenario testing
- **Use for:** Feature-level testing

#### ⚡ `performance_test.c`
- Timing and performance validation
- Resource usage testing
- Benchmark patterns
- **Use for:** Real-time critical code

### Template Usage
```bash
# Copy template for your module
cp test-templates/basic_module_test.c test/test_my_module.c

# Customize the template:
# 1. Replace placeholder names
# 2. Add your specific test cases
# 3. Configure mocks if needed
# 4. Run and verify
ceedling test:test_my_module
```

## 🎯 Common Patterns

**Path:** `common-patterns/`

Reusable code snippets and patterns for common embedded testing scenarios.

### Pattern Categories

#### 🏗️ Test Structure Patterns
- **Arrange-Act-Assert** - Standard test organization
- **Given-When-Then** - BDD-style test structure
- **Setup-Exercise-Verify-Teardown** - Complete test lifecycle

#### 🎭 Mocking Patterns
- **Hardware register mocking** - Peripheral simulation
- **Communication protocol mocking** - SPI/I2C/UART patterns
- **Callback function mocking** - Event-driven testing
- **Time-dependent mocking** - Timer and delay simulation

#### 📊 Data Testing Patterns
- **Boundary value testing** - Edge case validation
- **Parameterized testing** - Data-driven tests
- **Error injection patterns** - Fault tolerance testing
- **Property-based testing** - Invariant validation

#### 🔧 Assertion Patterns
- **Custom assertions** - Domain-specific validations
- **Floating-point comparisons** - Tolerance-based assertions
- **Memory testing** - Buffer and allocation validation
- **State verification** - Complex object validation

### Pattern Usage Example
```c
// Copy pattern snippet
#include "common-patterns/boundary_value_testing.h"

// Use in your test
void test_temperature_sensor_boundary_conditions(void)
{
    BOUNDARY_TEST_SUITE(temperature_readings) {
        BOUNDARY_CASE(-40, TEMP_ERROR_UNDERRANGE),
        BOUNDARY_CASE(85, TEMP_OK),
        BOUNDARY_CASE(86, TEMP_ERROR_OVERRANGE),
    };

    RUN_BOUNDARY_TESTS(temperature_readings, temperature_sensor_read);
}
```

## 🔧 Utility Scripts

### Build Automation
- **new_project.sh** - Create new project from template
- **run_tests.sh** - Run tests with reporting
- **generate_mocks.sh** - Regenerate all mocks
- **coverage_report.sh** - Generate and open coverage report

### Code Generation
- **new_module.sh** - Generate module and test files
- **add_mock.sh** - Add new mock to existing project
- **update_tests.sh** - Refresh test runners

### Quality Assurance
- **check_coverage.sh** - Validate coverage thresholds
- **run_static_analysis.sh** - Execute static analysis tools
- **validate_tests.sh** - Ensure test quality standards

## 📋 Configuration Examples

### project.yml Variations

#### **Minimal Configuration**
- Basic Unity testing
- No mocking
- Simple build setup
- **Best for:** Learning, simple projects

#### **Standard Configuration**
- Unity + CMock
- Basic coverage reporting
- Standard plugins
- **Best for:** Most embedded projects

#### **Advanced Configuration**
- Full plugin suite
- Custom build steps
- Multiple targets
- CI/CD integration
- **Best for:** Complex, production projects

### Compiler Configurations
- **ARM Cortex-M** - GCC ARM toolchain setup
- **AVR** - AVR-GCC configuration
- **x86** - Native development setup
- **Cross-platform** - Multiple target support

## 🎓 Best Practices Guide

### Project Organization
- ✅ **Consistent structure** across all projects
- ✅ **Clear naming conventions** for files and functions
- ✅ **Modular architecture** with clean interfaces
- ✅ **Documentation standards** for maintainability

### Test Organization
- ✅ **One test file per source file** as starting point
- ✅ **Descriptive test names** that explain scenarios
- ✅ **Logical test grouping** within test files
- ✅ **Shared test utilities** for common operations

### Build Configuration
- ✅ **Version control** for project.yml changes
- ✅ **Environment independence** - works anywhere
- ✅ **Fast feedback** - quick test execution
- ✅ **Clear error messages** when things fail

## 🚀 Getting Started Workflow

### For New Projects
1. **Copy project template**
2. **Customize configuration** for your target
3. **Create first module and test**
4. **Verify build pipeline works**
5. **Add your implementation**

### For Existing Projects
1. **Choose appropriate test template**
2. **Copy relevant patterns**
3. **Adapt to your existing structure**
4. **Start with simple tests**
5. **Gradually increase coverage**

## 📈 Scaling Considerations

### Small Projects (< 10 modules)
- Use **basic templates** and patterns
- Focus on **core functionality** testing
- Keep **configuration simple**

### Medium Projects (10-50 modules)
- Adopt **modular test organization**
- Use **common patterns** extensively
- Implement **build automation**

### Large Projects (50+ modules)
- Require **standardized templates**
- Enforce **coding standards**
- Implement **CI/CD integration**

---

**Perfect for:** Starting new projects, maintaining consistency, accelerating development