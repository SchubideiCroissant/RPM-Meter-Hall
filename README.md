# Motor Umdrehungszähler mit Hall-Effekt-Sensor

## Projektbeschreibung

Dieses Projekt verwendet einen **Raspberry Pi Pico W**, um die Umdrehungen eines Motors mithilfe eines **Hall-Effekt-Sensors (KY-024)** zu messen. Der Sensor liefert analoge Spannungswerte, die vom ADC des Raspberry Pi Pico erfasst werden. Der Code zählt die Umdrehungen des Motors und berechnet die Umdrehungen pro Minute (RPM). Ein Schalter wird verwendet, um die Messung zu starten und zu beenden, während eine LED den Status der Messung anzeigt.

## Hardware-Komponenten

- **Raspberry Pi Pico W**
- **Hall-Effekt-Sensor (KY-024)**
- **Schalter**
- **LED**
- **Pull-Up-Widerstand** (intern oder extern)
- **Kondensator zur Entprellung des Schalters** (10 µF, optional)

## Schaltplan
```
---------------------------------------- VCC
        |                            | 
        |                            |
        ------------             -----------
        |    VC    |              |   VCC   |
        |        26|------------- | AO      |         
        |    [μC]  |              | [KY.024]|
------- |15      0 |------        |         |
|    |  |   GND    |     |        |    GND  |      
|    |  ------------     |        -----------
\    |       |           |             |
 \   |	     |           |             |
|  -----     |	         |	       |
|  [10yF]    |           |             |
|  -----     |           |             |
|    |       |         [LED]           |
|    |       |           |             |
|    |       |           |             |
|    |       |           |             |
--------------------------------------------- GND

```

### Legende:
- **[μC]**: Mikrocontroller (z. B. Raspberry Pi Pico)
- **26**: ADC-Pin für den Hall-Sensor
- **15**: Pin für den Schalter
- **0**: Pin für die LED
- **KY-024**: Hall-Effekt-Sensor
- **VCC**: Versorgungsspannung
- **GND**: Masse (Ground)
- **[10μF]**: Kondensator zur Entprellung des Schalters
- **[LED]**: Leuchtdiode

### Beschreibung:
- **VCC**: Versorgungsspannung wird an den Mikrocontroller und den Hall-Sensor angelegt.
- **GND**: Gemeinsame Masseverbindung.
- **Hall-Sensor**: Der Hall-Sensor ist an den ADC-Pin des Mikrocontrollers angeschlossen und gibt ein Signal entsprechend dem Magnetfeld aus.
- **Schalter**: Der Schalter ist an einen Pin des Mikrocontrollers angeschlossen und beeinflusst den Zustand der LED.
- **LED**: Die LED wird durch den Mikrocontroller gesteuert, um den Zustand der Messung anzuzeigen.
- **Kondensator**: Der Kondensator ist parallel zum Schalter geschaltet, um Störungen durch Prellen zu reduzieren.

Der **Hall-Effekt-Sensor KY-024** misst das Magnetfeld in seiner Umgebung und gibt eine analoge Spannung aus, die proportional zur Stärke des Magnetfelds ist. Die typischen Eigenschaften des Sensors sind:

- **Nordseite des Magneten:** Niedrigere Spannung (ca. 1.1V)
- **Südseite des Magneten:** Höhere Spannung (ca. 3.3V)
- **Idle Spannung:** Mittelspannung zwischen den beiden Magnetpositionen (ca. 2.5V)

Für präzise Messungen sollte die Spannungskurve des Sensors beobachtet und die Logik im Code entsprechend angepasst werden. Der Sensor sollte so positioniert werden, dass der Magnet sehr nah an ihm vorbei rotiert. Für die Messung der Umdrehungen pro Minute (RPM) sollte der Magnet an der Welle des Motors befestigt werden.

## Code-Beschreibung

Der bereitgestellte Code umfasst:

1. **Initialisierung der Hardware:**
   - Konfiguration des ADC für den Hall-Effekt-Sensor
   - Konfiguration der GPIO-Pins für den Schalter und die LED
   - Einrichtung eines Interrupts für den Schalter

2. **Interrupt-Service-Routine (ISR):**
   - Umschalten der LED (Lampe) an/aus
   - Starten der Zeitmessung beim Einschalten der Lampe
   - Berechnung der Zeitdauer und der Umdrehungen pro Minute beim Ausschalten der Lampe

3. **Kontinuierliche Messung der Hall-Sensor-Werte:**
   - Lesen der Werte vom ADC
   - Umrechnung der Werte in Spannungen und Zählen der Umdrehungen basierend auf den Spannungsschwankungen

4. **Ausgabe der Ergebnisse:**
   - Anzeige der Anzahl der Umdrehungen, der Messdauer in Millisekunden und Minuten sowie der Umdrehungen pro Minute

## Verwendung

1. **Schalter betätigen:** Um die Lampe ein- oder auszuschalten und die Zeitmessung zu starten oder zu beenden.
2. **Messwerte überprüfen:** Die Ergebnisse werden über die serielle Schnittstelle angezeigt und umfassen die Umdrehungen, die Messdauer und die Umdrehungen pro Minute, sobald die Lampe ausgeschaltet wird.
