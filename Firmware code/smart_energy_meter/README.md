## Get started / How to build it

**Pre-requisits**
- For this README we assume you have an installed version of either of
  - [PlatformIO VSCode Plugin](https://platformio.org/platformio-ide) (Recommended for quick start)
  - [PlatformIO Core (CLI)](https://docs.platformio.org/en/latest/core/index.html) ([See below](#additional-step-for-platformio-core-cli-setup))

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

**Platform.IO:**
Click the button that says `Upload`.

**CLI:**
Run
```sh
pio run --target upload
```

### Serial Monitor

PIO reads the baud rate from `platformio.ini` and hence does not require additional configuration.

**Platform.IO:**
Make sure you have the ESP connected via USB.
Then, click the button that says `Serial Monitor`

**CLI:**
Make sure you have the ESP connected via USB.
Then, run

```sh
pio device monitor
```

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
