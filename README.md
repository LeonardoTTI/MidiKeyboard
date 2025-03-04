# MIDI Keyboard with AVR Mega2560  

This project was developed as part of the **Operating Systems Course (2022)**.  
It explores the use of **interrupts** and **music processing** in an embedded system.  

## 🛠️ Technical Details  

- **Microcontroller:** AVR Mega2560  
- **Programming:** Entirely written in **C**, without using the Arduino IDE or Arduino libraries.  
- **Compilation & Uploading:** Done via **avr-gcc**, bypassing the Arduino framework.  
- **PC Interface:** Implemented using a **library for terminal window updates**, rendering an ASCII-based user interface.  

## 🎹 Project Overview  

### 🔹 Embedded System (AVR Mega2560)  
The microcontroller handles a **custom MIDI keyboard**, capable of:  
- **Detecting key presses and measuring press duration** using a **two-way switch**.  
- **Generating MIDI-compliant messages** based on user input.  
- **Transmitting MIDI messages** via serial communication.  

### 🔹 PC Application  
The PC-side program is responsible for:  
- **Reading MIDI messages** received through the serial port.  
- **Generating corresponding sound output** based on the received MIDI data.  
- **Displaying an ASCII-based UI**, using terminal characters to represent interface elements.  

