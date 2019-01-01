
const int sag_arka_trig = 52;
const int arka_trig = 50;
const int sag_on_trig = 48;
const int sol_on_trig = 46;
const int on_sag_trig = 44;
const int on_sol_trig = 42;

const int sag_arka_echo = 53;
const int arka_echo = 51;
const int sag_on_echo = 49;
const int sol_on_echo = 47;
const int on_sag_echo = 45;
const int on_sol_echo = 43;

const int sag_motor_ileri = 23;
const int sag_motor_geri = 22;
const int sol_motor_ileri = 24;
const int sol_motor_geri = 25;
const int pwm_motor1 = 2;
const int pwm_motor2 = 3;

const int vakum_motor_saga = 8;
const int vakum_motor_sola = 9;
int tur = 0;
bool duvarBuldu = false;
bool solmu = true;
void setup() {

  pinMode(sag_arka_trig, OUTPUT);
  pinMode(arka_trig, OUTPUT);
  pinMode(sag_on_trig, OUTPUT);
  pinMode(sol_on_trig, OUTPUT);
  pinMode(on_sag_trig, OUTPUT);
  pinMode(on_sol_trig, OUTPUT);

  pinMode(sag_arka_echo, INPUT);
  pinMode(arka_echo, INPUT);
  pinMode(sag_on_echo, INPUT);
  pinMode(sol_on_echo, INPUT);
  pinMode(on_sag_echo, INPUT);
  pinMode(on_sol_echo, INPUT);

  pinMode(sag_motor_ileri, OUTPUT);
  pinMode(sag_motor_geri, OUTPUT);
  pinMode(sol_motor_ileri, OUTPUT);
  pinMode(sol_motor_geri, OUTPUT);

  pinMode(vakum_motor_saga, OUTPUT);
  pinMode(vakum_motor_sola, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  if (!duvarBuldu) {
    ilkDuvariBul();
  }
  int onSagMesafe = mesafeBul(on_sag_trig, on_sag_echo);
  int onSolMesafe = mesafeBul(on_sol_trig, on_sol_echo);
  while (onSagMesafe > 6 && onSolMesafe > 6) {
    duzIleriGit(34);
    delay(50);
    onSagMesafe = mesafeBul(on_sag_trig, on_sag_echo);
    onSolMesafe = mesafeBul(on_sol_trig, on_sol_echo);

  }
  if (solmu) {
    doksanDereceSolaDon();
    duzIleriGit(34);
    duzIleriGit(34);
    doksanDereceSolaDon();
    solmu = false;
  } else {
    doksanDereceSagaDon();
    duzIleriGit(34);
    duzIleriGit(34);
    doksanDereceSagaDon();
    solmu = true;
  }
}

void ilkDuvariBul() {
  int onSagMesafe = mesafeBul(on_sag_trig, on_sag_echo);
  int onSolMesafe = mesafeBul(on_sol_trig, on_sol_echo);
  while (onSagMesafe > 6 && onSolMesafe > 6) {
    if (onSagMesafe > 50 && onSolMesafe > 50)
      duzIleriGit(44);
    else if (onSagMesafe > 35 && onSolMesafe > 35)
      duzIleriGit(40);
    else if (onSagMesafe > 25 && onSolMesafe > 25)
      duzIleriGit(37);
    else if (onSagMesafe > 19 && onSolMesafe > 19)
      duzIleriGit(35);
    else if (onSagMesafe > 12 && onSolMesafe > 12)
      duzIleriGit(32);
    else
      duzIleriGit(30);
    delay(50);
    onSagMesafe = mesafeBul(on_sag_trig, on_sag_echo);
    onSolMesafe = mesafeBul(on_sol_trig, on_sol_echo);
  }
  doksanDereceSolaDon();
  kaymaDuzelt(2);
  int arkaMesafe = mesafeBul(arka_trig, arka_echo);
  while (arkaMesafe > 7 ) {
    duzGeriGit(30);
    delay(50);
    arkaMesafe = mesafeBul(arka_trig, arka_echo);
  }
  duvarBuldu = true;

}

void kaymaDuzelt(int hata) {
  int fark = farkBul();
  delay(50);
  int tur=0;
  while (true) {
    if (fark >= hata) {
      if(tur==5||tur==6){
              sagaDon(300);
              tur=0;
      }else
      sagaDon(400);
    }
    else if (fark <= -hata) {
      if(tur==5||tur==6){
             solaDon(500);
             tur=0;
      }else
     solaDon(400);
    }
    else
      break;
    fark = farkBul();
    tur++;
  }
}

int farkBul() {
  delay(50);
  int sagOnMesafe = mesafeBul(sag_on_trig, sag_on_echo);
  int sagArkaMesafe = mesafeBul(sag_arka_trig, sag_arka_echo);
  int fark = sagOnMesafe - sagArkaMesafe;
  return fark;
}

void duzGeriGit(int adim) {
  for (int i = 0; i < 100 * adim; i++) {
    pwm_motor_ayarla(i);
    geriCalis();
  }
  hareketMotorDur();
  int fark = farkBul();
  if (fark < 5) {
    if (adim < 34) {
      kaymaDuzelt(2);
    } else {
      kaymaDuzelt(3);
    }
  }

}

void duzIleriGit(int adim) {
  for (int i = 0; i < 100 * adim; i++) {
    pwm_motor_ayarla(i);
    ileriCalis();
  }
  hareketMotorDur();
  int fark = farkBul();
  if (fark < 5) {
    if (adim < 34) {
      kaymaDuzelt(2);
    } else {
      kaymaDuzelt(3);
    }
  }
}

void farkIcinSolaDon(int istenenFark, int on, int arka) {
  int fark = on - arka;
  int sagOnMesafe = on;
  int sagArkaMesafe = arka;
  while (fark > istenenFark) {
    solaDon(600);
    fark = farkBul();
  }
  solaDon(800);
  while (fark < -istenenFark) {
    solaDon(600);
    fark = farkBul();
  }
  solaDon(400);
  solaDon(200);
  kaymaDuzelt(2);
}

void doksanDereceSolaDon() {
  solaDon(1200);
  delay(50);
  int sagOnMesafe = mesafeBul(sag_on_trig, sag_on_echo);
  int sagArkaMesafe = mesafeBul(sag_arka_trig, sag_arka_echo);
  int fark = sagOnMesafe - sagArkaMesafe;
  delay(50);
  if (fark > 0) {
    farkIcinSolaDon(1, sagOnMesafe, sagArkaMesafe);
  }
  else {
    farkIcinSolaDon(-1, sagOnMesafe, sagArkaMesafe);
  }
}

void farkIcinSagaDon(int istenenFark, int on, int arka) {
  int fark = on - arka;
  int sagOnMesafe = on;
  int sagArkaMesafe = arka;
  while (fark > istenenFark) {
    sagaDon(600);
    fark = farkBul();
  }
  sagaDon(800);
  while (fark < -istenenFark) {
    sagaDon(600);
    fark = farkBul();
  }
  sagaDon(400);
  sagaDon(200);
  kaymaDuzelt(2);
}

void doksanDereceSagaDon() {
  sagaDon(1200);
  delay(50);
  int sagOnMesafe = mesafeBul(sag_on_trig, sag_on_echo);
  int sagArkaMesafe = mesafeBul(sag_arka_trig, sag_arka_echo);
  int fark = sagOnMesafe - sagArkaMesafe;
  delay(50);
  if (fark > 0) {
    farkIcinSagaDon(1, sagOnMesafe, sagArkaMesafe);
  }
  else {
    farkIcinSagaDon(-1, sagOnMesafe, sagArkaMesafe);
  }
}

void pwm_motor_ayarla(int n_adim) {
  int pwm = pwm_bul(n_adim);
  analogWrite(pwm_motor1, pwm);
  analogWrite(pwm_motor2, pwm);
}

int pwm_bul(int n_adim) {
  int deger = 0;
  deger = 130 + (n_adim / 100 - 20) * 2;
  if (deger > 200)
    deger = 200;
  return deger;
}

void ileriCalis() {
  digitalWrite(sag_motor_ileri, HIGH);
  digitalWrite(sag_motor_geri, LOW);
  digitalWrite(sol_motor_ileri, HIGH);
  digitalWrite(sol_motor_geri, LOW);
}

void geriCalis() {
  digitalWrite(sag_motor_ileri, LOW);
  digitalWrite(sag_motor_geri, HIGH);
  digitalWrite(sol_motor_ileri, LOW);
  digitalWrite(sol_motor_geri, HIGH);
}

void sagIleriSolGeriCalis() {
  digitalWrite(sag_motor_ileri, HIGH);
  digitalWrite(sag_motor_geri, LOW);
  digitalWrite(sol_motor_ileri, LOW);
  digitalWrite(sol_motor_geri, HIGH);
}

void solIleriSagGeriCalis() {
  digitalWrite(sag_motor_ileri, LOW);
  digitalWrite(sag_motor_geri, HIGH);
  digitalWrite(sol_motor_ileri, HIGH);
  digitalWrite(sol_motor_geri, LOW);
}

void hareketMotorDur() {
  digitalWrite(sag_motor_ileri, LOW);
  digitalWrite(sag_motor_geri, LOW);
  digitalWrite(sol_motor_ileri, LOW);
  digitalWrite(sol_motor_geri, LOW);
  analogWrite(pwm_motor1, 0);
  analogWrite(pwm_motor2, 0);
}

void solaDon(int adim) {
  if (adim < 600) {
    analogWrite(pwm_motor1, 140);
    analogWrite(pwm_motor2, 140);
  } else {
    analogWrite(pwm_motor1, 180);
    analogWrite(pwm_motor2, 180);
  }
  for (int i = 0; i < 10 * adim; i++) {
    sagIleriSolGeriCalis();
  }
  hareketMotorDur();
}

void sagaDon(int adim) {
  if (adim < 600) {
    analogWrite(pwm_motor1, 140);
    analogWrite(pwm_motor2, 140);
  } else {
    analogWrite(pwm_motor1, 180);
    analogWrite(pwm_motor2, 180);
  }
  for (int i = 0; i < 10 * adim; i++) {
    solIleriSagGeriCalis();
  }
  hareketMotorDur();
}

int mesafeBul(int trig, int echo) {
  delay(50);
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);
  int sure = pulseIn(echo, HIGH);
  int mesafe = (sure / 2) / 29.1;
  if (mesafe < 0)
    mesafe = -mesafe;
  // Serial.print(mesafe);
  // Serial.println("cm.\n\n");
  delay(50);
  return mesafe;
}

