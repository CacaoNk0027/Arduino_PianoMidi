/*
Proyecto Teclado "MIDI" con arduino y 74HC165

# Para mas informacion visita estas paginas:
https://www.arduino.cc - Pagina oficial de arduino
https://www.alldatasheet.es/datasheet-pdf/pdf/15549/PHILIPS/74HC165.html - Pines del 74HC165
https://es.wikipedia.org/wiki/MIDI
https://sonic-pi.mehackit.org/assets/img/es/midi_notes_es.png - Imagen de las notas en MIDI

# Materiales requeridos
Arduino (cualquier placa)
2 Chips 74HC165 - la cantidad de chips depende de las notas que quieras agregar
13 botones - en este caso es para completar una octava de Do a Do
Cables para las conexiones

# Fuentes

Para el desarrollo de este proyecto me base en los siguentes videos de youtube asi como en los codigos de los creadores
https://www.youtube.com/watch?v=dkFy9S-GjIo
https://www.youtube.com/watch?v=pIFoOQJEFZ0&t=1s
https://www.youtube.com/watch?v=OXIIHRF_14o&t=33s
*/

#define entradas 16 // el numero de entradas totales por chip (es necesario que se escriba el total de entradas de los chips de modo que 2*8 = 16 3*8 = 24)
#define anchoDePulso 5 // un delay para cargar los datos y el reloj del 74hc165
boolean notasOff[entradas];

int clockPin = 5; // reloj conectado al pin CP del IC (pin 2)
int cargarDatos = 6; // carga conectado al pin PL del IC (pin 1)
int datosPin = 4; // salida conectado al pin Q7 del IC (pin 9)

// este array de abajo se usa para especificar las notas que se tocaran al momento de pulsar los botones
byte notas[] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

void setup() {
  // inicializamos puerto serial
  Serial.begin(115200);
  // inicializamos los pines digitales
  pinMode(cargarDatos, OUTPUT); 
  pinMode(clockPin, OUTPUT);
  pinMode(datosPin, INPUT);
  // inicializamos en estado bajo el reloj y en la carga
  digitalWrite(clockPin, LOW);
  digitalWrite(cargarDatos, LOW);
  for (int i = 0; i < entradas; i++) {
    notasOff[i] = LOW;
  }
}

void loop() {
  // pulso para cargar el estado de las entradas
  digitalWrite(cargarDatos, LOW);
  delayMicroseconds(anchoDePulso);
  digitalWrite(cargarDatos, HIGH);

  boolean byt[entradas]; // arreglo donde se guardara el estado de las entradas
  
  // comenzamos bucle para leer el valor de cada entrada
  for (int i = 0; i < entradas; i++) {
    byt[entradas - 1 - i] = digitalRead(datosPin);
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(anchoDePulso);
    digitalWrite(clockPin, LOW);
  }
  // bucle para cargar los estados de las entradas al serial
  for (int i = 0; i < entradas; i++) {
    if (byt[i] == HIGH) { // nota presionada
      if(notasOff[i] == 0) { // esta apagado?
        midi(144, notas[i], 100);
        notasOff[i] = byt[i];
      }
    } else { 
      if(notasOff[i] == 1) { // esta encendido? 
        midi(128, notas[i], 0);
        notasOff[i] = byt[i];
      }
    }
  }
}

// funcion para enviar el midi
void midi(unsigned char command, unsigned char note, unsigned char vel) {
  Serial.write(command);
  Serial.write(note);
  Serial.write(vel);
}