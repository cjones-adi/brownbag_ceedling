# 🧪 Zero to Ceedling - Presentation Slides

**60-minute Brown Bag Session**

## Slide Design Notes
- **Keep slides minimal** - You're doing live coding, not lecturing
- **Large fonts** (24pt minimum) for code snippets
- **Dark theme recommended** for code visibility
- **Slides support the demo**, they don't replace it

---

## 📑 Slide-by-Slide Content

### Slide 1: Title Slide
**Visual:** Clean, professional title slide
```
🧪 Zero to Ceedling
Unit Testing for Embedded Systems

SDP Learning & Development Initiative
Embedded Core - "Zero to Firmware" Series

[Your Name]
[Date]
```

**Speaker Notes:**
- Welcome everyone warmly
- Mention this is part of larger learning initiative
- Set expectation: hands-on, practical session

---

### Slide 2: Session Overview
**Visual:** Simple agenda with time estimates
```
🎯 Today's Journey (60 minutes)

• Quick Concept Refresh (5 min)
• Real-World Scenarios (8 min)
• Live Demo - Build Together (35 min)
• Wrap-up & Next Steps (7 min)
• Q&A (5 min)

Goal: See unit testing in action for embedded systems
```

**Speaker Notes:**
- Emphasize "hands-on" approach
- "Don't worry if you didn't read the materials"
- "You'll leave with working code"

---

### Slide 3: Show of Hands
**Visual:** Interactive slide with hand emojis
```
🙋 Quick Poll

How many of you:
• Are familiar with unit testing?
• Currently write unit tests for firmware?
• Have said "My code works, I tested it on hardware"?

👆 That's totally normal!
```

**Speaker Notes:**
- Get audience interaction immediately
- Acknowledge their current reality
- "Embedded development has unique challenges"

---

### Slide 4: The Reality Check
**Visual:** Split screen - Current vs. Enhanced approach
```
🔍 Current Reality vs. Enhanced Approach

Current Testing:                Enhanced Testing:
"Tested on hardware"     +     "Tested logic + hardware"
✅ Catches integration          ✅ Same integration testing
❌ Limited edge cases           ✅ Comprehensive edge cases
❌ Hardware dependencies        ✅ Test without hardware
❌ Slow feedback                ✅ Instant feedback
```

**Speaker Notes:**
- "Hardware testing is essential - we're adding to it, not replacing it"
- "Today I'll show you the enhanced approach"

---

### Slide 5: 2-Minute Concept Refresh
**Visual:** Simple definitions with icons
```
🧩 Key Concepts (2-minute refresh)

🎯 Unit Test = Test one small piece of code in isolation
🎭 Mock = Fake hardware interface for testing
📋 AAA Pattern = Arrange, Act, Assert

That's all you need for the demo!
```

**Speaker Notes:**
- "For those who read the materials - this is review"
- "For those who didn't - this is all you need to know"
- Keep this under 2 minutes

---

### Slide 6: Real-World War Story 1
**Visual:** Code snippet with highlighted problem area
```
🔥 Case 1: The Boundary Bug

// Battery safety logic
if (temperature > 40 && voltage >= 4.2) {
    emergency_shutdown();
}

🤔 Question: What could go wrong here?
```

**Speaker Notes:**
- Ask audience to identify problems
- Wait for responses
- "Let's see what manual testing vs unit testing catches"

---

### Slide 7: The Boundary Reality
**Visual:** Testing comparison table
```
📊 Manual Testing vs Unit Testing

Manual Testing Covers:
• 35°C, 4.0V ✅ (No shutdown - correct)
• 45°C, 4.5V ✅ (Shutdown - correct)

Unit Testing Also Covers:
• Exactly 40°C? 39.9°C and 4.3V?
• Negative ADC readings? Sensor failures?
• 10,000 combinations in 1 second ⚡

```

**Speaker Notes:**
- "Manual testing hits the obvious cases"
- "Unit testing hits the edge cases where bugs hide"

---

### Slide 8: Real-World War Story 2
**Visual:** Timeline showing delays
```
🔥 Case 2: The Hardware Delay

Week 1: "PCB will arrive next week"
Week 3: "PCB has issues, new spin in 2 weeks"
Week 6: "Hardware works, but software isn't ready"
Week 8: "Everything breaks when integrated"

🙋 Raise your hand if this sounds familiar!
```

**Speaker Notes:**
- Get audience participation
- "This is where mocking shines"
- "Develop 95% of logic without hardware"

---

### Slide 9: Live Demo Introduction
**Visual:** Clean slide with demo overview
```
💻 Live Demo: ADC Temperature Reading

We'll Build Together:
• ADC reading function with validation
• Mock hardware interface
• Comprehensive unit tests
• See bugs caught instantly

Follow along or just watch - both are fine!
```

