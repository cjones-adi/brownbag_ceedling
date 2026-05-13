# 🛠️ Ceedling Installation Guide

> **Latest version:** Ceedling **v1.0.1** (released January 2025)
> Ceedling is distributed as a Ruby gem. All installation paths below follow the same three-step pattern: install Ruby → install the gem → verify.

---

## Option A — Windows (Native)

### Prerequisites
1. Download and install **RubyInstaller for Windows** (includes DevKit):
   - Go to → https://rubyinstaller.org/downloads/
   - Choose the latest **Ruby+Devkit** `.exe` (e.g., `Ruby+Devkit 3.3.x (x64)`)
   - During setup, keep **"Add Ruby executables to your PATH"** checked
   - At the end of the installer, let it run `ridk install` → choose option **3** (MSYS2 and MINGW)

2. Download and install **GCC for Windows** via MSYS2 (comes with the DevKit above) — no extra steps needed.

### Install Ceedling
Open a new **Command Prompt** or **PowerShell** window (so PATH updates take effect):

```powershell
gem install ceedling
```

### Verify
```powershell
ceedling version
ruby --version
gcc --version
```

Expected output:
```
Ceedling:: 1.0.1-fb1ce6c
ruby 3.3.x ...
gcc (Rev...) ...
```

---

## Option B — Windows WSL (Windows Subsystem for Linux)

> **Recommended** for a Linux-native experience on Windows. Uses Ubuntu inside WSL.

### Step 1 — Enable WSL (if not already done)
Open **PowerShell as Administrator** and run:

```powershell
wsl --install
```

Restart when prompted. This installs WSL 2 with Ubuntu by default.

### Step 2 — Open Ubuntu terminal and update packages
```bash
sudo apt update && sudo apt upgrade -y
```

### Step 3 — Install Ruby and GCC
```bash
sudo apt install -y ruby-full gcc make
```

### Step 4 — Install Ceedling
```bash
gem install ceedling
```

> If you see a `PATH` warning about gem executables, add the following to your `~/.bashrc`:
> ```bash
> export PATH="$HOME/.local/share/gem/ruby/3.x.0/bin:$PATH"
> ```
> Then run `source ~/.bashrc`.

### Verify
```bash
ceedling version
ruby --version
gcc --version
```

---

## Option C — Docker

> **Best for CI/CD or zero-install environments.** No Ruby or GCC needed on the host machine.

### Prerequisites
- Install **Docker Desktop**: https://www.docker.com/products/docker-desktop/
- Ensure Docker is running (`docker info` should not error)

### Run Ceedling via the official image
ThrowTheSwitch publishes a ready-to-use image. Mount your project folder as a volume:

```bash
# Windows PowerShell
docker run --rm -v "${PWD}:/workspace" -w /workspace throwtheswitch/madsciencelab:latest ceedling version

# Windows CMD
docker run --rm -v "%cd%:/workspace" -w /workspace throwtheswitch/madsciencelab:latest ceedling version

# WSL / Linux / macOS
docker run --rm -v "$(pwd):/workspace" -w /workspace throwtheswitch/madsciencelab:latest ceedling version
```

### Run tests inside Docker
```bash
docker run --rm -v "$(pwd):/workspace" -w /workspace throwtheswitch/madsciencelab:latest ceedling test:all
```

> The `throwtheswitch/madsciencelab` image bundles Ruby, GCC, Ceedling, Unity, and CMock — everything preconfigured.

---

## Quick-Start: Create Your First Project

Once Ceedling is installed (any option above), scaffold a new project:

```bash
ceedling new my_project
cd my_project
ceedling test:all
```

You should see:

```
-------------------
TESTED:  0
PASSED:  0
FAILED:  0
IGNORED: 0
```

No errors = installation is working. ✅

---

## Troubleshooting

| Symptom | Fix |
|---|---|
| `ceedling: command not found` | Gem `bin` directory is not on PATH — see Step 4 WSL note above |
| `gcc: command not found` (Windows) | Re-run `ridk install` → option 3 |
| `Permission denied` installing gem | Use `sudo gem install ceedling` (Linux/WSL only) |
| Docker: `permission denied on /workspace` | Run Docker Desktop as administrator or fix volume permissions |
| Wrong Ceedling version installed | Run `gem install ceedling -v 1.0.1` to pin the version |

---

**Next Step:** Proceed to `project-setup.md` to configure your first Ceedling project.
