bool leeSwitch(){
  bool estado;
  estado=digitalRead(interruptor);
  return estado;  
}

bool leeRayo(){
  bool estado;
  estado=digitalRead(rayo);
  return estado;  
}

bool leePuerta(){
  bool estado;
  estado=digitalRead(puerta);
  return estado;  
}

bool leePorton(){
  bool estado;
  estado=digitalRead(porton);
  return estado;  
}

bool leeReceptor(){
  bool estado;
  estado=digitalRead(receptor);
  return estado;  
}

void actuaAlarma(bool estado){
  digitalWrite(alarma,estado);
  if (estado && timerAlarma==0){timerAlarma=segAlarma;DEBUGLN("Alarma Activa");}
}

void actuaPorton(bool estado){
  digitalWrite(porton,estado);
  if (estado && timerPorton==0){timerPorton=segPorton;DEBUGLN("Porton Activo");}
}

void actuaSalida(unsigned int tiempo){
  if (tiempo%2 == 0)
    {
      grabaled(255,0,0); // rojo
    }
    else
    {
      grabaled(0,255,0);  // verde
    }  
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