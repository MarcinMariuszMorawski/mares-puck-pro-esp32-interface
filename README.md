### mares-puck-pro-esp32-interface

This repository contains the documentation, wiring diagram, and source code for a lightweight, hardware-free diagnostic interface for the **Mares Puck Pro+** dive computer using an **ESP32 NodeMCU** development board. 

This solution allows for seamless dive log synchronization with **Subsurface** without requiring any external discrete components (such as diodes, resistors, or logic level shifters). 

### 1. Transmission Specifications (Mares Puck Pro+)

* **Baudrate:** 115200 bps
* **Data bits:** 8
* **Parity:** Even (SERIAL_8E1)
* **Stop bits:** 1
* **Transmission Type:** Half-Duplex (Transmission and reception occur over a single shared DATA wire)

## 2. Wiring Diagram (Pinout)

The connection is established using only **two direct wires** between the ESP32 and the brass contacts located on the back of the dive computer case (next to the battery hatch).

```text
===================================================================
      ESP32 NodeMCU                               MARES PUCK PRO+
===================================================================
                                                (Back of the case)

                                              [ STRAP MOUNT ]
                                              _________________
   +-----------+                             /                 \

   |    GND    | -------------------------> /    (•) GND        \  <-- Top pin
   +-----------+                           |                       |

                                           |     [ BATTERY ]       |
                                           |    [  HATCH  ]        |
   +-----------+                           |                       |

   |    D4     | -------------------------> \    (•) DATA         /  <-- Bottom pin
   +-----------+                             \___________________/
                                              [ STRAP MOUNT ]
```

* **Top Mares Pin** (closer to the strap mount) ➡️ **GND** on the ESP32
* **Bottom Mares Pin** (closer to the circular battery cover) ➡️ **D4 (GPIO4)** on the ESP32

### 3. Microcontroller Firmware (ESP32)

The main challenge with single-wire half-duplex communication is the **loopback echo effect** (the ESP32 reads its own transmitted data). By utilizing Serial2.setPins(4, 4, -1, -1) and a precise logical delay, this sketch tracks the exact number of sent bytes and flushes them from the RX buffer. This prevents corrupted checksums inside the Subsurface software. 

### 4. Data Download Procedure (Subsurface)

To ensure a stable data transfer and prevent synchronization time-outs, follow these steps precisely: 

1. Flash the firmware above onto your ESP32 board using the Arduino IDE (e.g., select **ESP32 Dev Module**).
2. Connect the ESP32 to your PC via a USB cable and note the assigned port number (e.g., COM3).
3. Clean the brass contacts on the back of the watch (using isopropyl alcohol or a rubber eraser) to ensure solid electrical contact.
4. On the dive computer, navigate through the menu and enter **PC mode** (the word PC must remain solid on the screen).
5. Firmly hold your DIY interface wires against the Mares pins according to the pinout diagram.
6. Open **Subsurface** and open the download window (Ctrl + D).
7. Configure the following download parameters: 

  * **Vendor:** Mares
  * **Model:** Puck Pro +
  * **Connection/Port:** Select the specific COM port of your ESP32 (e.g., COM3).
8. Click **Download** while keeping a steady physical pressure on the contacts until the transfer progress bar finishes completely.
