# 📖 Reference Examples

These are complete, working Ceedling projects that demonstrate different levels of complexity. Use them as reference implementations when working on exercises or building your own projects.

## 🏗️ Project Overview

### 💡 Simple LED Control
**Path:** `simple-led-control/`
- **Complexity:** Beginner-friendly
- **Files:** 2 source files, 1 test file
- **Focus:** Basic testing concepts

A straightforward LED control module that demonstrates:
- Basic unit test structure
- Simple assertions with Unity
- Minimal mocking requirements
- Clear test organization

**Perfect for:** Your first look at a working Ceedling project

### 🌡️ Complex Temp Sensor
**Path:** `complex-temp-sensor/`
- **Complexity:** Production-ready example
- **Files:** 47+ source files, 15+ test files
- **Focus:** Advanced testing patterns

A comprehensive temperature sensor system that demonstrates:
- Multiple module integration
- Hardware abstraction layer testing
- Complex mocking scenarios
- Advanced Ceedling configurations
- Coverage reporting
- Real-world embedded architecture

**Perfect for:** Understanding how testing scales to large projects

## 🎯 How to Use These Examples

### For Learning
1. **Start Simple** - Explore the LED control first
2. **Understand Structure** - See how tests are organized
3. **Run the Tests** - Execute `ceedling test:all`
4. **Study the Output** - Understand what success looks like
5. **Modify and Experiment** - Break things and fix them

### For Reference
- **Copy Patterns** - Use similar test structures in your code
- **Understand Configurations** - See how `project.yml` is set up
- **Learn Best Practices** - See clean, testable embedded code
- **Get Inspiration** - Ideas for your own testing approaches

## 🚀 Quick Start

### LED Control Example
```bash
cd reference-examples/simple-led-control/
ceedling test:all
```

### Temperature Sensor Example
```bash
cd reference-examples/complex-temp-sensor/
ceedling test:all
```

## 📊 Comparison Matrix

| Aspect | LED Control | Temp Sensor |
|--------|-------------|-------------|
| **Learning Curve** | Gentle | Steep |
| **Code Complexity** | Low | High |
| **Test Coverage** | Basic | Comprehensive |
| **Mocking Usage** | Minimal | Extensive |
| **Real-world Relevance** | Educational | Production-like |
| **Time to Understand** | 30 minutes | 2-3 hours |

## 💡 Study Suggestions

### For the LED Control Example:
1. Read the source code first (`src/led_control.c`)
2. Examine the test file (`test/test_led_control.c`)
3. Run the tests and observe output
4. Modify a test and see it fail
5. Fix the test and see it pass

### For the Temperature Sensor Example:
1. Start with the README in that directory
2. Explore the project structure
3. Focus on a single module (e.g., `TemperatureCalculator`)
4. Study the corresponding test file
5. Look at the mocking usage patterns
6. Examine the `project.yml` configuration

## 🔧 Configuration Insights

Both examples show different Ceedling configurations:

**Simple LED Control:**
- Basic mocking enabled
- Standard Unity assertions
- Simple build configuration

**Complex Temp Sensor:**
- Advanced mocking patterns
- Custom test support files
- Coverage reporting enabled
- Multiple test suites

## ⚠️ Important Notes

### Don't Copy Blindly
- Understand **why** certain patterns are used
- Adapt patterns to **your specific needs**
- Consider the **complexity trade-offs**

### Use as Starting Points
- **Fork and modify** for your projects
- **Extract patterns** rather than entire implementations
- **Learn the principles** behind the code

### When to Reference Each

**Use LED Control when:**
- Starting your first embedded testing project
- Teaching others basic concepts
- Needing a simple example to modify

**Use Temperature Sensor when:**
- Building complex embedded systems
- Looking for advanced testing patterns
- Understanding production-level test organization

---

**Next Step:** Explore `simple-led-control/README.md` or dive into the exercises!