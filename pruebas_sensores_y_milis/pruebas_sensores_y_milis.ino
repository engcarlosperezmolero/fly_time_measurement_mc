template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

const int infrarrojo = 26; //G26 -> 0: lona, 1: aire
int estadoAnterior = 1; // 0: lona, 1: aire

unsigned long previousMillis = 0;  //will store last time LED was blinked
const long period = 10;

unsigned long tiempoAterrizaje = 0;
unsigned long tiempoDespegue = 0;
int long tiempoVuelo = 0;


void setup() {
  pinMode(infrarrojo, INPUT);
  Serial.begin(115200);
}

void loop() {
    // unsigned long currentMillis = millis(); // store the current time
  

    if (digitalRead(infrarrojo) == 0 && estadoAnterior == 1){
      tiempoAterrizaje = millis();
      tiempoVuelo = tiempoAterrizaje - tiempoDespegue;
      Serial << "aterrizo | tiempo vuelo: " << tiempoVuelo << "\n\n";
      estadoAnterior = 0;
    }
    if (digitalRead(infrarrojo) == 1 && estadoAnterior == 0){
        tiempoDespegue = millis();
        Serial.println("despego\n\n");
        estadoAnterior = 1;
    }
  
}
