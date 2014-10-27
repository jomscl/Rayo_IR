/* DAtos del Shield 4in 4out de Olimex
// Optoacopladores en las entradas digitales D4, D5, D6 y D7
int Opto1 = 7;     
int Opto2 = 6;
int Opto3 = 5;
int Opto4 = 4;

// Las entradas análogas se comportan como salidas digitales A0, A1, A2 y A3
int Relay1 =  A0;   
int Relay2 =  A1;
int Relay3 =  A2;
int Relay4 =  A3;
*/

#include <Metro.h>

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
  #define rayo 4  // op4
  #define puerta 5  // op3
  #define porton 6  // op2
  #define receptor 7 // op1
  
  // salidas
  #define alarma A0 // relay 1
  #define swPorton A1 // relay 2
  
  // switch
  #define interruptor 8

  // constantes
  #define segAlarma 20
  #define segPorton 10
  #define segSalida 100
  
// variables
Metro pulso = Metro(100); 

unsigned timerAlarma=0;
unsigned timerPorton=0;
unsigned timerSalida=0;

void setup() {
  // habilitación del debug serial
  #ifdef HABILITA_DEBUG
    Serial.begin(9600);
  #endif
  
  // leds
  pinMode(ledp,OUTPUT);
  for (int i=0;i<2;i++){pinMode(led[i],OUTPUT);}
  
  // entradas
  pinMode(rayo,INPUT);
  pinMode(puerta,INPUT);
  pinMode(porton,INPUT);
  pinMode(receptor,INPUT);
  
  // salidas
  pinMode(alarma,OUTPUT);
  pinMode(swPorton,OUTPUT);
  actuaAlarma(LOW);
  actuaPorton(LOW);
  
  // interruptor
  pinMode(interruptor,INPUT_PULLUP);
  
}

void loop() {
  // rutinas con timer
  if (pulso.check()){
    // alternar el led de poder
    digitalWrite(ledp,!digitalRead(ledp));  
 
     // decrementar los timers
    reduceVariables();
    
    // revisar el rayo
    if (leeSwitch()){
      // swirch armado, operación normal
      if (leePuerta()){
        // puerta cerrada
      }else{
        // puerta abierta
        grabaled(0,255,0); // verde
        timerSalida=segSalida;
      } // fin if leepuerta
    }else{
      // switch en modo servicio, rayo no opera
      grabaled(0,0,255); // azul
    } // fin if switch 
   } // fin rutina timer
  
  // revisar receptor
  if (leeReceptor()){
    // Reviso las condiciones de las puertas. Si están distintas significa
    // que hay una puerta abierta, por lo que no se puede operar la puerta
    if (leePuerta()==leePorton()){
      actuaPorton(HIGH); 
    }
    
  } // fin repector activo
}

void reduceVariables(){
    if (timerAlarma>0){timerAlarma--;}else{actuaAlarma(LOW);}
    if (timerPorton>0){timerPorton--;}else{actuaPorton(LOW);}
    if (timerSalida>0){actuaSalida(timerSalida--);}
}
