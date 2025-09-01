# Traffic Lights Project

This repository contains an embedded application that implements a simple
traffic-light controller using red, amber, and green lights.

## Overview

- Runs on the Nordic Semiconductor nRF5340-DK (Arm Cortex-M33).  
  Can be ported to other platforms with minimal changes.  
- LEDs are controlled through a small abstraction layer, making the code
  platform-agnostic.  
- The application cycles through a standard traffic-light sequence:
  RED → RED+AMBER → GREEN → AMBER.  

## Design

The `TrafficLight` module is implemented as a composite of three `LED`
instances: one for red, one for amber, and one for green.  
This composition keeps the LED abstraction reusable across projects, while
allowing the `TrafficLight` class to focus on sequencing and state management.  

## Development

- Build and flash the project using the
  [VS Code nRF Connect Extension Pack](https://marketplace.visualstudio.com/items?itemName=nordic-semiconductor.nrf-connect-extension-pack).  
- The code is written in C and organized into reusable modules (`led` and
  `traffic_light`).  

## Documentation

- Documentation is generated with **Doxygen**.  
- Source files are fully annotated with Doxygen comments to make the API
  discoverable.  

## Goals

- Provide a clean, modular example of a traffic-light state machine.  
- Learn and apply embedded coding standards and documentation practices.  
- Explore automated testing and portability across embedded platforms.  
