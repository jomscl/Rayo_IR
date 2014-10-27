// depuración
#define HABILITA_DEBUG    // comentar para no debuggear

// Macros de impresión por serial
#ifdef HABILITA_DEBUG
  #define DEBUG(x)  Serial.print (x)
  #define DEBUGLN(x)  Serial.println (x)
#else
  #define DEBUG(x)
  #define DEBUGLN(x)
#endif

// defines

// leds
#define ledp 13
#define ledr 9 //pwm
#define ledg 10
#define ledb 11

byte led[]={ledr,ledg,ledb};
byte estadoled[]={0,0,0};

// entradas
#define oprayo 2 // OP2 en el shield
#define oppuerta 3  //OP1 en el shield

// salidas
#define relesirena 6 // relay 1 en el shield
#define relezona 7 // relay 2 en el shield

int timersirena=0;
#define segsirena 200
int timerzona=0;
#define segzona 30

// switch
#define interruptor 8

int timersalida=0;
#define segsalida 100;

#define mretardo 100

void setup() {
  // put your setup code here, to run once:
  // leds
  pinMode(ledp,OUTPUT);
  digitalWrite(ledp,HIGH);
  
  for (int i=0;i<=2;i++){
    pinMode(i,OUTPUT);
  }
  ledrgb();
  
  // entradas
  pinMode(oprayo,INPUT);
  pinMode(oppuerta,INPUT);

  // salidas
  pinMode(relesirena,OUTPUT);
  actuasirena(LOW);
  pinMode(relezona,OUTPUT);
  actuazona(LOW);
  
  // interruptor
  pinMode(interruptor,INPUT_PULLUP);
  
// habilitación del debug serial
#ifdef HABILITA_DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  // put your main code here, to run repeatedly: 
  if (leeinterruptor()){
    // modo apagado
    actuasirena(LOW);
    actuazona(LOW);
    grabaled(0,0,255);
    DEBUG(F("Llave off"));
  } // fin else interruptor off
  else
  {
    // modo encendido
    if (leepuerta()){
      // modo puerta abierta
      actuasirena(LOW);
      actuazona(LOW);
      grabaled(0,255,0);
      timersalida=segsalida;
      DEBUG(F("Puerta abierta"));
    }  // fin if puerta abierta
    else
    {
      if(timersalida>0){
        // modo tiempo de salida
        actuasirena(LOW);
        actuazona(LOW);
        if (timersalida%2 == 0)
        {
          grabaled(255,0,0);
        }
        else
        {
          grabaled(0,255,0);  
        }
        DEBUG(F("Timer salida"));
      }  // fin timer salida>0
      else{
        if(leerayo())
        {
          // Modo armado
          if(timerzona>0)
          {
            grabaled(255,0,255);
          }
          else
          {
            actuazona(LOW);
          }
          if(timersirena>0)
          {
            grabaled(255,0,255);
          }
          else
          {
            actuasirena(LOW);
            grabaled(255,0,0);
          }
          DEBUG(F("Modo Armado"));        
        }  // fin if rayo no activo
        else
        {
          // activar alarma
          actuasirena(HIGH);
          actuazona(HIGH);
          grabaled(255,0,255); 
          DEBUG(F("Alarma activa")); 
        } // fin else rayo activo
      }  // fin else timersalida>0
    } // fin else puerta cerrada
  } // fin else interruptor on

  // rutinas de decremento de tiempo
  delay(mretardo);
  if (timersalida>0){timersalida--;}
  if (timerzona>0){timerzona--;}
  if (timersirena>0){timersirena--;}
  DEBUG(F(" Salida="));DEBUG(timersalida);
  DEBUG(F(" Zona="));DEBUG(timerzona);
  DEBUG(F(" Sirena="));DEBUGLN(timersirena);
}

boolean leeinterruptor(){
  boolean estado=true;
  estado=digitalRead(interruptor);
  // descomentar para invertir el comportamiento del interruptor
  //estado=!estado;
  return estado;
}

boolean leepuerta(){
  boolean estado=false;
  estado=digitalRead(oppuerta);
  // descomentar para invertir el comportamiento del interruptor
  // estado=!estado; 
  return estado;
}

boolean leerayo(){
  boolean estado=false;
  estado=digitalRead(oprayo);
  // descomentar para invertir el comportamiento del interruptor
  // estado=!estado;  
  return estado;
}

void actuasirena(boolean estado){
  // descomentar para invertir el comportamiento del rele
  // estado=!estado;  
  timersirena=0;
  if (estado){timersirena=segsirena;}
  digitalWrite(relesirena,estado);
}

void actuazona(boolean estado){
  // descomentar para invertir el comportamiento del rele
  // estado=!estado;  
  timerzona=0;
  if (estado){timerzona=segzona;}
  digitalWrite(relezona,estado);
}

void grabaled(byte r, byte g, byte b){
  estadoled[0]=r;
  estadoled[1]=g;
  estadoled[2]=b;
  ledrgb();  
}

void ledrgb(){
  for (int i=0;i<=2;i++){
    analogWrite(led[i],estadoled[i]);
  }
} 
