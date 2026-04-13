# 🧮 Exercise 2: Calculator TDD (2-3 hours)

**Learn Test-Driven Development by Building a Calculator**

## 🎯 Learning Objectives

By the end of this exercise, you will:
- ✅ Master the Red-Green-Refactor cycle
- ✅ Experience how tests drive API design
- ✅ Learn to write tests before implementation (always!)
- ✅ Understand TDD's impact on code quality and confidence
- ✅ Practice refactoring with safety of comprehensive tests

## 📋 Prerequisites

- Completed Exercise 1: First Test
- Understanding of basic unit testing concepts
- Familiar with Arrange-Act-Assert pattern
- Ready to think differently about development process

## 🎮 Exercise Overview

You'll build a **Scientific Calculator** using strict Test-Driven Development. Unlike Exercise 1, you will **NEVER** write implementation code without a failing test first.

**🔴 RED → 🟢 GREEN → 🔧 REFACTOR** - This is your new religion!

## 🏗️ What You'll Build

A calculator that supports:

### Core Operations
- Basic arithmetic (+, -, ×, ÷)
- Memory functions (store, recall, clear)
- Chain operations (5 + 3 × 2 = ?)
- Clear and reset functionality

### Advanced Operations (Bonus)
- Scientific functions (sqrt, power, sin, cos)
- Operation history
- Undo last operation
- Constants (π, e)

## 🎯 The TDD Rules

**YOU MUST FOLLOW THESE RULES STRICTLY:**

### Rule 1: No Production Code Without Tests
> You are not allowed to write any production code unless it is to make a failing unit test pass.

### Rule 2: Write Minimal Tests
> You are not allowed to write any more of a unit test than is sufficient to fail (compilation failures are failures).

### Rule 3: Write Minimal Implementation
> You are not allowed to write any more production code than is sufficient to pass the one failing unit test.

**Break these rules and start over!** 😤

## 🔄 The TDD Cycle (Micro-Steps)

```
🔴 RED (30 seconds - 2 minutes)
├── Write the smallest possible failing test
├── Watch it fail for the RIGHT reason
└── NO IMPLEMENTATION YET!

🟢 GREEN (30 seconds - 5 minutes)
├── Write the simplest code to pass the test
├── Ignore code quality for now
├── Copy-paste, hardcode, whatever works
└── SEE THE TEST PASS!

🔧 REFACTOR (1-5 minutes)
├── Clean up the code (implementation AND tests)
├── Remove duplication
├── Improve names and structure
└── KEEP ALL TESTS PASSING!

↩️ REPEAT
```

**Each cycle should take 2-10 minutes max!**

## 📚 Calculator Requirements (Revealed Through TDD)

You'll discover these requirements by writing tests first:

### State Management
```c
typedef struct {
    double current_value;
    double memory_value;
    char last_operation;
    double last_operand;
    bool error_state;
    // You'll discover what else you need through TDD!
} calculator_t;
```

### Core Interface (Will Evolve)
```c
// Basic lifecycle
void calculator_init(calculator_t* calc);
void calculator_clear(calculator_t* calc);

// Input operations
void calculator_input_number(calculator_t* calc, double number);
void calculator_input_operation(calculator_t* calc, char operation);
double calculator_get_result(calculator_t* calc);

// Memory operations
void calculator_memory_store(calculator_t* calc);
void calculator_memory_recall(calculator_t* calc);
void calculator_memory_clear(calculator_t* calc);

// You'll add more functions as TDD guides you!
```

## 🚀 TDD Journey Map

### Phase 1: Calculator Initialization (20 minutes)
**Tests you'll write:**
- Calculator should initialize to zero
- Calculator should clear to zero
- Calculator should handle initial state

### Phase 2: Basic Number Input (30 minutes)
**Tests you'll write:**
- Should accept single number input
- Should return input number as result
- Should handle decimal numbers
- Should handle negative numbers

### Phase 3: Simple Addition (45 minutes)
**Tests you'll write:**
- Should add two positive numbers
- Should add negative numbers
- Should add zero
- Should chain multiple additions

### Phase 4: All Basic Operations (60 minutes)
**Tests you'll write:**
- Subtraction scenarios
- Multiplication scenarios
- Division scenarios
- Division by zero handling

### Phase 5: Memory Functions (45 minutes)
**Tests you'll write:**
- Store value to memory
- Recall value from memory
- Clear memory
- Memory persists across operations

