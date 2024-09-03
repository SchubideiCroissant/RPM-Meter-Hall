#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <stdio.h>

#define HALL_SENSOR_PIN 26  // ADC-Pin für den Hall-Effekt-Sensor
#define SWITCH_PIN 15 
#define LED_PIN 0
bool lamp_state = false;    // Status der Lampe
absolute_time_t start_time; // Startzeit der Lampe
volatile uint64_t elapsed_ms;
volatile float rotations = 0; // Zählt jede Halbe Umdrehung
uint8_t current_measure = 0;

// Interrupt-Service-Routine (ISR) für den Schalter
void switch_isr(uint gpio, uint32_t event_mask) {
    // Schalte die Lampe um
    lamp_state = !lamp_state;
    gpio_put(LED_PIN, lamp_state);  // Setze den Lampen-Pin auf den neuen Status

    if (lamp_state) {
        // Lampe wird eingeschaltet, Zeit starten
        start_time = get_absolute_time();
        current_measure++;
        float rotations = 0;
        printf("Anfang des Messintervals %d\n", current_measure);
    } else {
        // Lampe wird ausgeschaltet, Zeit berechnen
        absolute_time_t end_time = get_absolute_time();
        elapsed_ms = absolute_time_diff_us(start_time, end_time) / 1000;
       // printf("############. Dauer: %llu ms\n", elapsed_ms);
    }
}

void print_output(){
    if(!lamp_state && (rotations/ (elapsed_ms/ 30000.0))!=0 && (elapsed_ms/ 30000.0)!=0 ){ // Ausgabe erfolgt nur wenn Zeitintervall und Rotationen gewmesen wurden 
            printf("Ende des Messintervals\n");
            printf("Anzahl der Umdrehungen: %.1f rad\n", rotations/2); // 
            printf("Messdauer ms: %llu ms\n", elapsed_ms);
            printf("Messdauer min: %.4f min\n", elapsed_ms/60000.0);
            printf("Umdrehungen pro Minute: %.1f rpm\n\n", (rotations/ (elapsed_ms/ 30000.0)));
            printf("____________________________________\n\n"); 
            rotations = 0; elapsed_ms = 0; // Messung erfolgt, Rücksetzen der Werte
    }
}

int main() {
    // Initialisierung der Standard-I/O (falls serielle Ausgabe gewünscht ist)
    stdio_init_all();

    // ADC initialisieren
    adc_init();
    // Switch initialisieren
    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);
    gpio_pull_up(SWITCH_PIN);  // Aktiviert den Pull-Up-Widerstand

    //LED initialisieren mit Pull Up-Widerstand
    gpio_init(LED_PIN); 
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);
    //bool lamp_state = false;    // Status der Lampe
    gpio_set_irq_enabled_with_callback(SWITCH_PIN, GPIO_IRQ_EDGE_FALL, true, &switch_isr); // Interrupt für Switch

    // GPIO26 als ADC-Eingang konfigurieren (Kanal 0)
    adc_gpio_init(HALL_SENSOR_PIN);
    adc_select_input(0);
  
    int j = 0; // Zählt, wie oft die Schleife wiederholt wird
    int a = 50000; // Output Rate
    bool b = false; // Nord-Süd Seiten Check
    // Endlosschleife für kontinuierliche Messung
    while (true) {
        
        // Lese den rohen ADC-Wert (12-Bit Auflösung, Bereich 0-4095)
        uint16_t raw_value = adc_read(); 
        float voltage = (raw_value / 4095.0) * 3.3; // Umrechnung des rohen Wertes in eine Spannung (0 bis 3.3V)  Idle Spannung: 2,50 V
        if (voltage < 2.4 && !b) { //Nordseite Magnet Spannung sinkt auf 1,1V
            rotations++;
            b = true;
        }
        if (voltage > 2.7  && b) { //Südseite Magnet: Spannung steigt auf 3,3V
            rotations++;
            b = false; 
        }
       // if(j%a == 0 )printf("ADC-Wert: %u, Spannung: %.2f V\n", raw_value, voltage); // Debug Output der den Output des Adcs schreibt
     
        j++;
        if(j%(a) == 0){
            print_output();
            
            j= 0; }
    }
}
