<p align="center">
  <a href="https://github.com/EnAccess/OpenSmartMeter">
    <img
      src="https://enaccess.org/wp-content/uploads/2023/04/Open-Smart-Meter-GitHub-2240-%C3%97-800-transparent.svg"
      alt="OpenSmartMeter"
      width="640"
    >
  </a>
</p>
<p align="center">
    <em>The Open Smart Meter solves a problem that affects most solar developers in the industry. This inexpensive and open source Smart Meter meets IEC and key elements of the local regulations in Nigeria.</em>
</p>
<p align="center">
  <img
    alt="Project Status"
    src="https://img.shields.io/badge/Project%20Status-Functional%20prototype-yellow"
  >
  <img
    alt="GitHub Workflow Status"
    src="https://img.shields.io/github/actions/workflow/status/enaccess/OpenSmartMeter/opensmartmeter.yaml"
  >
  <a href="https://github.com/EnAccess/OpenSmartMeter/blob/main/LICENSE" target="_blank">
    <img
      alt="License"
      src="https://img.shields.io/github/license/enaccess/OpenSmartMeter"
    >
  </a>
</p>

---

## Description

This repository contains manufacturing details as related to smart energy meter, details such as hardware design, web software, firmware, exterior casing STL files, rationale behind design, how-to document to guide a skilled user to replicate every part of the smart meter available in the repository.

![OpenSmartMeter 3D Render](assets/hardware/render_3d/opensmartmeter_render_3d_high_quality.png)

## Getting Started

You have now an OpenSmartMeter in your hands and you want to test it? Here is how.

### Initial configuration
ToDo: Do a tutorial on how to access the technical mode via the keypad of the OpenSmartMeter. Make pictures of the OSM.

ToDo: How to set up secret key, starting code, etc.

### Testing OpenPAYGO tokens
After plugging the OSM, it will appear the following screen:
ToDo: Provide picture

By default, the OSM is deactivated, which means it doesn't provide any energy on its output.
If the initial default configuration has been applied, you can test tokens available on the Excel sheet "Token examples.xlsx" (Find it here: OpenSmartMeter\firmware\test).
If you want to create other token:
1) Open the repository on an IDE with python3.9 or above.
2) Install the python requirements (in OpenSmartMeter/firmware/test):
   pip3 install -r requirements.txt
3) Change initial parameters in "test_openpaygo.py"
Token_value can be days or kWh, depending on the initial default configuration you have defined on the OSM itself.
4) Run script and get your desired token in console output.

