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
    src="https://img.shields.io/github/actions/workflow/status/EnAccess/OpenSmartMeter/check.yaml"
  >
  <a href="https://github.com/EnAccess/OpenSmartMeter/blob/main/LICENSE" target="_blank">
    <img
      alt="License"
      src="https://img.shields.io/github/license/EnAccess/OpenSmartMeter"
    >
  </a>
</p>

---

## Description

This repository contains manufacturing details as related to smart energy meter, details such as hardware design, web software, firmware, exterior casing STL files, rationale behing design, how-to document to guide a skilled user to replicate every part of the smart meter is available in the repository.

![OpenSmartMeter 3D Render](docs/assets/hardware/render_3d/opensmartmeter_render_3d_high_quality.png)

## Technical developmental breakdown

The open-source smart meter development has four main parts to it as highlighted below:

### Circuit and Enclosure Design

The circuit was designed on Kicad free and open-source PCB design software while the enclosure design was done using Fusion 360 software.

### PCB and Enclosure fabrication

PCB fabrication involves putting together the physical components that makes up the smart meter.
The PCB features a 2layer design incorporating CICADA GSM module for communication while the enclosure offers a minimalistic IP 51 design according to IEC metering enclosure design standard.

### Firmware, Web interface Development & API integration

The firmware code was written in Arduino IDE using C++, while the web front end was written majorly in HTML and CSS while the back end was written majorly in PHP.
Links to all these codes are here.
ThingsBoard API was used to view meter parameters such as energy, credit.

### Calibration & Testing

After putting the hardware and software together, we needed to validate the performance of the smart meter and ascertain its accuracy.
Due to varying tolerances of used components, it is vital to perform a calibration exercise.
This is used to correct any errors due to varying tolerances.
Further details including source code, schematics, and manufacturing files can be found in the EnAccess github repository

### Infrastructure diagram

<img
  width="707"
  alt="Screenshot 2022-11-05 at 4 14 06 AM"
  src="https://user-images.githubusercontent.com/99180312/200098555-717123fc-a56d-44c0-b2da-77d279fcb1c4.png">
