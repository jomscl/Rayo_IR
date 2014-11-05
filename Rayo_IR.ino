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
//#include <Console.h>

// depuración
#define HABILITA_DEBUG    // comentar para no debuggear
//#define DEBUG_YUN

// Macros de impresión por serial
#ifdef HABILITA_DEBUG
  #ifdef DEBUG_YUN
    #define DEBUG(x)  Console.print (x)
    #define DEBUGLN(x)  Console.println (x)
  #else
    #define DEBUG(x)  Serial.print (x)
    #define DEBUGLN(x)  Serial.println (x)
  #endif
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
  #define rayo 7  // op4
  #define puerta 6  // op3
  #define porton 5  // op2
  #define receptor 4 // op1
  
  // salidas
  #define alarma A0 // relay 1
  #define swPorton A1 // relay 2
  
  // switch
  #define interruptor 8

  // constantes
  #define segAlarma 20
  #define segPorton 1
  #define segSalida 200

// variables
Metro pulso = Metro(100); 

unsigned timerAlarma=0;
unsigned timerPorton=0;
unsigned timerSalida=0;

void setup() {
  // habilitación del debug serial
  #ifdef HABILITA_DEBUG
    #ifdef DEBUG_YUN
       // Initialize Console and wait for port to open:
      Bridge.begin();
      Console.begin();
    
      // Wait for Console port to connect
      while (!Console);
    #else
      Serial.begin(9600);
      //while (!Serial.available()){Serial.println("a");}
    #endif
  #endif
 
  DEBUGLN("Encendido");
  
  // leds
  pinMode(ledp,OUTPUT);
  for (int i=0;i<2;i++){pinMode(led[i],OUTPUT);}
  
  // entradas Para debug INPUT_PULLUP
  pinMode(rayo,INPUT_PULLUP);
  pinMode(puerta,INPUT_PULLUP);
  pinMode(porton,INPUT_PULLUP);
  pinMode(receptor,INPUT_PULLUP);
  
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
    
    // revisar el rayo, para efectos de alarma
    if (leeSwitch()){ // swirch armado, operación normal
      if (leePuerta()){ // puerta cerrada
        if (timerSalida==0){ // si se acabó el tiempo de salida
          if (timerAlarma==0){
            if  (leeRayo()){ // rayo cortado
              grabaled(255,0,255); // morado
              actuaAlarma(HIGH);
            }else{ // rayo no cortado
               grabaled(255,0,0); // rojo
            } // fin if leerayo
          }else{ // timer alarma mayor que cero
            grabaled(255,0,255); // morado
          } // fin if timer alarma
        } // fin if timer salida
      }else{ // puerta abierta
        timerSalida=segSalida;
      } // fin if leepuerta
    }else{ // switch en modo servicio, rayo no opera
      grabaled(0,0,255); // azul
      timerSalida=0;
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
