# 🚀 Demo 1: Simple Start (30 minutes)

**Your First Unit Test — Pure Logic, No Hardware**

---

## 🎯 Demo Objectives

By the end of this demo, attendees will:
- ✅ Understand what a unit test is and why it matters in embedded development
- ✅ See **Unity** (Ceedling's built-in test framework) running live on the host
- ✅ Recognise the **Red → Green → Refactor** cycle in action
- ✅ Know what passing test output looks like
- ✅ Feel ready to write their first test

---

## 🧰 Ceedling Feature Highlighted: Unity

This demo focuses exclusively on **Unity** — the unit-test framework that ships
with Ceedling.  No mocks, no code generation, no plugins.

| Unity feature used | Purpose in this demo |
|---|---|
| `TEST_ASSERT_TRUE` / `FALSE` | Check LED on/off state |
| `TEST_ASSERT_FALSE_MESSAGE` | Emit a helpful message when looping over all channels |
| `TEST_ASSERT_GREATER_THAN` | Verify `get_max_leds()` returns a sensible value |
| `setUp()` | Re-initialise the module before every test for full isolation |
| `tearDown()` | Placeholder — nothing to clean up (no mocks, no hardware) |

CMock (Ceedling's mocking layer) is introduced in **Demo 2**.

---

## 📦 Reference Project

```
reference-examples/simple-led-control/
```

The module under test (`led_control`) is pure C — a `bool` state array with
index-bounds checking.  It has **no GPIO calls, no HAL, no board headers**.
It compiles and runs entirely on the host.

---

## ⏱️ Session Timeline

| Time | Activity |
|------|----------|
| 0–2 min | Opening hook |
| 2–5 min | What we are building & requirements |
| 5–10 min | Live project setup & `project.yml` walk-through |
| 10–15 min | Header walk-through, first test written |
| 15–16 min | Red phase — linker fails |
| 16–20 min | Green phase — minimal implementation |
| 20–22 min | Refactor phase — independence & boundary tests |
| 22–23 min | `ceedling test:all` — full green suite |
| 23–30 min | Q&A and next-demo teaser |

---

## 📋 Prerequisites

**Instructor:** Ceedling installed, `reference-examples/simple-led-control` clean and tested.  
**Attendees:** Basic C knowledge and curiosity about testing.

---

## 🔗 Files

| File | Purpose |
|---|---|
| `demo-script.md` | Step-by-step presenter script with live-coding cues |
| `reference-examples/simple-led-control/` | Complete working reference project |

---

**Next:** Demo 2 — Mocking Hardware (CMock replaces `gpio_hal` so tests run without a board).
