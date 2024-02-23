
// JUSTIFICAR CAMBIOS EN EL CODIGO 

/* VARIABLES */

// PUENTE H
#define IN_1 26
#define IN_2 27
#define EN 14
#define EN_YELLOW 18
#define EN_WHITE 19

// PWM 1
#define PWM_CH1 0
#define FREQ1 5000
#define RES1 8 // Resolución 

// PWM 2
#define PWM_CH2 1
#define FREQ2 5000
#define RES2 8 // Resolución 

// MOTOR
#define DT 20000 // 
#define REDUCTOR 34
#define CPR 12 // Cuentas por revolución 

#define PI 3.1415926535897932384626433832795

// AUMENTA VELOCIDAD
int Max_vol = 6; // Cambia este por el voltaje
float CAM = (0.05*255)/Max_vol;

// VARIABLES
double pos = 0;
double pos_ant = 0;
double timer = 0;
float vel = 0;
float vel_rads;
float duty_cycle;
int val_pot;

// FUNCION INTERRUPCION
void IRAM_ATTR int_callback(){
  if(digitalRead(EN_WHITE) == 0){
    pos = pos + 1;
  }
  else {
    pos = pos - 1;
  }
}

void setup() {
  // INICIALIZAMOS SERIAL
  Serial.begin(115200);

  // PUENTE H
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(EN_YELLOW, INPUT);
  pinMode(EN_WHITE, INPUT);

  // INTERRUPCION
  attachInterrupt(EN_YELLOW, int_callback, FALLING);

  // PWN 1
  ledcSetup(PWM_CH1, FREQ1, RES1);
  ledcAttachPin(IN_1, PWM_CH1);
 // PWN 2
  ledcSetup(PWM_CH2, FREQ2, RES2);
  ledcAttachPin(IN_2, PWM_CH2);
  delay(1000);
}

void loop() {

  // CONTADOR 
  if ((micros()-timer)>DT){
    
    timer = micros();

    // VELOCIDAD EN SEGUNDOS 
    vel = (((pos - pos_ant)/DT)/(CPR*REDUCTOR))*(1000000*60);
    pos_ant = pos; // DELTA POSICION


    // VELOCIDAD EN RADIANES 
    vel_rads=(vel*60)/(2*PI);
    // CALCULO DUTY CYCLE
    //duty_cycle=(val_pot-2048)/(-20.48);

    // SENTIDO DE LOS MOTORES 
    //digitalWrite(IN_1, HIGH);
    //digitalWrite(IN_2, LOW);
    // 255 lee el pin y el maximo es 6 volts
    // 0.05 es 2.125
    ledcWrite(PWM_CH1, duty_cycle);
    ledcWrite(PWM_CH2, 0);
    if((duty_cycle*Max_vol)/255 <Max_vol){
      duty_cycle = duty_cycle+CAM;
      Serial.print(" Voltaje :");
      Serial.print((duty_cycle*Max_vol)/255); 
      //Serial.println(duty_cycle); 
      Serial.print(" Velocidad: ");
      Serial.println(vel);
    }else{
      delay(100000);
    }

    // CAMBIO DE PWM
    // ledcWrite(PWM_CH, duty_cycle);

    // PRINT DATOS 
    //Serial.print(val_pot);
    //Serial.print("   duty: ");
    /*
    Serial.print("TIEMPO: ");
    Serial.print(timer/100000);
    Serial.print("   Posición: ");
    Serial.print(pos);
    Serial.print("   Velocidad (RPM): ");
    Serial.print(vel);
    Serial.print("   Velocidad (rad/s): ");
    Serial.println(vel_rads);
    */

    }
}