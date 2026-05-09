# Building and Uploading from WSL (Windows Subsystem for Linux)

This guide covers setting up PlatformIO in WSL2 to build and upload the Christmas Power Monitor firmware to an Arduino Mega 2560.

## Prerequisites

- Windows 10 (build 19041+) or Windows 11 with WSL2 installed
- Python 3 available in your WSL distribution (`python3 --version`)
- The Arduino Mega connected via USB

---

## 1. Install PlatformIO

Install PlatformIO Core (the CLI) inside WSL using the official installer:

```bash
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core-installer/develop/get-platformio.py -o /tmp/get-platformio.py
python3 /tmp/get-platformio.py
```

Add the PlatformIO binaries to your PATH by appending this to `~/.bashrc` (or `~/.zshrc`):

```bash
export PATH="$PATH:$HOME/.platformio/penv/bin"
```

Reload your shell:

```bash
source ~/.bashrc
```

Verify the installation:

```bash
pio --version
```

Install the AVR platform (required for Arduino Mega 2560):

```bash
pio pkg install --global --platform atmelavr
```

---

## 2. Connect the Arduino via USB (usbipd-win)

WSL2 does not have direct USB access. The **usbipd-win** tool bridges USB devices from Windows into WSL.

### Install usbipd-win on Windows

Open **PowerShell as Administrator** on Windows and run:

```powershell
winget install usbipd
```

### Install the usbip client tools in WSL

```bash
sudo apt-get install linux-tools-generic hwdata
sudo update-alternatives --install /usr/local/bin/usbip usbip /usr/lib/linux-tools/*-generic/usbip 20
```

> **Note:** You may see a warning like `usbipd not found for kernel 5.15.167.4-microsoft`. This is harmless — `usbipd` is the server daemon and is not needed inside WSL. The `usbip` client is all that's required to receive devices from Windows.

### Attach the Arduino to WSL

Each time you plug in the Arduino, run the following steps.

**In PowerShell (as Administrator):**

```powershell
# List connected USB devices and find your Arduino
usbipd list
```

The Arduino Mega 2560 will appear as `USB Serial Device (COMx)` with VID `2341`. Example output:

```
BUSID  VID:PID    DEVICE                        STATE
1-9    2341:0010  USB Serial Device (COM3)       Not shared
```

Note its `BUSID` (e.g., `1-9`).

```powershell
# Bind it (one-time setup per device)
usbipd bind --busid 1-9

# Attach it to WSL
usbipd attach --wsl --busid 1-9
```

**In WSL**, confirm the device is visible:

```bash
ls /dev/ttyACM* /dev/ttyUSB*
```

You should see something like `/dev/ttyACM0` or `/dev/ttyUSB0`.

### Grant permission to access the serial port

If you get a permission error during upload, add your user to the `dialout` group (one-time):

```bash
sudo usermod -aG dialout $USER
# Log out and back in, or run:
newgrp dialout
```

---

## 3. Build and Upload

Navigate to the project root and use the `pio` CLI.

### Compile only

```bash
cd ~/src/ChristmasPowerMonitor
pio run
```

### Compile and upload

```bash
pio run --target upload
```

If WSL detects the wrong port, specify it explicitly:

```bash
pio run --target upload --upload-port /dev/ttyACM0
```

### Open the serial monitor

```bash
pio device monitor
```

The monitor runs at 115200 baud as configured in `platformio.ini`. Press `Ctrl+C` to exit.

> **Important:** Run `pio device monitor` from the project root directory so it picks up the baud rate from `platformio.ini`. Running it from another directory will default to 9600 baud and produce garbled output. Alternatively, specify the baud rate explicitly:
> ```bash
> pio device monitor --baud 115200
> ```

---

## Troubleshooting

| Problem | Fix |
|---|---|
| `pio: command not found` | Make sure `~/.platformio/penv/bin` is in your PATH and you reloaded your shell. |
| No `/dev/ttyACM*` or `/dev/ttyUSB*` | Re-run `usbipd attach --wsl --busid <BUSID>` in an admin PowerShell window. |
| `Permission denied` on serial port | Run `sudo usermod -aG dialout $USER` then log out and back in. |
| Upload fails / wrong port | Pass `--upload-port /dev/ttyACM0` (adjust number as needed) to `pio run --target upload`. |
| Device detaches unexpectedly | Some USB hubs drop the connection. Try connecting directly to the PC. |
