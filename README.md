# Labor im Fach Technische Informatik III (T3INF4260)

In diesem Labor wurde mit ESP32-C3 Mikrokontroller gearbeitet. Es wurden verschiedene Aufgaben bearbeitet, die in den jeweiligen Ordnern zu finden sind.

# Contributors

- [Felix Bilz](https://github.com/felixblz)
- [Dinar Karchevskii](https://github.com/karchevskii)

# Installation

1. Installieren Sie [Visual Studio Code](https://code.visualstudio.com/)
2. Installieren Sie [Espressif IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html)

# Wokwi
Wokwi ist ein Simulationstool für uC und ist sowohl im Browser, als auch als VSC Extension verfügbar. Die Klausuraufgabe kann ohne Hardware damit getestest werden (die Konfiguration ist bereits vorhanden). Dafür bildet man das Projekt und öffnet diagram.json im VSCode. Alterativ gibt es dieses Projekt auch [online](https://wokwi.com/projects/415695793737351169).

## Bitte beachten!
1. Im Labor wurde anderes LED-Strip verwendet, nämlich SK6812. Dieses hat andere Zeitkonstanten für SPI. Das im Wokwi verfügbares LED-Strip ist vom anderen Typ (WS2812), gleich wie der Onboard LED. Deswegen wirdim Wokwi die gleiche Funktion wie für Onboard LED auch für das Strip verwendet.
2. Um die Farben tatsächlich sehen zu können, muss der Poti hochgedreht werden, sonst ist die Helligkeit minimal.

# Besonderheiten

Im Termin 2 in der Aufgabe 4 in ISR darf man keine Print-Ausgaben machen, da es zu einem Fehler führt. Dies gilt allgemein für alle ISR. Die Print-Ausgaben sind außerhalb ISR aber erlaubt.
