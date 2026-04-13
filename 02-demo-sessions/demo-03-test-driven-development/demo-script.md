# 🔴 Demo 3: Quick Reference Script

**Total Time: 45 minutes**

## 📝 Opening Philosophy (5 min)
```
Traditional Approach:
1. Think implementation → 2. Write code → 3. Debug → 4. Add tests → 5. Ship

TDD Approach:
1. Think behavior → 2. Write test (RED) → 3. Make pass (GREEN) → 4. Improve (REFACTOR) → Repeat

"Embedded devs say: 'No time for tests first!' But consider:
- Debugging on hardware = 10x slower than fixing failing tests
- Design flaws at integration = expensive to fix
- Unclear requirements = rework and missed deadlines
- Untested error paths = field failures"
```

## 🔄 TDD Cycle (5 min)

### The Three Phases
```
🔴 RED: Write failing test (define behavior, prove test works)
🟢 GREEN: Make it pass (simplest code, don't optimize yet)
🔧 REFACTOR: Improve design (tests protect during changes)

Mantra: "Red, Green, Refactor - never skip a step!"
        "Small steps - one test at a time!"
```

## 🎯 The Problem (2 min)
```
PID Controller for temperature regulation:
1. Calculate output from setpoint and current value
2. Support Kp, Ki, Kd gains
3. Handle integral windup protection
4. Reset controller state
5. Limit output to min/max

Perfect for TDD:
- Clear requirements ✅
- Testable math ✅
- Edge cases ✅
```

## ⚡ Setup (2 min)
```bash
mkdir demo_pid_tdd
cd demo_pid_tdd
ceedling new demo_pid_tdd
cd demo_pid_tdd
```

## 🔴 RED: First Test (4 min)
```c
// test/test_pid_controller.c
#include "unity.h"
#include "pid_controller.h"  // Doesn't exist yet!

void test_pid_calculate_should_return_proportional_error_when_only_kp_set(void) {
    // Arrange
    pid_controller_t pid;
    pid_init(&pid, 1.0f, 0.0f, 0.0f);  // Kp=1.0, Ki=0, Kd=0

    float setpoint = 25.0f;
    float current = 20.0f;
    float expected_output = 5.0f;  // Error=5, Output=Kp*error=1.0*5=5

    // Act
    float output = pid_calculate(&pid, setpoint, current, 0.1f);

    // Assert
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_output, output);
}
```

```bash
ceedling test:test_pid_controller  # FAILS - perfect RED!
```

## 🟢 GREEN: Make It Pass (6 min)

### Header (2 min)
```c
// src/pid_controller.h
typedef struct {
    float kp, ki, kd;
    float integral;
    float last_error;
} pid_controller_t;

void pid_init(pid_controller_t* pid, float kp, float ki, float kd);
float pid_calculate(pid_controller_t* pid, float setpoint, float current, float dt);
```

### Minimal Implementation (4 min)
```c
// src/pid_controller.c
void pid_init(pid_controller_t* pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0.0f;
    pid->last_error = 0.0f;
}

float pid_calculate(pid_controller_t* pid, float setpoint, float current, float dt) {
    float error = setpoint - current;
    return pid->kp * error;  // Only P term - minimal!
}
```

```bash
ceedling test:test_pid_controller  # GREEN!
```

## 🔴 RED: Add Integral (3 min)
```c
void test_pid_calculate_should_accumulate_integral_error_when_ki_set(void) {
    pid_controller_t pid;
    pid_init(&pid, 0.0f, 1.0f, 0.0f);  // Ki=1.0

    float setpoint = 25.0f, current = 20.0f, dt = 0.1f;

    float output1 = pid_calculate(&pid, setpoint, current, dt);
    float output2 = pid_calculate(&pid, setpoint, current, dt);

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, output1);  // Ki*error*dt=1*5*0.1=0.5
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, output2);  // 0.5+0.5=1.0
}
```

## 🟢 GREEN: Add Integral (3 min)
```c
float pid_calculate(pid_controller_t* pid, float setpoint, float current, float dt) {
    float error = setpoint - current;

    // Add integral term
    pid->integral += error * dt;

    return pid->kp * error + pid->ki * pid->integral;
}
```

## 🔴 RED: Add Derivative (4 min)
```c
void test_pid_calculate_should_compute_derivative_when_kd_set(void) {
    pid_controller_t pid;
    pid_init(&pid, 0.0f, 0.0f, 1.0f);  // Kd=1.0

    float dt = 0.1f;
    float output1 = pid_calculate(&pid, 25.0f, 20.0f, dt);  // error=5
    float output2 = pid_calculate(&pid, 25.0f, 15.0f, dt);  // error=10, deriv=(10-5)/0.1=50

    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, output1);   // No previous error
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 50.0f, output2);  // Kd*derivative=1*50=50
}
```

## 🟢 GREEN: Complete PID (2 min)
```c
float pid_calculate(pid_controller_t* pid, float setpoint, float current, float dt) {
    float error = setpoint - current;

    float p_term = pid->kp * error;

    pid->integral += error * dt;
    float i_term = pid->ki * pid->integral;

    float derivative = (error - pid->last_error) / dt;
    float d_term = pid->kd * derivative;
    pid->last_error = error;

    return p_term + i_term + d_term;
}
```

## 🔧 REFACTOR: Improve Design (5 min)
```c
// Add output limiting and helper functions
void pid_set_output_limits(pid_controller_t* pid, float min, float max);

static float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// Update calculate to use clamp
float output = p_term + i_term + d_term;
return clamp(output, pid->output_min, pid->output_max);
```

```bash
ceedling test:test_pid_controller  # Still GREEN!
```

## 💬 Discussion (5 min)

### What We Accomplished
- ✅ Clear API design driven by tests
- ✅ Complete PID functionality (P, I, D terms)
- ✅ Safe refactoring with test protection
- ✅ Living documentation in test form
- ✅ Confidence - proved it works step by step

### When to Use TDD in Embedded

#### TDD Works Great For:
- ✅ **Algorithms** - Math functions, filters, transformations
- ✅ **Business Logic** - Rules, decisions, protocols
- ✅ **Clear Requirements** - Known inputs/outputs, edge cases

#### TDD is Challenging For:
- ⚠️ **Hardware Init** - Register setup, interrupts
- ⚠️ **Real-Time Critical** - ISRs, time-critical loops
- ⚠️ **Exploratory** - Hardware bring-up, proof of concept

#### Hybrid Approach:
```
Application Layer  ← TDD (business logic)
Service Layer      ← TDD (data processing)
Driver Layer       ← Traditional (hardware interfaces)
HAL Layer         ← Traditional (registers)
```

### Common Questions
**"TDD slows development?"** → "Slower initially, faster overall due to less debugging"
**"Requirements change?"** → "Change tests first, then code"
**"What test to write first?"** → "Simplest behavior that provides value"
**"Performance requirements?"** → "Make it work (TDD), then make it fast"

---

## 🎯 Instructor Notes

### Keep Energy High
- **Celebrate each GREEN** - Build excitement
- **Move at good pace** - Don't get bogged down in details
- **Show confidence** in refactoring with test protection
- **Ask prediction questions** - "What test should we write next?"

### Handle Issues
- **Compilation errors**: Have code snippets ready to paste
- **Test failures**: Use as teaching moment for debugging
- **Getting stuck**: "Let me think about simplest behavior to test..."

### Key Messages
- Tests **drive design** decisions
- **Small steps** are powerful
- **Refactoring is safe** with tests
- TDD is about **design**, not just testing

---

**End Goal**: Attendees see TDD as design methodology, not just testing approach!