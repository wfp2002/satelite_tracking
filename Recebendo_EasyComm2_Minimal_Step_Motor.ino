#include <AccelStepper.h>
#define HALFSTEP 8

#define motorPin5  8     // IN1 on the ULN2003 driver 2
#define motorPin6  9     // IN2 on the ULN2003 driver 2
#define motorPin7  10    // IN3 on the ULN2003 driver 2
#define motorPin8  11    // IN4 on the ULN2003 driver 2

//Inicializa o motor nos pinos acima
AccelStepper motor(HALFSTEP, motorPin5, motorPin7, motorPin6, motorPin8);

//variaveis
float graus360 = 4075.7728395;
int velocidade = 1000; //velocidade do motor (passos por segundo)
int passos = 0; //Serve para manter o tracking do motor o quanto falta para terminar ate ir para o proximo local
float passosConvertido = 0;
float local;
float anguloInicial = 0;
float anguloFinal;

float angleAz, angleEl;

#define Motor_Steps_Per_Revolution 200 //Steps Per Revolution of the motors. You may need to change this if using motors other than the ones supplied with the ThumbNet kit.
#define Motor_Degrees_Per_Step 1.8 //Define the steps per degree of the motors. You may need to change this if using motors other than the ones supplied with the ThumbNet kit.

#define AZ_Gear_Ratio 4.625 //Gear ratio of the Azimuth drive system (74t/16t)
#define EL_Gear_Ratio 3.625 //Gear ratio of the Elevation drive system (58t/16t)

void setup() {
  
  delay(3000); 

  motor.setMaxSpeed(2000.0);
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
  
  //Colocar um IF aqui para fazer o mnotor para pq aqui depois q o satelite sai ele fica mandando pra posicao zero eternamente, colocar um AZ_MOTOR _ENAVBLE = LOW ou 
  //se graus360-curret = 4075 nao faz nada.
  motor.move(graus360 - motor.currentPosition());
  motor.setSpeed(velocidade);
  passos = motor.distanceToGo();
  motor.runSpeedToPosition();

  
}

else {
  
  //primeira vez executada passos = 0
  if (passos == 0) {
     Serial.print("Angulo Azimute: ");
     Serial.println(angleAz);
     Serial.print("Angulo Elevacao: ");
     Serial.println(angleEl);
 
     local = 0;
     
     
  //Serial.print("Passos DistanceToGo: ");
  //Serial.println(motor.distanceToGo());
  
  //Serial.print("Posicao Corrente: ");
  //Serial.println(motor.currentPosition());
  
  //Serial.print("Local - Corrent: ");
  //Serial.println(local - motor.currentPosition());
  
 
  delay(3000);
  
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

