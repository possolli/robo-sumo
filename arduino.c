// === Sensores IR ===
#define IR_FR 2
#define IR_FL 3
#define IR_BR 4
#define IR_BL 5

// === Sensor Ultrassônico ===
#define TRIG 12
#define ECHO 13

// === Motores ===
#define ENA 11
#define IN1 10
#define IN2 9
#define IN3 8
#define IN4 7
#define ENB 6

const int velocidadeA = 255;
const int velocidadeB = 255;
const int distanciaAlvo = 25; // distância para detectar oponente

void setup() {
  pinMode(IR_FR, INPUT);
  pinMode(IR_FL, INPUT);
  pinMode(IR_BR, INPUT);
  pinMode(IR_BL, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
  Serial.println("Iniciando em 2 segundos...");
  delay(2000);
}

// === Funções de Movimento ===

void avancar() {
  analogWrite(ENA, velocidadeA);
  analogWrite(ENB, velocidadeB);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void recuar() {
  analogWrite(ENA, velocidadeA);
  analogWrite(ENB, velocidadeB);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void parar() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void girarReEsquerda() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 180);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW);
}

void girarReDireita() {
  analogWrite(ENA, 180);
  analogWrite(ENB, 255);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void avancarEsquerda() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 180);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void avancarDireita() {
  analogWrite(ENA, 180);
  analogWrite(ENB, 255);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void girarBuscarOponente() {
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);  // Motor A ré
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH); // Motor B frente
}

long lerDistanciaCM() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long duracao = pulseIn(ECHO, HIGH, 20000); // timeout 20ms
  return duracao * 0.034 / 2;
}

void loop() {
  bool ir_fr = digitalRead(IR_FR);
  bool ir_fl = digitalRead(IR_FL);
  bool ir_br = digitalRead(IR_BR);
  bool ir_bl = digitalRead(IR_BL);
  long distancia = lerDistanciaCM();

  Serial.print("FR: "); Serial.print(ir_fr);
  Serial.print(" | FL: "); Serial.print(ir_fl);
  Serial.print(" | BR: "); Serial.print(ir_br);
  Serial.print(" | BL: "); Serial.print(ir_bl);
  Serial.print(" | Dist: "); Serial.print(distancia); Serial.println("cm");

  // === BORDAS COM PRIORIDADE ABSOLUTA ===

  if (ir_fr == LOW) {
    Serial.println("-> IR_FR ATIVADO: RÉ PARA ESQUERDA");
    while (digitalRead(IR_FR) == LOW) {
      girarReEsquerda();
    }
    parar(); delay(100);
  }
  else if (ir_fl == LOW) {
    Serial.println("-> IR_FL ATIVADO: RÉ PARA DIREITA");
    while (digitalRead(IR_FL) == LOW) {
      girarReDireita();
    }
    parar(); delay(100);
  }
  else if (ir_br == LOW && distancia < distanciaAlvo) {
    Serial.println("-> IR_BR + Oponente: RÉ ESQUERDA AGRESSIVA");
    while (digitalRead(IR_BR) == LOW) {
      girarReEsquerda();
    }
    parar(); delay(100);
  }
  else if (ir_bl == LOW && distancia < distanciaAlvo) {
    Serial.println("-> IR_BL + Oponente: RÉ DIREITA AGRESSIVA");
    while (digitalRead(IR_BL) == LOW) {
      girarReDireita();
    }
    parar(); delay(100);
  }
  else if (ir_br == LOW) {
    Serial.println("-> IR_BR ATIVADO: AVANÇAR VIRANDO PARA ESQUERDA");
    while (digitalRead(IR_BR) == LOW) {
      avancarEsquerda();
    }
    parar(); delay(100);
  }
  else if (ir_bl == LOW) {
    Serial.println("-> IR_BL ATIVADO: AVANÇAR VIRANDO PARA DIREITA");
    while (digitalRead(IR_BL) == LOW) {
      avancarDireita();
    }
    parar(); delay(100);
  }

  // === MODO DE ATAQUE ===
  else if (distancia > 0 && distancia < distanciaAlvo) {
    Serial.println("-> OPONENTE DETECTADO: AVANÇANDO");
    avancar();
  }

  // === MODO DE BUSCA ===
  else {
    Serial.println("-> NENHUM OPONENTE: GIRANDO");
    girarBuscarOponente();
  }

  delay(100);
}