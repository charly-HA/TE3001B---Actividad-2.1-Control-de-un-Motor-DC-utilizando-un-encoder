#define IN_1 26
#define IN_2 27
#define EN 14

#define EN_A 18
#define EN_B 19

#define PWM_CH 0
#define FREQ 5000
#define RES 8

#define DT 20000
#define REDUCTOR 34
#define CPR 12

double pos = 0;
double pos_ant = 0;
double timer = 0;
float vel = 0;

int duty_cycle = 100;

//Funcion para los cambios (FALLING) en el canal A
void IRAM_ATTR int_callback(){
  if(digitalRead(EN_B) == 0){
    pos = pos + 1;
  } else {
    pos = pos - 1;
    }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(EN, OUTPUT);

  pinMode(EN_A, INPUT);
  pinMode(EN_B, INPUT);

  attachInterrupt(EN_A, int_callback, FALLING);// Define la interrupcion para los flancos de bajada en A.

  //Configurar el canal de pwm
  ledcSetup(PWM_CH, FREQ, RES);
  ledcAttachPin(EN, PWM_CH);

  //Seleccionar sentido de giro
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  ledcWrite(PWM_CH, duty_cycle);
  
}

void loop() {
  // Revisar si ha transcurrido un periodo de tiempo
  if ((micros()-timer)>DT){
    timer = micros();
    // Estimar la velocidad del motor en RPM
    vel = (((pos - pos_ant)/DT)/(CPR*REDUCTOR))*(100000*60);
    pos_ant = pos;
    Serial.print("TIEMPO: ");
    Serial.println(timer/100000);
    Serial.print("   Posicion: ");
    Serial.print(pos);
    Serial.print("   Velocidad: ");
    Serial.println(vel);
    
    } 
}