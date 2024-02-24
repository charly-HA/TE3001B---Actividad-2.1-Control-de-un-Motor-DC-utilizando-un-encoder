#define IN_1 26
#define IN_2 27
#define EN 14

#define EN_A 18
#define EN_B 19

#define PWM_CH 0
#define FREQ 5000
#define RES 8

double pos = 0;
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
  
}

void loop() {
  // put your main code here, to run repeatedly:
  ledcWrite(PWM_CH, duty_cycle);
  Serial.println(pos);
 
}