### Phase 6: Operation Chaining (60 minutes)
**Tests you'll write:**
- Chain multiple operations
- Handle operator precedence
- Complex expressions
- Error recovery

## 🎯 Success Criteria

### Technical Completion
- [ ] All tests pass
- [ ] Calculator handles all required operations
- [ ] Memory functions work correctly
- [ ] Error states handled gracefully
- [ ] Code is clean and well-structured

### TDD Process Mastery
- [ ] Never wrote implementation without failing test
- [ ] Can demonstrate Red-Green-Refactor cycle
- [ ] Tests drove API design decisions
- [ ] Refactored confidently with test protection
- [ ] Understands how TDD improves design

### Code Quality
- [ ] Tests are clear and focused
- [ ] Implementation is simple and readable
- [ ] No duplication in code or tests
- [ ] Good function and variable names
- [ ] Comprehensive edge case coverage

## 🎮 Exercise Approach

### Strict TDD Mode (Recommended)
- **Follow the rules religiously**
- **Take tiny steps** (1-2 lines of code at a time)
- **Refactor frequently** when tests are green
- **Let tests guide your design completely**

### Guided TDD Mode (For Beginners)
- **Use the provided test templates** as starting points
- **Follow the step-by-step instructions** closely
- **Focus on understanding the process** over speed
- **Ask for help** when stuck on TDD rules

### Advanced Challenge Mode
- **Add scientific calculator functions**
- **Implement operation history**
- **Add undo/redo functionality**
- **Build a simple calculator UI** (bonus)

## 🛠️ Getting Started

### Option 1: Start from Scratch (Pure TDD)
```bash
mkdir calculator-tdd-exercise
cd calculator-tdd-exercise
ceedling new calculator
cd calculator
```

### Option 2: Use Exercise Template
```bash
# Copy the starter template
cp -r exercise-02-calculator-tdd/starter-code my-calculator-tdd
cd my-calculator-tdd
```

## 📖 TDD Learning Resources

### During the Exercise
- **Step-by-step instructions** in `tdd-guide.md`
- **Red-Green-Refactor examples** for each phase
- **Common TDD mistakes** and how to avoid them
- **Refactoring patterns** for calculator code

### Reference Materials
- **Complete solution** with TDD history
- **Test naming patterns** for calculator operations
- **Design evolution** showing how tests drove architecture

## 🤔 Reflection Questions

As you work through the exercise, consider:

1. **How did writing tests first change your approach?**
2. **What design decisions did the tests force you to make?**
3. **When did you feel most confident in your code?**
4. **What would have been different without tests?**
5. **How did refactoring feel with comprehensive test coverage?**

## ⚠️ Common TDD Pitfalls

### Writing Too Much Test
```c
❌ Bad - tests multiple behaviors
void test_calculator_should_handle_all_operations(void)
{
    calculator_input_number(&calc, 5);
    calculator_input_operation(&calc, '+');
    calculator_input_number(&calc, 3);
    calculator_input_operation(&calc, '*');
    calculator_input_number(&calc, 2);
    // Too much at once!
}

✅ Good - tests one behavior
void test_calculator_should_accept_single_number_input(void)
{
    calculator_input_number(&calc, 5);
    TEST_ASSERT_EQUAL(5.0, calculator_get_result(&calc));
}
```

### Writing Too Much Implementation
```c
❌ Bad - implements more than needed
double calculator_get_result(calculator_t* calc)
{
    // Over-engineering before it's needed
    if (calc->operation_pending) {
        return calculate_pending_operation(calc);
    }
    return calc->current_value; // This is all we need now!
}

✅ Good - minimal implementation
double calculator_get_result(calculator_t* calc)
{
    return calc->current_value; // Just enough to pass
}
```

### Skipping the RED Phase
```c
❌ Bad - implementing without seeing failure
// Writing code that already passes

✅ Good - always see RED first
// 1. Write test that fails
// 2. Watch it fail for the right reason
// 3. Then implement
```

## 🏆 Advanced Challenges

After mastering basic TDD:

### Scientific Calculator
- Square root, power, trigonometric functions
- Logarithms and exponentials
- Complex number support

### Expression Parser
- Parse mathematical expressions from strings
- Handle operator precedence correctly
- Support parentheses grouping

### Calculator History
- Track operation history
- Undo/redo last operations
- Save/load calculator sessions

---

**Ready to experience the magic of Test-Driven Development? Let's build a calculator the TDD way!** 🚀