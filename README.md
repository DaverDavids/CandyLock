DISCLAIMER: This code was made with with the help of AI/LLMs.

# TwitchLock

ESP32-C3 sketch that monitors a Twitch channel's IRC chat and pulls **GPIO 3 HIGH for 5 seconds** when a qualifying message is received.

## Features
- Built-in web UI (Twitch-purple themed) with **live status & chat readout**
  - WiFi connection status + IP
  - Twitch IRC connection status
  - Solenoid trigger status
  - Live chat feed (last 30 messages); lines that trigger the unlock are highlighted
- Configurable via web UI (Twitch channel and trigger rules)
- **No manual unlock control** — the solenoid is only triggered by qualifying chat messages
- Trigger filters: broadcaster / mod / VIP / subscriber / any chatter / whitelisted users
- Optional command matching (e.g. `!lock`)
- Per-user blacklist
- ArduinoOTA firmware updates
- mDNS (`http://candylock.local`)
- WiFi captive portal when credentials are missing/wrong
- All settings persist across reboots and firmware flashes (NVS flash)
- Feature toggles at the top of the sketch

## Top-of-sketch Toggles
```cpp
#define ENABLE_OTA    0   // 0 = disable ArduinoOTA
#define ENABLE_WEBUI  1   // 0 = disable web UI
#define ENABLE_DEBUG  0   // 0 = disable all Serial output
```

## Setup
1. Copy `Secrets.h` (excluded from git), fill in your WiFi credentials and hostname.
2. Flash to your ESP32-C3.
3. Open `http://candylock.local` (or the IP shown on Serial) to configure the Twitch channel and trigger rules.

## Anonymous vs Authenticated
Leave **Bot username** and **oauth** blank to join Twitch IRC anonymously — this is enough to read chat.
For an authenticated bot (e.g. to verify subscriber status more reliably via tags), generate an oauth token at https://twitchapps.com/tmi/ and enter it in the web UI.

## Wiring
| ESP32-C3 | External |
|----------|----------|
| GPIO 3   | Relay/MOSFET IN |
| GND      | GND |

GPIO 3 goes HIGH for 5 seconds on each trigger event.
