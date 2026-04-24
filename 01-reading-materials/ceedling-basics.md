# ⚙️ Ceedling – Build System Basics

## What Is It?
Ceedling is a **build and test automation system** for C projects. It ties Unity, CMock, and GCC together into a single command-line workflow so you can build and run tests with zero manual wiring.

## The Stack

```
Ceedling  ←  orchestrates everything
  ├── Unity    ←  test assertions & runner
  ├── CMock    ←  auto-generates hardware mocks
  └── GCC/LLVM ←  compiles and links test binaries
```

## Project Structure
```
my_project/
├── project.yml       ← single config file
├── src/
│   ├── my_module.c
│   └── my_module.h
└── test/
    └── test_my_module.c
```

## Essential Commands

| Command | What It Does |
|---|---|
| `ceedling new <name>` | Scaffold a new project |
| `ceedling test:all` | Build & run all tests |
| `ceedling test:path[test_foo]` | Run a single test file |
| `ceedling gcov:all` | Generate code coverage report |
| `ceedling clean` | Remove all build artifacts |

## Key `project.yml` Sections
```yaml
:project:
  :build_root: build/

:paths:
  :source: [src/**]
  :test:   [test/**]
  :include: [src/**]

:plugins:
  :enabled: [gcov, stdout_pretty_tests_report]
```

## Key Takeaways
- ✅ Convention over configuration — follow the folder structure, it just works
- ✅ Automatic mock generation — add a header to `:mocks:` and CMock does the rest
- ✅ Built-in coverage via `gcov` plugin
- ✅ One config file (`project.yml`) controls everything
