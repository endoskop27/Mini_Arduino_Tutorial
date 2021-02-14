// Über "#include" fügt man Bibliotheken rein. Es gibt für verschiedene Dinge bereits fertige Bibliotheken um bspw. Sensoren auszulesen. Genau das machen wir. Das "#include <Arduino.h> sagt dem Programm, dass es auf die Funktionen des Boards zugreifen kann (bspw. die Namen der Pins)"
#include <Arduino.h>

// Display (1602A) -----
// Hier binden wir die Bibliothek für das 1602A Display ein. 
#include <LiquidCrystal.h>

// Hier definieren wir die Pins des Displays passend zu dem wie wir das angeschlossen haben: https://create.arduino.cc/projecthub/najad/interfacing-lcd1602-with-arduino-764ec4
// "#define benutzt man um globale Konstanten zu setzen. Diese können nicht mehr überschrieben werden -> sind wirklich FIX im Speicher"
#define RS 12
#define E  11
#define D7 10
#define D6 9
#define D5 8
#define D4 7

// BMP 388 ----- 
// Hier binden wir die Bibliotheken für den Temperatursensor ein. Dadurch können wir auf bereits vorgebaute Funktionen zugreifen. 
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

// Dieser Define setzt den Luftdruck bei 0 Metern über dem Meeresspiegel. Das wird gebraucht um deine Höhe anhand des gemessenen Luftdrucks zu messen
#define SEALEVELPRESSURE_HPA (1013.25) // Wir nehmen 1013.25 als Durchschnitt an. Das ändert sich natürlich täglich, aber als Schätzung sollte das passen.

// An dieser Stelle benutzen die "Adafruit_BMP3XX" Bibliothek die wir vorher geladen haben und erstellen uns eine Instanz des Sensors. Nach der folgenden Zeile kannst du wenn du "bmp." eintippst und dann STRG+LEER drückst siehst du alle Funktionen, Parameter etc. der Instanz
Adafruit_BMP3XX bmp;

// So jetzt sollte auch das Display gehen nach dieser Zeile. Wir erstellen wieder eine Instanz des Sensors. Wir müssen die Pins mitgeben an denen das Display angeschlossen ist. Weiter oben haben wir das über define festgelegt, daher müssen wir hier nicht die nummern eintragen, sondern können direkt "RS, E, ..." reinschreiben. Sollten wir später die Pins ändern müssen, so können wir das nur oben machen und den rest müssen wir nicht mehr anfassen.
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);


// Hier legen wir fest, dass wir LED an Pin 5 und den Taster an Pin6 angebracht haben.
#define LED 5
#define TASTER 6
#define LDR 4


// Bei Arduino teilt sich die SW auf zwei Teile auf. Die setup() Funktion wird nur ein mal ausgeführt bei jedem Systemstart. Hier kannst du sachen reinpacken, die nur einmal ausgeführt werden müssen. Bspw. Sensorkommunikation starten oder Variablen definieren, etc. 
void setup() {
  
  // Die Serielle Schnittstelle ist das USB Kabel zwischen dem Arduino und dem PC. Darüber kannst du eine Konsole aufmachen und bspw. Ausgaben sehen die zum debuggen sehr hilfreich sind. Ebenso könntest du damit mit deinem Programm kommunizieren.
  Serial.begin(115200);

  // Hier sagen wir der Software, dass der Pin am Board an dem die LED hängt ein Ausgang ist
  pinMode(LED, OUTPUT); // LED ist der define von weiter oben, und OUTPUT sagt, dass es ein Ausgang ist.
  pinMode(TASTER, INPUT); // Hier sagen wir dass am TASTER Pin ein Eingang ist. Damit wir erkennen können ob jemand die Taste gedrückt hat.
  pinMode(LDR,INPUT);
  
  digitalWrite(LED, HIGH); // Hier testen wir ob die LED funktioniert. Damit setzen wir den Pin an dem die LED hängt digital auf "AN" und machen diese nach 2 sekunden wieder aus.
  delay(2000);
  digitalWrite(LED, LOW);



  // Hier starten wir den Temperatursensor. Du siehst wir rufen "bmp.begin_I2C()" auf. Dadurch macht das Programm schon alles um mit dem Sensor sprechen zu können. Sollte der Sensor antworten, würde die Funktion "WAHR" zurückliefen.  die Bedingung if() folgt der logik -> Wenn das was in der Klammer steht wahr ist (bspw. bier == lecker), dann geht das Programm in den Bereich zwischen den { } Klammern. Dir ist sicher das "!" aufgefallen. Das invertiert das ganze.
  // Sprich die Zeile unten heißt "Wenn Funktion begin_I2C nicht WAHR zurückliefert, dann gehe in die Klammer und Schreibe über die Serielle Schnittstelle den Text"
  // Temperatursensor (BMP388)
  if(!bmp.begin_I2C()) {
    Serial.println("BME388 nicht gefunden - Bitte Kabel prüfen!");
  }

  // Display starten
  lcd.begin(16,2);  // die 16 steht für 16 zeichen pro zeile und die 2 dafür, dass das display 2 zeilen hat.
  
  // damit wir sehen, ob alles geht, schreiben wir einfach mal was auf den Bildschirm
  lcd.print("Display geht!");
  // Damit sezten wir den Cursor. (0,1) (Position Horizontal, Position Vertikal)
  lcd.setCursor(0,1);
  lcd.print("Hurra!");

  // 2 Sekunden warten, damit du überhaupt was erkennen kannst. Programm rennt sehr schnell^^
  delay(2000);

  // Wir machen den Bildschirm mal sauber
  lcd.clear();

  // Jetzt müssen wir gucken, oder der Taster funktioniert. Daher lassen wir das Programm warten, bis jemand den Taster einmal drückt. Damit man das checkt, schreiben wir die Anweisung aufs Display
  lcd.setCursor(0,0);
  lcd.print("Taste drücken!");

  // digitalRead ist das gegenteil von digitalWrite. Der Ausdruck unten heißt: "Solange der Taster nicht HIGH ist, wiederhole immerwieder das zwischen den Klammern". In der Klammer zeigen wir den aktuellen Zustand des Tasters
  
  while(digitalRead(TASTER) != HIGH ) {
    lcd.setCursor(0,1);
    lcd.print(String(digitalRead(TASTER)));
    delay(100);

  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GEHT");

}

void loop() {
  lcd.clear();
  lcd.setCursor(0,0);
lcd.print(String(analogRead(TASTER)));
delay(500);

}