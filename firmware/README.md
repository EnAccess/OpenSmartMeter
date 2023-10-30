## Get started / How to build it

### Pre-requisits

- For this README we assume you have an installed version of either of
  - [PlatformIO VSCode Plugin](https://platformio.org/platformio-ide) (Recommended for quick start)
  - [PlatformIO Core (CLI)](https://docs.platformio.org/en/latest/core/index.html) ([See below](#additional-step-for-platformio-core-cli-setup))
- Access to either of
  - USB to 3.3V TTL adapter
  - ST-LINK

### Build the image

**Platform.IO:**
Click the button that says `Build`

**CLI:**
Run

```sh
pio run
```

### Upload code

`upload` is a preconfigured target script by PIO.

### Uploading using a USB to 3.3V TTL adapter

Connect the USB to 3.3V TTL adapter to the STM32 Bluepill

| STM32 Bluepill | USB to 3.3V TTL adapter |
| --- | --- |
| `3.3V` | `3.3V` |
| `GND` | `GND` |
| `A9` | `RXD-->` |
| `A10` | `TXD<--` |

Set the device into programming mode, by adjusting the onboard jumper configuration

| Jumper pin | value |
| --- | --- |
| `BOOT0` | `1` |
| `BOOT1` | `0` |

Press the onboard `RESET` button to boot into programming mode.

> ⚠️ Pressing the onboard `RESET` button is mandatory to enter programming mode.
  If the device restarts after a succesful flash it won't enter programming mode again.
  Hence the onboard `RESET` button has to be pressed again to enter programming mode.

**Platform.IO:**
Click the button that says `Upload`.

**CLI:**
Run

```sh
pio run --target upload
```

### Uploading using ST-LINK

**TBD:** Write how to use ST-LINK

### Serial Monitor

PIO reads the baud rate from `platformio.ini` and hence does not require additional configuration.

### Serial Monitor using a USB to 3.3V TTL adapter

Connect the USB to 3.3V TTL adapter to the STM32 Bluepill

| STM32 Bluepill | USB to 3.3V TTL adapter |
| --- | --- |
| `3.3V` | `3.3V` |
| `GND` | `GND` |
| `A2` | `RXD-->` |
| `A3` | `TXD<--` |

**Platform.IO:**
Click the button that says `Serial Monitor`

**CLI:**
Run

```sh
pio device monitor
```

### Serial Monitor using ST-LINK

**TBD:** Write how to use ST-LINK

## Additional step for PlatformIO Core (CLI) setup

To enable auto-completion and IntelliSense [VSCode](https://code.visualstudio.com/) users can add the following to workspace `.vscode/c_cpp_properties.json` (adapt OS specifics like `compilerPath`)

```json
{
  "configurations": [
    {
      "name": "STM32",
      "includePath": [
        "${workspaceFolder}/**",
        "${HOME}/.platformio/packages/framework-arduinoststm32/**"
      ],
      "browse": {
        "path": [
          "${workspaceFolder}",
          "${HOME}/.platformio/packages/framework-arduinoststm32"
        ],
        "limitSymbolsToIncludedHeaders": false
      },
      "defines": [],
      "compilerPath": "/usr/bin/clang",
      "cStandard": "c11",
      "cppStandard": "c++17",
      "intelliSenseMode": "macos-clang-arm64"
    }
  ],
  "version": 4
}
```

Also add to workspace settings `.vscode/settings.json`

```json
{
  "C_Cpp.intelliSenseEngine": "Tag Parser",
}
```
