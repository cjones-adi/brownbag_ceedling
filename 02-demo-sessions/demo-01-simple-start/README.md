# 🚀 Demo 1: Simple Start (30 minutes)

**Your First Unit Test Journey - From Zero to Green**

## 🎯 Demo Objectives

By the end of this demo, attendees will:
- ✅ See a complete Ceedling project setup from scratch
- ✅ Witness the first unit test being written and executed
- ✅ Understand the Red-Green-Refactor cycle
- ✅ Recognize test output and know what success looks like
- ✅ Feel confident to try unit testing on their own

## 📋 Prerequisites

**For Instructor:**
- Ceedling installed and working
- Demo environment prepared (clean directory)
- LED control example code ready

**For Attendees:**
- Read basic unit testing concepts
- Basic C programming knowledge
- Curiosity about testing! 😊

## ⏱️ Demo Timeline

| Time | Activity | Duration |
|------|----------|----------|
| 0-5 min | **Problem Setup** - Introduce the scenario | 5 min |
| 5-15 min | **Live Setup** - Create project from scratch | 10 min |
| 15-20 min | **First Test** - Write and run first test | 5 min |
| 20-25 min | **Red-Green-Refactor** - See the cycle in action | 5 min |
| 25-30 min | **Discussion** - Q&A and next steps | 5 min |

## 🎬 Demo Script

### Opening Hook (1 minute)

> **"Show of hands: How many of you have ever said 'My code works, I tested it on the hardware'?"**
>
> **"Today I'm going to show you how to test your embedded code before it ever touches a microcontroller. In 30 minutes, you'll see how unit testing can catch bugs that hardware testing might miss."**

### Part 1: Problem Setup (5 minutes)

#### The Scenario
> **"We're building LED control firmware. Simple, right? Just turn LEDs on and off. But even simple code can have bugs.**
>
> **Let's say we have this requirement:**

```c
// LED Control Requirements:
// 1. Turn LED on/off by number (0-7)
// 2. Query LED state (on/off)
// 3. Handle invalid LED numbers gracefully
// 4. Initialize all LEDs to OFF state
```

> **"Normally, you'd write this code, flash it to a board, and test with LEDs. But what if you don't have the board yet? What if you have 8 LEDs but want to test all 256 possible combinations? What if you want to test error conditions?"**

#### Why This Matters
> **"This simple LED controller demonstrates the fundamental testing concepts you'll use for complex embedded systems. The patterns we learn here scale to sensor drivers, communication protocols, and safety-critical logic."**

### Part 2: Live Setup (10 minutes)

#### Step 1: Create Project Directory (2 minutes)
```bash
# [INSTRUCTOR TYPES LIVE]
mkdir demo_led_control
cd demo_led_control

# Show empty directory
ls -la
```

> **"Starting with a completely empty directory - no magic setup files."**

#### Step 2: Initialize Ceedling Project (3 minutes)
```bash
# [INSTRUCTOR TYPES LIVE]
ceedling new demo_led_control
cd demo_led_control
ls -la
```

> **"Ceedling creates a complete project structure. Let's look at what we got:"**

```bash
# [INSTRUCTOR SHOWS]
tree
```

**Explain the key directories:**
- `src/` - Our production code goes here
- `test/` - Our unit tests go here
- `project.yml` - Ceedling configuration
- `build/` - Generated files (created when we run tests)

#### Step 3: Examine project.yml (2 minutes)
```bash
# [INSTRUCTOR OPENS]
cat project.yml
```

> **"This configuration tells Ceedling:**
> - **Use mocks** - We can simulate hardware
> - **Use Unity** - The test framework
> - **Test file prefix** - test_ files are our tests"**

#### Step 4: Verify Setup Works (3 minutes)
```bash
# [INSTRUCTOR TYPES]
ceedling test:all
```

> **"Should show 'No tests to run' - perfect! Ceedling is working."**

### Part 3: First Test (5 minutes)

#### Step 1: Create Header File (2 minutes)
```bash
# [INSTRUCTOR CREATES]
vim src/led_control.h
```

```c
#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

// Initialize LED controller
void led_control_init(void);

// Turn LED on/off
void led_control_turn_on(uint8_t led_num);
void led_control_turn_off(uint8_t led_num);

// Query LED state
bool led_control_is_on(uint8_t led_num);

// Get number of LEDs supported
uint8_t led_control_get_max_leds(void);

#endif
```

> **"Clean interface - no hardware dependencies visible. This is key for testability!"**

#### Step 2: Create First Test (3 minutes)
```bash
# [INSTRUCTOR CREATES]
vim test/test_led_control.c
```

```c
#include "unity.h"
#include "led_control.h"

void setUp(void)
{
    led_control_init();
}

void tearDown(void)
{
    // Cleanup after each test
}

void test_led_control_should_initialize_all_leds_to_off_state(void)
{
    // Arrange - setUp() already called led_control_init()

    // Act & Assert - Check that all LEDs start in OFF state
    for (uint8_t led = 0; led < led_control_get_max_leds(); led++) {
        TEST_ASSERT_FALSE_MESSAGE(led_control_is_on(led),
                                  "LED should be OFF after initialization");
    }
}
```

> **"Notice the test structure:**
> - **setUp()** - Runs before each test
> - **Test name** - Describes exactly what we're testing
> - **Arrange-Act-Assert** - Clear test structure"**

