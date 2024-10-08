Dieses kleine Badge habe ich 2021 für einen Lötkurs an meiner Hochschule entworfen. Damit sollten die Teilnehmer SMD löten üben. Am Ende konnten sie ihr Werk mit nach Hause nehmen und nach belieben programmieren.
Anlässlich des Girl's Days habe ich in einer späteren Iteration noch einen CH340 ergänzt, damit der Controller ohne extra Hardware programmiert werden kann, es wird nur noch ein Mini-USB Kabel benötigt.

FuldAIno vorn<br />
![FuldAIno_front](https://github.com/kyouma-mad-scientisto/loetkurs_2021/blob/main/FuldAIno_CH340_front.jpg)

FuldAIno hinten<br />
![FuldAIno_back](https://github.com/kyouma-mad-scientisto/loetkurs_2021/blob/main/FuldAIno_CH340_back.jpg)

FuldAIno im Betrieb<br />
![FuldAIno_running](https://github.com/kyouma-mad-scientisto/loetkurs_2021/blob/main/FuldAIno_inBetrieb.png)



IM WEITEREN FOLGT DIE ORIGINALE README.md
###########################################################################################################################################################################################################################


FuldAIno - eine 8x8 LED Matrix gesteuert von einem Atmega328p


Soll FuldAIno nach dem bestücken der Leiterplatte Arduino kompatibel sein, muss zunächst mit einem AVR ISP Flasher, über den 6 pol. ISP Anschluss, der Bootloader auf den Atmega328p gebrannt werden.
Den passenden Bootloader bzw. die passende Bibliothek für das Projekt findet ihr unter: https://github.com/oshlab/Breadboard-Arduino
Bitte zunächst den Anweisungen auf obiger Webseite folgen.
Pinbelegung auf der Platine (Draufsicht).


```
	1  2
       -------
MISO   | o o |  Vcc (5V)
SCK   [  o o |  MOSI
RESET  | o o |  GND
       -------
```



Danach wird ein einfacher USB-Serial Adapter benötigt. Diese findet man sehr günstig bei eBay, aliexpress und co. z.B. unter folgendem Namen: USB zu Serial TTL FT232RL
Hierbei sollte auch auf das Vorhandensein des DTR Pins geachtet werden, dieser wird benötigt, damit die Arduino IDE den Controller zum flashen zurücksetzen kann.
Außerdem empfehle ich gleich einen Adapter zu kaufen, welcher auch für 3,3V Logik geeignet ist, so sind auch zukünftige Projekte mit 3,3V Bauteilen, wie z.B. dem Arduino Pro Mini oder dem ESP32 kein Problem.
Pinbelegung auf der Platine (Draufsicht).


```
        ---
GND     |o|
Vcc(5V) |o|
Rx      |o|
Tx      |o|
DTR     |o|
        ---
```



Den seriellen Adapter entsprechend der obigen Belegung mit dem FuldAIno verbinden (Achtung! TX muss jeweils auf RX), nochmal alles kontrollieren, doppelt hält besser  ;).
Nun den Adapter mit dem PC verbinden, das FuldAIno Demoprojekt öffnen, den korrekten Port und das korrekte Board (Breadboard Arduino (OSHLAB.com) -> Atmega328p (8MHz internal)) auswählen und dann, oben links,
auf den blauen Pfeil klicken (hochladen) und warten, bis der Vorgang abgeschlossen ist.


Hinweise zum Demoprojekt:

Für die regelmäßge Aktualisierung der Anzeige wird der Timer1 des Atmegas beansprucht. Dieser kann also nicht für andere Dinge verwendet werden. Ihr könnt aber die ISR, welche in "ScanLines.ino" ab Zeile 13
beginnt für KURZE Funktionen nutzen, welche regelmäßig ausgeführt werden sollen.
Die Funktion "scanLines()" muss IMMER in der Timer1 ISR ausgeführt werden, sonst findet keine Aktualisierung der Anzeige mehr statt.
Die Datei "ScanLines.ino" am besten garnicht verändern, außer ihr möchtet die LEDs direkt ansteuern, ohne den Framebuffer.
