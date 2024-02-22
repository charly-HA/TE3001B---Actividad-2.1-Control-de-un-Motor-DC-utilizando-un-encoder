// no funcionó correctamente con PWM en enable, porque la corriente que da el puente H no es suficiente para mover el motor

// PUENTE H
#define IN_1 26
#define IN_2 27
#define EN_YELLOW 18
#define EN_WHITE 19

// PWM 
#define PWM_CH1 0
#define PWM_CH2 1
#define FREQ1 5000
#define RES1 8 // Resolución 
#define FREQ2 5000
#define RES2 8 // Resolución 
#define DT 20000 // 
#define REDUCTOR 34
#define CPR 12 // Cuentas por revolución 

// PONTENCIÓMETRO
#define POT 15
#define PI 3.1415926535897932384626433832795

// VARIABLES
double pos = 0;
double pos_ant = 0;
double timer = 0;
float vel = 0;
float vel_rads;
int duty_cycle;
int val_pot;

// FUNCIÓN INTERRUPCIÓN
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

  // PONTENCIÓMETRO
  pinMode(POT, INPUT);

  // INTERRUPCIÓN
  attachInterrupt(EN_YELLOW, int_callback, FALLING);

  // PWM 1
  ledcSetup(PWM_CH1, FREQ1, RES1);
  ledcAttachPin(IN_1, PWM_CH1);
  
  // PWM 2
  ledcSetup(PWM_CH2, FREQ2, RES2);
  ledcAttachPin(IN_2, PWM_CH2);
}

void loop() {

  // CONTADOR 
  if ((micros()-timer)>DT){
    timer = micros();

    // VELOCIDAD EN SEGUNDOS 
    vel = (((pos - pos_ant)/DT)/(CPR*REDUCTOR))*(100000*60);
    pos_ant = pos; // DELTA POSICION

    // VELOCIDAD EN RADIANES 
    vel_rads=(vel*60)/(2*PI);

    // LEER POTENCIÓMETRO
    val_pot=analogRead(POT);

    if (val_pot<2048){
      // CÁLCULO DUTY CYCLE
      duty_cycle=map(val_pot,0,2048,255,0);
      // SENTIDO DE LOS MOTORES 
      ledcWrite(PWM_CH1, duty_cycle);
      ledcWrite(PWM_CH2, 0);  
    }
    else if (val_pot>=2048){
      // CÁLCULO DUTY CYCLE
      duty_cycle=map(val_pot,2049,4096,0,255);
      // SENTIDO DE LOS MOTORES 
      ledcWrite(PWM_CH1, 0);
      ledcWrite(PWM_CH2, duty_cycle);
    }

    // PRINT DATOS 
    Serial.println(val_pot);
    Serial.print("   duty: ");
    Serial.print(duty_cycle); 
    Serial.print("   Tiempo: ");
    Serial.print(timer/100000);
    Serial.print("   Posición: ");
    Serial.print(pos);
    Serial.print("   Velocidad (RPM): ");
    Serial.println(vel);
    Serial.print("   Velocidad (rad/s): ");
    Serial.println(vel_rads);
    }
}
