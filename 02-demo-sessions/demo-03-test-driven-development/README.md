# 🔴 Demo 3: Test-Driven Development (45 minutes)

**Red-Green-Refactor: Writing Tests First for Better Design**

## 🎯 Demo Objectives

By the end of this demo, attendees will:
- ✅ Understand the TDD cycle: Red → Green → Refactor
- ✅ See how writing tests first improves API design
- ✅ Experience how TDD catches design problems early
- ✅ Learn when to apply TDD in embedded development
- ✅ Recognize the difference between TDD and test-after approaches

## 📋 Prerequisites

**For Instructor:**
- Ceedling configured and working
- PID controller or state machine example prepared
- Confidence with live coding under pressure

**For Attendees:**
- Familiar with basic unit testing (Demo 1)
- Understanding of control systems helpful but not required
- Open mind about writing tests before code

## ⏱️ Demo Timeline

| Time | Activity | Duration |
|------|----------|----------|
| 0-5 min | **TDD Philosophy** - Why test-first changes everything | 5 min |
| 5-10 min | **The TDD Cycle** - Red, Green, Refactor explained | 5 min |
| 10-35 min | **Live TDD Demo** - Build PID controller test-first | 25 min |
| 35-40 min | **Refactoring** - Improve design while tests stay green | 5 min |
| 40-45 min | **Discussion** - When to use TDD, when not to | 5 min |

## 🎬 Demo Script

### Opening Philosophy (5 minutes)

#### The Traditional Approach
```
1. Think about implementation
2. Write code
3. Debug until it works
4. Add tests (maybe)
5. Ship and hope
```

#### The TDD Approach
```
1. Think about behavior
2. Write test (RED)
3. Make it pass (GREEN)
4. Improve design (REFACTOR)
5. Repeat with confidence
```

#### Why This Matters for Embedded

> **"Embedded developers often say: 'I don't have time to write tests first!'"**
> **"But consider this:"**
>
> 📍 **Debugging on hardware** takes 10x longer than fixing failing tests
> 📍 **Design flaws** discovered during integration are expensive to fix
> 📍 **Unclear requirements** lead to rework and missed deadlines
> 📍 **Untested error paths** cause field failures

> **"TDD addresses all of these by forcing us to think about behavior before implementation."**

### The TDD Cycle Deep Dive (5 minutes)

#### 🔴 RED Phase
> **"Write a test that fails"**
- Define the desired behavior
- Write the minimal test to express it
- Run test and see it fail (RED)
- **Key:** Failing test proves it's actually testing something

#### 🟢 GREEN Phase
> **"Make the test pass"**
- Write the simplest code that makes the test pass
- Don't worry about perfect design yet
- Run test and see it pass (GREEN)
- **Key:** Focus on making it work, not making it beautiful

#### 🔧 REFACTOR Phase
> **"Improve the design"**
- Clean up code while keeping tests green
- Remove duplication
- Improve naming and structure
- **Key:** Tests protect you during refactoring

#### The TDD Mantra
> **"Red, Green, Refactor - never skip a step!"**
> **"Take small steps - one test at a time!"**

### Part 1: Live TDD Demo - PID Controller (25 minutes)

#### The Problem Statement (2 minutes)

> **"We need a PID controller for temperature regulation:"**

```c
// Requirements:
// 1. Calculate PID output from setpoint and current value
// 2. Support configurable Kp, Ki, Kd gains
// 3. Handle integral windup protection
// 4. Reset controller state
// 5. Limit output to min/max values
```

> **"Perfect for TDD because:**
> - **Clear requirements** - We know what it should do
> - **Testable math** - Pure function behavior
> - **Edge cases** - Windup, limits, reset conditions"**

#### Setup Project (2 minutes)

```bash
# [INSTRUCTOR TYPES LIVE]
mkdir demo_pid_tdd
cd demo_pid_tdd
ceedling new demo_pid_tdd
cd demo_pid_tdd
```

#### RED: First Test (4 minutes)

> **"Let's start with the most basic behavior: PID calculation with only P term"**

```bash
vim test/test_pid_controller.c
```

```c
#include "unity.h"
#include "pid_controller.h"  // Doesn't exist yet!

void setUp(void) {
    // Fresh state for each test
}

void tearDown(void) {
    // Cleanup
}

void test_pid_calculate_should_return_proportional_error_when_only_kp_set(void) {
    // Arrange
    pid_controller_t pid;
    pid_init(&pid, 1.0f, 0.0f, 0.0f);  // Kp=1.0, Ki=0, Kd=0

    float setpoint = 25.0f;
    float current = 20.0f;
    float expected_output = 5.0f;  // Error = 25 - 20 = 5, Output = Kp * error = 1.0 * 5 = 5

    // Act
    float output = pid_calculate(&pid, setpoint, current, 0.1f);  // 0.1s time step

    // Assert
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_output, output);
}
```

