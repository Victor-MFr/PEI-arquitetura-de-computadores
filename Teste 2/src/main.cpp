#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <EmonLib.h>

// ===========================
// CONFIGURAÇÕES DE REDE
// ===========================
#define WIFI_SSID ""
#define WIFI_PASS ""

// ===========================
// CONFIGURAÇÕES DO FIREBASE
// ===========================
#define API_KEY "AIzaSyCe32jGPp6BfjjAslNZfBsxv35tPaslyM8"
#define DATABASE_URL "https://arquitetura-de-computado-7ad5a-default-rtdb.firebaseio.com"
#define USER_EMAIL "victorfragoso88@gmail.com"
#define USER_PASSWORD "123456789"

// ===========================
// PINOS DOS SENSORES
// ===========================
#define PINO_TENSAO 35     // ZMPT101B
#define PINO_CORRENTE 36   // ACS712
#define RELAY 32

// ===========================
// CALIBRAÇÃO DOS SENSORES
// ===========================

#define CALIBRACAO_TENSAO 40.9
#define CALIBRACAO_CORRENTE 1.98
#define AJUSTE_FASE 1.7

// ===========================
// OBJETOS DA BIBLIOTECA
// ===========================
EnergyMonitor emon1;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;

// ===========================
// VARIÁVEIS GLOBAIS
// ===========================
int contador = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Inicializando sistema de medição...");

  pinMode(RELAY, OUTPUT);

  // Configuração do EmonLib
  emon1.voltage(PINO_TENSAO, CALIBRACAO_TENSAO, AJUSTE_FASE);
  emon1.current(PINO_CORRENTE, CALIBRACAO_CORRENTE);

  // ====== Conexão Wi-Fi ======
  /*
  Serial.println("Conectando-se ao Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
  */

  // ====== Inicialização do Firebase ======
  /*
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.reconnectNetwork(true);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);
  */
}

void loop() {
  int relay_status = digitalRead(RELAY);

  emon1.calcVI(20, 2000);

  // Leitura dos valores RMS e potências
  float tensao = emon1.Vrms;
  float corrente = emon1.Irms;
  float potencia_real = emon1.realPower;        // Watts
  float potencia_aparente = emon1.apparentPower; // VA
  float fator_potencia = emon1.powerFactor;

  if(tensao < 80 && contador > 20){
    tensao = 0;
    corrente = 0;
    potencia_real = 0;
  } else if(tensao > 238 && contador > 20){
    tensao = 220;
  }

  // Exibição no monitor serial
  Serial.println("===================================");
  Serial.print("Tensão RMS: ");
  Serial.print(tensao, 2);
  Serial.println(" V");

  Serial.print("Corrente RMS: ");
  Serial.print(corrente, 3);
  Serial.println(" A");

  Serial.print("Potência Real: ");
  Serial.print(potencia_real, 2);
  Serial.println(" W");

  Serial.print("Potência Aparente: ");
  Serial.print(potencia_aparente, 2);
  Serial.println(" VA");

  Serial.print("Fator de Potência: ");
  Serial.println(fator_potencia, 2);

  Serial.print("Leitura nº: ");
  Serial.println(contador);

  Serial.println("===================================");

  // ====== Envio para o Firebase ======
  /*if(contador > 20){
      if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000)) {
      sendDataPrevMillis = millis();
      Firebase.RTDB.setFloat(&fbdo, "/medidas/voltagem", tensao);
      Firebase.RTDB.setFloat(&fbdo, "/medidas/corrente", corrente);
      Firebase.RTDB.setFloat(&fbdo, "/medidas/potencia_real", potencia_real);
      Firebase.RTDB.setFloat(&fbdo, "/medidas/relay_status", relay_status);
      }
  }
  */
  contador++;
  delay(500);
}