**Speaker Notes:**
- Transition to live coding
- "This is where the real learning happens"
- Check if people have Ceedling installed

---

### Slides 10-15: Demo Support Slides
**Note:** These are backup slides to support your live coding if needed

#### Slide 10: Demo - Function Signature
```c
// Function we're going to test
typedef enum {
    ADC_OK,
    ADC_OUT_OF_RANGE,
    ADC_HARDWARE_ERROR
} adc_result_t;

adc_result_t read_temperature_sensor(uint16_t* temp_celsius);
```

#### Slide 11: Demo - Test Structure Template
```c
void test_adc_should_return_correct_temperature_for_valid_reading(void)
{
    // ARRANGE: Set up test data

    // ACT: Call the function we're testing

    // ASSERT: Verify the result
}
```

#### Slide 12: Demo - Mock Setup
```c
// Mock the hardware function
uint16_t hal_adc_read_channel(uint8_t channel);

// In our test, we control what it returns
hal_adc_read_channel_ExpectAndReturn(ADC_TEMP_CHANNEL, 2048);
```

---

### Slide 16: Demo Results Summary
**Visual:** Success metrics from demo
```
🎉 What We Just Built

✅ Function with input validation
✅ 4 comprehensive unit tests
✅ Tests run in milliseconds
✅ Caught edge cases automatically
✅ No hardware required!

Total time: ~20 minutes
```

**Speaker Notes:**
- Celebrate what was accomplished
- "This same function took me days to debug on hardware"
- "Tests would have caught the bug in seconds"

---

### Slide 17: The Power Demonstration
**Visual:** Before/after comparison
```
🚀 What We Just Demonstrated

Before Unit Tests:
• Flash firmware → Test → Debug → Repeat
• Miss edge cases and error conditions
• Hardware dependencies block progress

With Unit Tests:
• Write test → Code → Instant feedback
• Comprehensive edge case coverage
• Develop independently of hardware
```

**Speaker Notes:**
- "You just saw the difference in action"
- "This scales to much more complex scenarios"

---

### Slide 18: Your Compiler vs Your Tests
**Visual:** Large, impactful quote
```
💡 Key Insight

"Your compiler says your code is
syntactically correct.

Your unit tests say your code is
behaviorally correct."

Both are essential for quality firmware.
```

**Speaker Notes:**
- Let this sink in for a moment
- "Syntax is just the beginning"
- "Behavior is what customers experience"

---

### Slide 19: Tools & Next Steps
**Visual:** Learning path with resources
```
🛠️ Tools & Resources

Tools You Saw Today:
• Ceedling (build system)
• Unity (test framework)
• CMock (hardware mocking)

Next Steps:
• Installation guide & materials shared
• Hands-on exercises available
• Office hours for support
```

**Speaker Notes:**
- "Everything is available for practice"
- "Start with Exercise 1: First Test"
- Mention support available

---

### Slide 20: The Journey Ahead
**Visual:** Learning progression diagram
```
📈 Your Unit Testing Journey

Today: Saw the basics in action
Week 1: Try your first unit test
Month 1: Test-driven development
Month 3: Full embedded testing mastery

Remember: Every expert was once a beginner!
```

**Speaker Notes:**
- Set realistic expectations
- "Learning is a process, not an event"
- "Start small, build confidence"

---

### Slide 21: Q&A / Contact
**Visual:** Contact information and resources
```
❓ Questions & Support

Questions now?
• Technical questions
• Tool setup help
• Application to your projects

Resources:
• [Your email/contact]
• Shared materials folder
• Team forum/chat

Thank you! 🙏
```

**Speaker Notes:**
- Open floor for questions
- Address specific concerns
- Thank everyone for their time and attention

---

## 📱 **Slide Design Tips**

### Visual Guidelines:
- **Minimal text** - Slides support your demo, don't replace it
- **Large fonts** - 24pt minimum, 32pt for code
- **High contrast** - Dark backgrounds for code visibility
- **Consistent branding** - Use your company colors/fonts

### Code Formatting:
- **Syntax highlighting** - Use appropriate color scheme
- **Large, readable fonts** - Consolas, Fira Code, or similar
- **Minimal code per slide** - Focus on key concepts
- **Line numbers** if helpful for reference

### Interactive Elements:
- **"Show of hands" moments** - Get audience participation
- **"What do you think?" pauses** - Let them predict outcomes
- **"Follow along" encouragement** - For those with laptops

### Backup Slides:
- **Have extra detail slides** ready if demo fails
- **Include installation troubleshooting** slides
- **Prepare for common questions** with dedicated slides

---

**To implement these slides:**
1. **Copy the content** into your preferred presentation tool
2. **Add company branding** and styling
3. **Test the live demo** beforehand
4. **Practice transitions** between slides and demo
5. **Have backup plans** for technical issues

**The goal: Slides should enhance your live demo, not compete with it!** 🎯