> **"Notice:"**
> - **We're defining the API as we write the test**
> - **The test expresses the requirement clearly**
> - **We chose simple values for easy verification**

#### Run Test - See RED (1 minute)

```bash
ceedling test:test_pid_controller
```

> **"FAILS! Header file doesn't exist. This is perfect RED - test fails for the right reason."**

#### GREEN: Minimal Implementation (6 minutes)

```bash
vim src/pid_controller.h
```

```c
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

typedef struct {
    float kp, ki, kd;
    float integral;
    float last_error;
} pid_controller_t;

void pid_init(pid_controller_t* pid, float kp, float ki, float kd);
float pid_calculate(pid_controller_t* pid, float setpoint, float current, float dt);

#endif
```

```bash
vim src/pid_controller.c
```

```c
#include "pid_controller.h"

void pid_init(pid_controller_t* pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0.0f;
    pid->last_error = 0.0f;
}

float pid_calculate(pid_controller_t* pid, float setpoint, float current, float dt) {
    float error = setpoint - current;
    return pid->kp * error;  // Only P term - minimal implementation!
}
```

```bash
ceedling test:test_pid_controller
```

> **"GREEN! Test passes with minimal implementation. We only implemented P term because that's all the test required."**

#### RED: Add Integral Term Test (3 minutes)

```c
void test_pid_calculate_should_accumulate_integral_error_when_ki_set(void) {
    // Arrange
    pid_controller_t pid;
    pid_init(&pid, 0.0f, 1.0f, 0.0f);  // Kp=0, Ki=1.0, Kd=0

    float setpoint = 25.0f;
    float current = 20.0f;
    float dt = 0.1f;

    // Act - Call twice to accumulate integral
    float output1 = pid_calculate(&pid, setpoint, current, dt);
    float output2 = pid_calculate(&pid, setpoint, current, dt);

    // Assert
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.5f, output1);  // Ki * error * dt = 1.0 * 5 * 0.1 = 0.5
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, output2);  // Previous + new = 0.5 + 0.5 = 1.0
}
```

> **"This test defines how integral term should work - accumulate error over time."**

#### GREEN: Add Integral Implementation (3 minutes)

```c
float pid_calculate(pid_controller_t* pid, float setpoint, float current, float dt) {
    float error = setpoint - current;

    // Add integral term
    pid->integral += error * dt;

    return pid->kp * error + pid->ki * pid->integral;
}
```

```bash
ceedling test:test_pid_controller
```

> **"GREEN! Both tests pass. We added integral without breaking proportional."**

#### RED: Add Derivative Term Test (4 minutes)

```c
void test_pid_calculate_should_compute_derivative_when_kd_set(void) {
    // Arrange
    pid_controller_t pid;
    pid_init(&pid, 0.0f, 0.0f, 1.0f);  // Kp=0, Ki=0, Kd=1.0

    float dt = 0.1f;

    // Act - Two calls with different errors to create derivative
    float output1 = pid_calculate(&pid, 25.0f, 20.0f, dt);  // error = 5
    float output2 = pid_calculate(&pid, 25.0f, 15.0f, dt);  // error = 10, derivative = (10-5)/0.1 = 50

    // Assert
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, output1);   // No previous error, derivative = 0
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 50.0f, output2);  // Kd * derivative = 1.0 * 50 = 50
}
```

#### GREEN: Complete PID Implementation (2 minutes)

```c
float pid_calculate(pid_controller_t* pid, float setpoint, float current, float dt) {
    float error = setpoint - current;

    // Proportional term
    float p_term = pid->kp * error;

    // Integral term
    pid->integral += error * dt;
    float i_term = pid->ki * pid->integral;

    // Derivative term
    float derivative = (error - pid->last_error) / dt;
    float d_term = pid->kd * derivative;
    pid->last_error = error;

    return p_term + i_term + d_term;
}
```

```bash
ceedling test:test_pid_controller
```

> **"GREEN! Complete PID implementation, driven by tests."**

### Part 2: REFACTOR Phase (5 minutes)

#### Improve the Design (3 minutes)

> **"Now we refactor to improve design while keeping tests green:"**

```c
// Add output limiting
#include "pid_controller.h"

void pid_init(pid_controller_t* pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->integral = 0.0f;
    pid->last_error = 0.0f;
    pid->output_min = -100.0f;  // Add limits
    pid->output_max = 100.0f;
}

void pid_set_output_limits(pid_controller_t* pid, float min, float max) {
    pid->output_min = min;
    pid->output_max = max;
}

static float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float pid_calculate(pid_controller_t* pid, float setpoint, float current, float dt) {
    float error = setpoint - current;

    // Calculate PID terms
    float p_term = pid->kp * error;

    pid->integral += error * dt;
    float i_term = pid->ki * pid->integral;

    float derivative = (error - pid->last_error) / dt;
    float d_term = pid->kd * derivative;
    pid->last_error = error;

    // Calculate output and apply limits
    float output = p_term + i_term + d_term;
    return clamp(output, pid->output_min, pid->output_max);
}
```

