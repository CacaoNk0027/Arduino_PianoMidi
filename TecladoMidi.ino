#define teclas 16
#define anchoDePulso 5
boolean notasOff[teclas];

int clockPin = 5;
int cargarDatos = 6;
int datosPin = 4;

byte notas[] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

void setup() {
  Serial.begin(115200);
  pinMode(cargarDatos, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(datosPin, INPUT);
  digitalWrite(clockPin, LOW);
  digitalWrite(cargarDatos, LOW);
  for (int i = 0; i < teclas; i++) {
    notasOff[i] = LOW;
  }
}

void loop() {
  digitalWrite(cargarDatos, LOW);
  delayMicroseconds(anchoDePulso);
  digitalWrite(cargarDatos, HIGH);
  boolean byt[teclas];
  for (int i = 0; i < teclas; i++) {
    byt[teclas - 1 - i] = digitalRead(datosPin);
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(anchoDePulso);
    digitalWrite(clockPin, LOW);
  }
  for (int i = 0; i < teclas; i++) {
    if (byt[i] == HIGH) {
      if(notasOff[i] == 0) {
        midi(144, notas[i], 100);
        notasOff[i] = byt[i];
      }
    } else {
      if(notasOff[i] == 1) {
        midi(128, notas[i], 0);
        notasOff[i] = byt[i];
      }
    }
  }
}

void midi(unsigned char command, unsigned char note, unsigned char vel) {
  Serial.write(command);
  Serial.write(note);
  Serial.write(vel);
}