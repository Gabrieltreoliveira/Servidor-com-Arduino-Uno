// leitura de sensor

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

EthernetServer server(80);

int pinNTC = A0;
float R1 = 10000; // valor da resistência na temperatura de referência
float logR2, R2, T;
float A = 0.001129148, B = 0.000234125, C = 0.0000000876741;

void setup()
{
  pinMode(pinNTC, INPUT);
  Serial.begin(9600);
  Ethernet.begin(mac);
  server.begin();
  Serial.print("Server started on: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    logR2 = log(R1 / (1023.0 / analogRead(pinNTC) - 1.0));
    R2 = R1 * exp(A * logR2 * logR2 + B * logR2 + C);
    T = 1.0 / (A + B * logR2 + C * logR2 * logR2 * logR2);
    T = T - 273.15; // converter de Kelvin para Celsius
    client.println("Temperatura: " + String(T, 2) + " °C");
    delay(1000);
    client.stop();
  }
}
