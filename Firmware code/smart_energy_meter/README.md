## Get started / How to build it

**Pre-requisits**
- For this README we assume you have an installed version of either of
  - [PlatformIO VSCode Plugin](https://platformio.org/platformio-ide) (Recommended for quick start)
  - [PlatformIO Core (CLI)](https://docs.platformio.org/en/latest/core/index.html)

#### Build the image

**Platform.IO:**
Click the button that says `Build`

**CLI:**
Run
```sh
pio run
```

#### Upload code

`upload` is a preconfigured target script by PIO.

**Platform.IO:**
Click the button that says `Upload`.

**CLI:**
Run
```sh
pio run --target upload
```

#### Serial Monitor

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