#### Verify Refactoring (2 minutes)

```bash
ceedling test:test_pid_controller
```

> **"Still GREEN! We improved the design without breaking existing functionality."**

> **"The tests gave us confidence to refactor. Without tests, we'd be afraid to touch working code."**

### Part 3: Discussion - When to Use TDD (5 minutes)

#### When TDD Works Well in Embedded

✅ **Algorithm Development**
- Mathematical functions (PID, filters, transformations)
- Data processing and parsing
- State machines and control logic

✅ **Business Logic**
- Application-level rules and decisions
- Communication protocols
- Data validation and conversion

✅ **Clear Requirements**
- Well-defined inputs and outputs
- Known edge cases and error conditions
- Measurable behavior

#### When TDD is Challenging

⚠️ **Hardware Initialization**
- Register setup and configuration
- Interrupt handler setup
- Clock and power management

⚠️ **Real-Time Critical Code**
- Interrupt service routines
- Time-critical loops
- Hardware-specific optimizations

⚠️ **Exploratory Development**
- Hardware bring-up
- Proof-of-concept code
- Research and experimentation

#### Hybrid Approach for Embedded

```
TDD for Logic + Traditional for Hardware = Best Results

Example Architecture:
┌─────────────────┐  ← TDD: Business logic, algorithms
│  Application    │
├─────────────────┤
│  Service Layer  │  ← TDD: Data processing, state machines
├─────────────────┤
│  Driver Layer   │  ← Traditional: Hardware interfaces
├─────────────────┤
│  HAL Layer      │  ← Traditional: Register manipulation
└─────────────────┘
```

#### Key Benefits We Demonstrated

> **"In 25 minutes of TDD, we got:"**
> 1. **Clear API design** - Tests drove interface decisions
> 2. **Complete functionality** - P, I, and D terms all working
> 3. **Regression protection** - Safe refactoring with test coverage
> 4. **Living documentation** - Tests show exactly how to use the code
> 5. **Confidence** - We know it works because we proved it step by step

#### Common Questions & Answers

**Q: "Doesn't TDD slow down development?"**
> **A: "Slower initially, but much faster overall due to less debugging and higher confidence."**

**Q: "What if requirements change?"**
> **A: "Change the tests first, then change the code. Tests help manage changing requirements."**

**Q: "How do you know what test to write first?"**
> **A: "Start with the simplest behavior that provides value. Build complexity incrementally."**

**Q: "What about performance requirements?"**
> **A: "First make it work (TDD), then make it fast (optimize with test protection)."**

## 🛠️ Instructor Preparation

### Pre-Demo Setup
- [ ] Ceedling working smoothly
- [ ] PID controller domain knowledge refreshed
- [ ] Code examples prepared as backup
- [ ] Timer ready for keeping pace

### Key Teaching Points

#### Emphasize These Concepts
- **Tests drive design decisions** - API emerges from test requirements
- **Small steps are powerful** - One failing test at a time
- **Refactoring is safe** with comprehensive test coverage
- **TDD != testing** - It's a design methodology

#### Common Misconceptions to Address
- ❌ "TDD means write all tests first" → ✅ "TDD means write ONE test, then implement"
- ❌ "TDD is about testing" → ✅ "TDD is about design and confidence"
- ❌ "Tests must be perfect" → ✅ "Tests evolve with understanding"

### Demo Tips

#### For Technical Flow
- **Keep tests simple** - Complex tests confuse the message
- **Celebrate each GREEN** - Build excitement for passing tests
- **Show refactoring confidence** - Run tests frequently
- **Admit when stuck** - "Let me think about the simplest test..."

#### For Engagement
- **Ask prediction questions** - "What should the test be?"
- **Share TDD struggles** - "I used to hate writing tests first too..."
- **Relate to real projects** - "How many of you debug PID controllers on hardware?"
- **Encourage interruption** - "Stop me if this doesn't make sense"

### Backup Plans

**If live coding fails:**
- Have completed code sections ready to paste
- Explain what you would have typed
- Continue with discussion of principles

**If tests fail unexpectedly:**
- Use it as teaching moment for debugging
- Show how tests help isolate problems
- Don't panic - real development has failures too

## 📊 Success Metrics

**Demo succeeds if attendees:**
- [ ] Understand Red-Green-Refactor cycle
- [ ] See how tests can drive API design
- [ ] Recognize when to apply TDD vs traditional approaches
- [ ] Feel motivated to try TDD on appropriate problems
- [ ] Ask questions about TDD in their specific context

**Follow-up Actions:**
- [ ] Share complete PID controller code
- [ ] Provide TDD resources and books
- [ ] Offer to review their first TDD attempts
- [ ] Schedule advanced TDD workshop if interest is high

---

**Key Message: "TDD isn't just about testing - it's about designing better software through the discipline of test-first thinking."**