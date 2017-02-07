#include <AccelStepper.h>
#include <LCD5110_Basic.h>


LCD5110 tela(2,3,4,5,6);
/*Cria objeto da classe LCD5110
CLK – Pino 2
DIN – Pino 3
DC – Pino 4
RST – Pino 5
CE – Pino 6
*/
 
//Obtendo as fontes
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];// pin 2 - Serial clock out (SCLK)


#define HALFSTEP 8

#define motorPin5  8     // IN1 on the ULN2003 driver 2
#define motorPin6  9     // IN2 on the ULN2003 driver 2
#define motorPin7  10    // IN3 on the ULN2003 driver 2
#define motorPin8  11    // IN4 on the ULN2003 driver 2

//Inicializa o motor nos pinos acima
AccelStepper motor(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);



//variaveis
float graus360 = 4075.7728395;
int velocidade = 500; //velocidade do motor (passos por segundo)
int passos = 0; //Serve para manter o tracking do motor o quanto falta para terminar ate ir para o proximo local
float passosConvertido = 0;
float local;
float anguloInicial = 0;
float anguloFinal;
int posicao;
float angleAz, angleEl;

#define Motor_Steps_Per_Revolution 200 //Steps Per Revolution of the motors. You may need to change this if using motors other than the ones supplied with the ThumbNet kit.
#define Motor_Degrees_Per_Step 1.8 //Define the steps per degree of the motors. You may need to change this if using motors other than the ones supplied with the ThumbNet kit.

#define AZ_Gear_Ratio 4.625 //Gear ratio of the Azimuth drive system (74t/16t)
#define EL_Gear_Ratio 3.625 //Gear ratio of the Elevation drive system (58t/16t)

void setup() {
  
  delay(2000); 
  tela.InitLCD(); //Inicializando o display
  
  
  
  
  tela.setFont(SmallFont); //Definindo a fonte
 //Escreve a palavra vida alinhada à esquerda começando na linha 0
 
 tela.print("INICIANDO", LEFT, 0);
 tela.print("O", CENTER, 20);
 tela.print("LCD", RIGHT, 40);
 delay(1000);
 tela.clrScr();
 
 //tela.print("Numero medio:", LEFT, 0);
 //tela.setFont(MediumNumbers);
 //tela.printNumI(123, CENTER, 16);
 //delay(2000);
 //tela.clrScr();
 
 //tela.setFont(SmallFont);
 //tela.print("Numero grande:", LEFT, 0);
 //tela.setFont(BigNumbers);
 //tela.printNumI(123, CENTER, 16);
 //delay(2000);
 //tela.clrScr();
 
  
  motor.setMaxSpeed(500.0);
  motor.move(-1);  //Isso foi necessario
  motor.setSpeed(velocidade);
  Serial.begin(19200);
  Serial.println("Start");
}

void loop()
{
  
  cmd_proc();
 
  
if(angleAz == 0){
//Nao faz nada  
}

else if (angleEl == 0){

  posicao = (graus360 - motor.currentPosition());
  //Serial.println(graus360);
  //Serial.println(motor.currentPosition());
  //Serial.println(posicao);
  //Se posicao for igual 4075 quer dizer que o motor ja voltou ao ponto inicial depois da passagem do satelite e nao precisa ficar mandando a informacao de retorno eternamente ate ter a proxima elevacao
  if (posicao == 0) {
    //Nao faz nada
      //Serial.println("Rotor na posicao 0");
  } else {
      //Serial.println("Voltando o rotor para posicao 0");
      motor.move(graus360 - motor.currentPosition());
      motor.setSpeed(velocidade);
      passos = motor.distanceToGo();
      motor.runSpeedToPosition();
  }
  
}

else {
  
  //primeira vez executada passos = 0
  if (passos == 0) {
    
     int val = analogRead(0);
     
     if (val > 0) {
     angleAz = map(val, 0, 1023, 0, 360);
     }
       
     tela.clrScr();
     tela.setFont(SmallFont);
     tela.print("Azimute  Graus", CENTER, 0);
     tela.setFont(MediumNumbers);
     tela.printNumF(angleAz,2, CENTER, 8);
     tela.setFont(SmallFont);
     tela.print("Elevacao Graus", CENTER,24);
     tela.setFont(MediumNumbers);
     tela.printNumF(angleEl,2, CENTER, 32);
     
     
     //Serial.print("Angulo Azimute: ");
     //Serial.println(angleAz);
     //Serial.print("Angulo Elevacao: ");
     //Serial.println(angleEl);
     //Serial.print("Valor PIN 0: ");
     //Serial.println(angleAz);
     local = 0;
     
     
  //Serial.print("Passos DistanceToGo: ");
  //Serial.println(motor.distanceToGo());
  
  //Serial.print("Posicao Corrente: ");
  //Serial.println(motor.currentPosition());
  
  //Serial.print("Local - Corrent: ");
  //Serial.println(local - motor.currentPosition());
  
 
  delay(1000);
  
  grausParaPassos(angleAz);
  motor.move(local - motor.currentPosition());
  motor.setSpeed(velocidade);
  }
  
  //Passos da posicao corrente ate o local destino
  passos = motor.distanceToGo();
  //Gira o motor na velocidade selecinada ate o local de destino
  motor.runSpeedToPosition();
  
}  
  
   
}

void cmd_proc() {
  char buffer[256];
  char incomingByte;
  char *p = buffer;
  char *str;
  static int counter = 0;
  char data[100];

  while (Serial.available() > 0) {
        incomingByte = Serial.read();
        if (incomingByte == '!') {
          counter = 0;
        } 
        
        else if (incomingByte == '\n') {
          p = buffer;
          buffer[counter] = 0;
          //Serial.println(buffer);
          if (buffer[0] == 'A' && buffer[1] == 'Z') {
                    if (buffer[2] == ' ' && buffer[3] == 'E' && buffer[4] == 'L') {
                    } 
                    else {
                      str = strtok_r(p, " " , &p);
                      strncpy(data, str + 2, 10);
                      angleAz = atof(data);
                      //stepAz = AZ_deg2step(angleAz);
                      
                      str = strtok_r(p, " " , &p);
                      if (str[0] == 'E' && str[1] == 'L') {
                          strncpy(data, str + 2, 10);
                          angleEl = atof(data);
                          //stepEl = EL_deg2step(angleEl);
                                   
                       }
                     }
          }
         }
        else {
          buffer[counter] = incomingByte;
          counter++;
        }
      }
    }
    
//passar o parametro em graus
void grausParaPassos(float par){
//devolve o local em passos
local = ((par * graus360) / 360);  
    
}

