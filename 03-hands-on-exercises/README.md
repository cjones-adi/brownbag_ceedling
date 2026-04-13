# 🔨 Hands-On Exercises (70% of Learning)

**This is where the real learning happens!** Self-exploration and hands-on experience with progressively challenging exercises designed to build your unit testing skills.

## 🎯 Learning Philosophy

> **"I hear and I forget. I see and I remember. I do and I understand."** - Confucius

These exercises follow a **learn-by-doing** approach where you'll:
- Start with simple, guided examples
- Progress to complex, real-world scenarios
- Make mistakes and learn from them
- Build confidence through repetition

## 📈 Exercise Progression

### 🌱 Beginner Level

#### 📝 Exercise 1: First Test (1-2 hours)
**File:** `exercise-01-first-test/`
- Write your very first unit test
- Understand test structure (Setup, Act, Assert)
- Learn basic Unity assertions
- **Goal:** Get comfortable with test syntax

#### 🧮 Exercise 2: Calculator TDD (2-3 hours)
**File:** `exercise-02-calculator-tdd/`
- Practice Test-Driven Development
- Implement basic arithmetic operations
- Learn the Red-Green-Refactor cycle
- **Goal:** Experience TDD workflow

### 🚀 Intermediate Level

#### 📡 Exercise 3: Sensor Driver (3-4 hours)
**File:** `exercise-03-sensor-driver/`
- Mock hardware interfaces (I2C, SPI)
- Test without physical hardware
- Handle communication errors
- **Goal:** Master hardware mocking

#### 🔄 Exercise 4: State Machine (4-5 hours)
**File:** `exercise-04-state-machine/`
- Test complex logic flows
- Validate state transitions
- Test boundary conditions
- **Goal:** Handle complex embedded logic

### 🎯 Advanced Level

#### 🔗 Exercise 5: Integration Testing (4-6 hours)
**File:** `exercise-05-integration/`
- Test multiple modules together
- Manage test complexity
- Balance unit vs integration tests
- **Goal:** Understand testing strategy

#### 🏆 Capstone Project (8-12 hours)
**File:** `capstone-project/`
- Complete embedded system testing
- Apply all learned techniques
- Reference the complex temp-sensor example
- **Goal:** Demonstrate mastery

## 🛠️ Exercise Structure

Each exercise includes:

```
exercise-XX-name/
├── README.md           # Exercise instructions and goals
├── REQUIREMENTS.md     # What you need to implement
├── starter-code/       # Starting point (if applicable)
├── solution/           # Reference solution (don't peek!)
├── tests/             # Test files to complete
└── docs/              # Supporting documentation
```

## 📋 Prerequisites

- ✅ Completed reading materials (01-reading-materials)
- ✅ Attended demo sessions (02-demo-sessions)
- ✅ Ceedling installed and working
- ✅ Basic C programming skills

## 🎮 How to Approach Each Exercise

### 1. 📖 Read First
- Understand the problem thoroughly
- Review requirements carefully
- Don't rush to start coding

### 2. 🎯 Plan Your Approach
- Break down the problem
- Identify test scenarios
- Think about edge cases

### 3. 🔴 Start with Red
- Write failing tests first (TDD)
- Make them as simple as possible
- One test at a time

### 4. 🟢 Make It Green
- Write minimal code to pass
- Don't over-engineer
- Focus on making tests pass

### 5. 🔧 Refactor
- Clean up code
- Improve design
- Keep tests green

### 6. 🔄 Repeat
- Add more test cases
- Handle edge cases
- Build confidence

## ⏱️ Time Estimates

| Exercise | Beginner | Intermediate | Advanced |
|----------|----------|--------------|----------|
| Exercise 1 | 2 hours | 1 hour | 30 min |
| Exercise 2 | 4 hours | 2 hours | 1 hour |
| Exercise 3 | 6 hours | 4 hours | 2 hours |
| Exercise 4 | 8 hours | 5 hours | 3 hours |
| Exercise 5 | 10 hours | 6 hours | 4 hours |
| Capstone | 16 hours | 12 hours | 8 hours |

## 🎓 Learning Objectives

By completing all exercises, you will:

✅ **Write effective unit tests** for embedded code
✅ **Mock hardware interfaces** to test without devices
✅ **Apply TDD** to embedded development
✅ **Test boundary conditions** and error scenarios
✅ **Balance unit and integration** testing strategies
✅ **Debug and maintain** test suites
✅ **Integrate testing** into your workflow

## 🆘 Getting Help

### When You're Stuck
1. **Review demo sessions** - Similar patterns
2. **Check reference examples** - Working implementations
3. **Read Ceedling docs** - Framework specifics
4. **Ask peers** - Collaborative learning
5. **Look at solutions** - Last resort!

### Common Pitfalls
- 🚫 Writing tests after implementation
- 🚫 Testing implementation details
- 🚫 Over-mocking simple functions
- 🚫 Ignoring boundary conditions
- 🚫 Making tests too complex

## 📊 Progress Tracking

Track your progress:
- [ ] Exercise 1: First Test
- [ ] Exercise 2: Calculator TDD
- [ ] Exercise 3: Sensor Driver
- [ ] Exercise 4: State Machine
- [ ] Exercise 5: Integration Testing
- [ ] Capstone Project

## 🏅 Success Criteria

You've mastered unit testing when you can:
- Write tests **before** writing implementation code
- Mock external dependencies **confidently**
- Test **error conditions** without breaking hardware
- **Refactor** code without breaking tests
- **Explain** the value of testing to colleagues

---

**Next Step:** Start with `exercise-01-first-test/README.md`