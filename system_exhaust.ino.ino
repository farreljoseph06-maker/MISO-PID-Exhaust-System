#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PID_v1.h>
#include <DHT.h>

#define PIN_MQ135 A1  
#define PIN_DHT   2   
#define PIN_KIPAS 3   
#define PIN_LAMPU 5   

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(PIN_DHT, DHT22); 

// loop 1 avatar pengendali asap mulai kerja
double Setpoint_Asap = 30.0;      
double PV_Asap;       
double Output_Asap;    

// loop 2 kerbaunya pantau suhu
double Setpoint_Suhu = 31.0;     
double PV_Suhu;       
double Output_Suhu;    

// OUTPUT FINAL 
double Output_PWM;    

// KONSTANTA PID (kenapa pakai 0,? supaya kelihatan prosesnya secara perlahan)
double Kp = 0.5;
double Ki = 0.1;
double Kd = 0.1;

PID myPIDAsap(&PV_Asap, &Output_Asap, &Setpoint_Asap, Kp, Ki, Kd, REVERSE);
PID myPIDSuhu(&PV_Suhu, &Output_Suhu, &Setpoint_Suhu, Kp, Ki, Kd, REVERSE);

unsigned long waktuSebelumnya = 0;
const long interval = 1000; 

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  pinMode(PIN_KIPAS, OUTPUT);
  pinMode(PIN_LAMPU, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistem Exhaust");
  lcd.setCursor(0, 1);
  lcd.print("Dual Loop PID...");
  delay(3000); 
  lcd.clear();
  
  myPIDAsap.SetMode(AUTOMATIC);
  myPIDSuhu.SetMode(AUTOMATIC);
}

void loop() {
  // loop  PID pertama avatar pengendali asap
  PV_Asap = analogRead(PIN_MQ135);
  
  // (PV < Setpoint), paksa mati
  if (PV_Asap <= Setpoint_Asap) {
    Output_Asap = 0;
  } else {
    myPIDAsap.Compute(); 
  }
  
  // loop PID Thermal fail save kalau avatarnya kepanasan
  PV_Suhu = dht.readTemperature();
  
  // Failsafe kalo sensor lepas atau buta (tidak terbaca)
  if (isnan(PV_Suhu)) {
    PV_Suhu = 0; 
  }

  //(PV < Setpoint), paksa mati!
  if (PV_Suhu <= Setpoint_Suhu) {
    Output_Suhu = 0;
  } else {
    myPIDSuhu.Compute(); 
  }

  Output_PWM = max(Output_Asap, Output_Suhu);
  
  analogWrite(PIN_KIPAS, Output_PWM);
  analogWrite(PIN_LAMPU, Output_PWM);
  
  // displaynya fomo setiap 1 detik

  unsigned long waktuSekarang = millis();
  if (waktuSekarang - waktuSebelumnya >= interval) {
    waktuSebelumnya = waktuSekarang;

    int lembab = dht.readHumidity();
    
    // Baris 1 LCD
    lcd.setCursor(0, 0);
    lcd.print("A:");
    lcd.print((int)PV_Asap);
    lcd.print("   "); 
    
    lcd.setCursor(9, 0); 
    lcd.print("T:");
    lcd.print((int)PV_Suhu);
    lcd.print("C  ");

    //Baris 2 LCD
    lcd.setCursor(0, 1);
    lcd.print("PWM:");
    lcd.print((int)Output_PWM);
    lcd.print("   "); 
    
    lcd.setCursor(9, 1); 
    lcd.print("H:");
    lcd.print(lembab);
    lcd.print("%  ");
    
    // ini untuk output serial monitor supaya rapi
    Serial.print("Asap:"); Serial.print(PV_Asap); Serial.print(",");
    Serial.print("Suhu:"); Serial.print(PV_Suhu); Serial.print(",");
    Serial.print("PWM_Final:"); Serial.println(Output_PWM);
  }
}