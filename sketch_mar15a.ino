#include <SPI.h> // Comunicação SPI com módulo GPS
#include <SD.h> // E/S com cartão SD
#include <SoftwareSerial.h> // Emular porta serial para interface com cartão SD e USB
#include <TinyGPS.h> // Tratar codigo NMEA do GPS

#define numero_de_compartimentos 4
#define botao_carregar 8
#define led_cartao 5
#define tempo_para_precionar_botao 2000

void GPS();
void lerCartao ();

File arqCoordenadas;
TinyGPS gps;
SoftwareSerial ss(7,6); // (TX,RX)

long int intcoordenadas[numero_de_compartimentos][4]; // coordenadas lidas
long int lat,lon;
unsigned long int tempo_precionado,tempo_led = 0;
boolean leu = false, precionado = false;


void setup() {
 pinMode(botao_carregar,INPUT);
 pinMode(led_cartao,OUTPUT);
 Serial.begin(9600);
 ss.begin(9600);
}

void loop() {
  botaoPrecionado();
  
  if(leu){
    piscaLed(led_cartao,3000);}
  else{
    digitalWrite(led_cartao,HIGH);}

 

}

void botaoPrecionado (){ //Verifica continuamente se o botão foi precionado

  if(digitalRead(botao_carregar)){ // O botão foi precionado ?
    if(precionado == false){ // O botão já vinha sendo precionado antes ? 
      precionado = true; // Marque que ele está sendo precionado e...
      Serial.println("Botao precionado");
      tempo_precionado = millis();}}// Registre o momento em segundos em que ele começou a ser precionado
  
  if(!digitalRead(botao_carregar)){ // Se o botao não estiver sendo precionado nesse momento
    precionado = false;}    //marque como falso 
  
  if(millis() - tempo_precionado >= tempo_para_precionar_botao){ // Já se passou mais que o tempo determinado para o usuario segurar? 
    if(precionado == true){// ele ainda está precionando ?
      Serial.println("carregar");
      lerCartao();}
       
         
   tempo_precionado += tempo_para_precionar_botao; // Se o usuário manter o botão precionado, atualiza o tempo para repetir a ação de carregar ao fim do tempo
}}
void lerCartao () {
       
        if (!SD.begin(10)) { // Se não conseguir contato 
        leu = false;
        Serial.print("erro");
        return;} // colocar o led para ascender aqui.*/
   
    arqCoordenadas = SD.open("coor.txt", FILE_READ);

       if (arqCoordenadas) {
           Serial.println("Arquivo aberto lendo coordenadas");
           lerCoordenadas();
           leu = true;
           for(int i = 0; i < 4; i++ ){
               Serial.println(intcoordenadas[0][i]);}}    
       else {
       Serial.println("Erro ao abrir o arquivo");
       leu = false;}
    
    arqCoordenadas.close();
}

static void piscaLed(int pino,int temporizacao){
     
     if((millis() - tempo_led) > temporizacao){ 
         tempo_led = millis();
         digitalWrite(pino,HIGH);}
        else{
         digitalWrite(pino,LOW);}}


void lerCoordenadas (){
  String coordenadas[4] = {"","","",""};
  char aux = ' ';
  int colunas = 0;

  while((arqCoordenadas.available()) && (colunas < 4)){
    aux = arqCoordenadas.read();
    
    if(aux == ','){
      colunas++;}
    else{        
      if((aux == '-') || (aux == '0') || (aux == '1') || (aux == '2') || (aux == '3') 
      || (aux == '4') || (aux == '5') || (aux == '6') || (aux == '7') || (aux == '8') 
      || (aux == '9')) {
        coordenadas[colunas].concat(aux);}}
      delay(10);  
  }
    for(int i = 0; i < 4; i++){
     intcoordenadas[0][i] = coordenadas[i].toInt();
  } 
}
/*
void GPS(){
 bool dadosGPS = false;
  unsigned long chars;
  unsigned short sentences;

  for (unsigned long inicio = millis(); millis() - inicio < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c)) 
        dadosGPS = true;
    }
  }

  if (dadosGPS)
  {
    unsigned long age;
    gps.get_position(&lat, &lon, &age);
    Serial.print("LAT=");
    Serial.print(lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lat, 0);
    Serial.print(" LON=");
    Serial.print(lon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lon, 0);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    Serial.print("  ");
    dentroFora();
  }
}


void dentroFora(){
 if((intcoordenadas[0] < lat ) && (intcoordenadas[1] < lon) && (intcoordenadas[2] > lat) && (intcoordenadas[3] > lon)){
  Serial.println("dentro");}
 else Serial.println("FORA");
}
*/



