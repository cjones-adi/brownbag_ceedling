# 🔄 Unit Testing in the SDLC

Unit testing isn't an "add-on" — it's a development practice woven into every phase of the Software Development Life Cycle. This document covers where it fits, how to apply it, and why it pays off.

---

## 🏗️ How Unit Testing Maps to Each SDLC Phase

| Phase | Unit Testing Role | Key Activities |
|-------|------------------|----------------|
| **Requirements** | Influence testable requirements | Write testable acceptance criteria; untestable requirements often signal design problems |
| **Design** | Drive modular architecture | Design HAL and interfaces with dependency injection; plan for mockability |
| **Implementation** | Guide and validate code | TDD cycle or test-as-you-go; tests are written alongside (or before) code |
| **Testing** | Foundation of the test pyramid | 70–80% of total test effort lives here |
| **Deployment** | Provide release confidence | Automated test execution gates every release |
| **Maintenance** | Enable safe refactoring | Tests act as regression protection for bug fixes and new features |

### Testable vs. Untestable Requirements Example
```
❌ "The system shall be fast"
✅ "Temperature readings shall be processed within 10ms of ADC completion"
```

---

## 📊 The Testing Pyramid

```
        /\
       /  \     E2E / System Tests   (5–10%)
      /____\    — Complete system behaviour
     /      \   Integration Tests    (15–25%)
    /________\  — Module interactions
   /          \ Unit Tests           (70–80%)
  /____________\ — Individual functions & modules
```

---

## �� Development Methodology Comparison

### Waterfall
```
Requirements → Design → Code → Test → Deploy → Maintain
                              ↑
                    Unit testing concentrated here
```
- Unit testing happens **after** coding is complete
- Late feedback makes bugs expensive to fix
- **Mitigation:** design testability in during the Architecture phase; write test plans during Design

### Agile
```
Sprint Planning → Daily (Code + Test) → Sprint Review → Retrospective → repeat
```
- Unit testing happens **continuously** during each sprint
- Fast feedback loops catch bugs early
- Include test-writing time in story point estimates; run tests in CI on every commit

### TDD (Test-Driven Development)
```
🔴 RED   → Write a failing test (define expected behaviour)
🟢 GREEN → Write minimal code  (make the test pass)
🔧 BLUE  → Refactor            (improve design, keep tests green)
↩️  REPEAT → Next small increment
```
- Tests are written **before** implementation — design emerges from the tests
- **Perfect for:** business logic, algorithms, state machines, data processing
- **Challenging for:** hardware bring-up, interrupt handlers → use traditional approach there

---

## ⏰ When to Write Unit Tests

| Timing | Approach | Best For | Maintenance Cost |
|--------|----------|----------|-----------------|
| **Before code (TDD)** | Red-Green-Refactor | Complex logic, algorithms | Low — tests guide design |
| **During code** | Write tests as you code | Most embedded development | Balanced |
| **After code** | Traditional testing | Hardware bring-up, prototypes | High — tests are retrofitted |

### Anti-Pattern to Avoid ❌
```
1. Write all code
2. Debug on hardware
3. Add tests only when bugs are found
→ Tests become regression nets, not design tools
```

---

## 🏭 Embedded-Specific Considerations

### Hardware-Software Co-Development
```
❌ Traditional:   SW team waits for HW → late integration → expensive debug
✅ With mocking:  SW (mocked HW) ‖ HW development → integrate → validate
```

### Separating Logic from Timing
```c
// Unit test — algorithm correctness, no timing pressure
void test_pid_calculates_correct_output(void) { /* test the math */ }

// Integration test — timing validated on real/simulated hardware
void test_pid_completes_within_1ms(void)      { /* test the deadline */ }
```

### Safety-Critical Standards
| Standard | Domain | Key Requirement |
|----------|--------|-----------------|
| ISO 26262 | Automotive | MC/DC structural coverage |
| IEC 61508 | Functional Safety | Systematic, documented testing |
| DO-178C | Aviation | Modified Condition/Decision Coverage |

---

## 🎯 Priority Matrix — What to Test First

| Code Type | Priority | Reason |
|-----------|----------|--------|
| Business Logic | 🔥 High | Complex algorithms, many edge cases |
| State Machines | 🔥 High | Critical transitions, error handling |
| Data Processing | 🔥 High | Calculations, transformations |
| Hardware Drivers | 🟡 Medium | Mock interfaces, validate error paths |
| Interrupt Handlers | 🟢 Low | Hard to isolate; focus on integration tests |
| Hardware Init | 🟢 Low | Hardware-dependent, minimal testable logic |

---

## 💰 ROI — The Cost of Finding Bugs Late

| Phase Bug Found In | Relative Fix Cost |
|--------------------|:-----------------:|
| Requirements | **$1** |
| Design | **$10** |
| Code | **$100** |
| Test Phase | **$1,000** |
| Production | **$10,000** |

### Development Impact
- **Initial time investment:** +20–30%
- **Long-term debugging time:** −40–90%
- **Feature velocity (after learning curve):** +25–40%
- **Defect density:** −40–80%
- **Customer-found bugs:** −60–90%

---

## 🚀 Team Adoption Roadmap

### Phase 1 — Foundation (Months 1–2)
- ✅ Install tooling (Ceedling, Unity, CMock)
- ✅ Train team on basics
- ✅ Create project templates
- ✅ Select a pilot project

### Phase 2 — Practice (Months 3–6)
- ✅ Apply TDD to new feature development
- ✅ Establish coding and testing standards
- ✅ Integrate tests into CI/CD pipeline
- ✅ Measure and track coverage + defect metrics

---

## 🔗 Integration with Other SDLC Activities

```
Commit → Build → Unit Tests → Static Analysis → Integration Tests → Deploy
              ↓          ↓             ↓                 ↓
           Fail?      Fail?         Fail?             Fail?
              └─────────────────────────────────────────┘
                    Stop pipeline, notify developer
```

- **Code reviews** should treat tests as first-class citizens — review tests as carefully as production code
- **Static analysis** catches syntax/style violations; unit tests catch logic errors — they complement each other

---

**Key Takeaway:** Unit testing improves design, reduces bugs, and accelerates long-term velocity. The earlier and more consistently it is integrated into your SDLC, the greater the return.
