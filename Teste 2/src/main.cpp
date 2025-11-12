#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <EmonLib.h>   // Biblioteca para medição de energia

// ===========================
// CONFIGURAÇÕES DE REDE (opcional)
// ===========================
#define WIFI_SSID ""
#define WIFI_PASS ""

// ===========================
// CONFIGURAÇÕES DO FIREBASE (opcional)
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
//
// Esses valores devem ser ajustados de acordo com o seu módulo.
// Você precisará fazer um teste com multímetro e carga conhecida.
//
#define CALIBRACAO_TENSAO 40.9   // Ajuste até o valor RMS bater com o multímetro
#define CALIBRACAO_CORRENTE 1.98  // Ajuste conforme o sensor ACS712 (ex: 30A = 26.4, 20A = 19.5, 5A = 11.1)
#define AJUSTE_FASE 1.7           // Corrige a defasagem entre tensão e corrente

// ===========================
// OBJETOS DA BIBLIOTECA
// ===========================
EnergyMonitor emon1;   // Objeto principal da EmonLib
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Inicializando sistema de medição...");

  // Configuração do EmonLib
  emon1.voltage(PINO_TENSAO, CALIBRACAO_TENSAO, AJUSTE_FASE);
  emon1.current(PINO_CORRENTE, CALIBRACAO_CORRENTE);

  // ====== (opcional) Conexão Wi-Fi ======
  /*
  Serial.println("Conectando-se ao Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
  */

  // ====== (opcional) Inicialização do Firebase ======
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
  // Mede tensão e corrente durante 20 ciclos da rede (20 x 16,6ms = ~333ms)
  emon1.calcVI(20, 2000);

  // Leitura dos valores RMS e potências
  float tensao = emon1.Vrms;
  float corrente = emon1.Irms;
  float potencia_real = emon1.realPower;        // W (considera fator de potência)
  float potencia_aparente = emon1.apparentPower; // VA
  float fator_potencia = emon1.powerFactor;

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
  Serial.println("===================================");

  // ====== (opcional) Envio para o Firebase ======
  /*
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000)) {
    sendDataPrevMillis = millis();
    Firebase.RTDB.setFloat(&fbdo, "/medidas/voltagem", tensao);
    Firebase.RTDB.setFloat(&fbdo, "/medidas/corrente", corrente);
    Firebase.RTDB.setFloat(&fbdo, "/medidas/potencia_real", potencia_real);
    
  }
  */

  delay(500);
}