### Part 4: Red-Green-Refactor Cycle (5 minutes)

#### Red Phase - See Test Fail (1 minute)
```bash
# [INSTRUCTOR RUNS]
ceedling test:test_led_control
```

> **"ERROR! Functions don't exist yet. This is the RED phase - test fails because we haven't implemented the code yet. This proves our test is actually testing something!"**

#### Green Phase - Make Test Pass (3 minutes)
```bash
# [INSTRUCTOR CREATES]
vim src/led_control.c
```

```c
#include "led_control.h"

#define MAX_LEDS 8
static bool led_states[MAX_LEDS];

void led_control_init(void)
{
    for (uint8_t i = 0; i < MAX_LEDS; i++) {
        led_states[i] = false;
    }
}

void led_control_turn_on(uint8_t led_num)
{
    if (led_num < MAX_LEDS) {
        led_states[led_num] = true;
    }
}

void led_control_turn_off(uint8_t led_num)
{
    if (led_num < MAX_LEDS) {
        led_states[led_num] = false;
    }
}

bool led_control_is_on(uint8_t led_num)
{
    if (led_num < MAX_LEDS) {
        return led_states[led_num];
    }
    return false;
}

uint8_t led_control_get_max_leds(void)
{
    return MAX_LEDS;
}
```

> **"Minimal implementation to make the test pass. Notice:**
> - **No hardware access** - Just state tracking
> - **Boundary checking** - Handle invalid LED numbers
> - **Simple logic** - Easiest thing that could work"**

#### Test Passes! (1 minute)
```bash
# [INSTRUCTOR RUNS]
ceedling test:test_led_control
```

> **"GREEN! The test passes. We now have:**
> - **Working code** that meets the requirement
> - **Automated test** that proves it works
> - **Confidence** to refactor or add features"**

### Part 5: Add Another Test Live (Bonus if time allows)

```c
void test_led_control_turn_on_should_set_led_state_to_on(void)
{
    // Arrange
    uint8_t led_num = 3;

    // Act
    led_control_turn_on(led_num);

    // Assert
    TEST_ASSERT_TRUE(led_control_is_on(led_num));
}
```

> **"Let's add another test and see it pass immediately - because our implementation already handles this case!"**

### Part 6: Discussion & Q&A (5 minutes)

#### Key Takeaways
> **"What we just saw:**
> 1. **Set up a complete testing environment in 10 minutes**
> 2. **Wrote tests that run instantly** - no hardware needed
> 3. **Used Red-Green-Refactor cycle** - test-driven development
> 4. **Created confidence** - we know our code works"**

#### Common Questions & Answers

**Q: "But this doesn't test the actual GPIO pins!"**
> **A: "Correct! Unit tests test logic, not hardware. We'll mock the GPIO in Demo 2. Integration tests verify hardware."**

**Q: "Isn't this slower than just testing on hardware?"**
> **A: "Initially yes, but unit tests run in milliseconds vs minutes for flashing. And they test edge cases you might miss manually."**

**Q: "What about real-time constraints?"**
> **A: "Unit tests validate algorithms work correctly. Performance testing is separate (and also benefits from this foundation)."**

#### Next Steps Teaser
> **"In the next demos, you'll see:**
> - **Demo 2: Mocking Hardware** - How to test I2C drivers without I2C chips
> - **Demo 3: Test-Driven Development** - Write tests first for better design
> - **Demo 4: Boundary Testing** - Catch the bugs that hardware testing misses"**

## 🛠️ Instructor Preparation

### Pre-Demo Setup
- [ ] Clean demo environment
- [ ] Ceedling working and tested
- [ ] Terminal ready with large font
- [ ] Code editor configured
- [ ] Backup plan if live coding fails

### Materials Needed
- [ ] Laptop with Ceedling installed
- [ ] Projector/screen for live coding
- [ ] Handout with key commands
- [ ] Contact info for follow-up questions

### Common Issues & Solutions

**Ceedling not found**
- Solution: Verify PATH includes gem binaries
- Backup: Use pre-installed VM or container

**Compilation errors**
- Solution: Double-check syntax carefully
- Backup: Have working files ready to copy

**Test failures**
- Solution: Walk through test logic step-by-step
- Backup: Use this as teaching moment for debugging

### Demo Tips

#### For Engagement
- **Ask questions** throughout - "What do you think will happen?"
- **Encourage prediction** - "Will this test pass or fail?"
- **Share mistakes** - "I made this error yesterday..."
- **Use analogies** - "Testing is like having a robot QA engineer"

#### For Technical Accuracy
- **Keep it simple** - Don't dive into advanced concepts
- **Stick to basics** - Focus on the fundamentals
- **Admit unknowns** - "Great question for the advanced session"
- **Time management** - Use timer to keep on track

## 📊 Success Metrics

**Demo is successful if attendees:**
- [ ] Can explain what a unit test is
- [ ] Understand Red-Green-Refactor cycle
- [ ] See the value of testing without hardware
- [ ] Want to try it themselves
- [ ] Ask questions about more advanced topics

**Follow-up Actions:**
- [ ] Send demo code to attendees
- [ ] Share exercise instructions
- [ ] Schedule office hours for individual help
- [ ] Collect feedback for improvement

---

**Key Message: "In 30 minutes, you've seen how unit testing can transform embedded development - faster feedback, better design, and more confidence in your code."**