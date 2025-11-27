#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <EmonLib.h>

// ===========================
// CONFIGURAÇÕES DE REDE
// ===========================
#define WIFI_SSID "PEI"
#define WIFI_PASS "123456789"

// ===========================
// CONFIGURAÇÕES DO FIREBASE
// ===========================
#define API_KEY "AIzaSyC8bqCXthUftcj2c0dzHIW3j_gReOd4rPE"
#define DATABASE_URL "arquitetura-de-computado-54e7e-default-rtdb.firebaseio.com"
#define USER_EMAIL "victorfragoso88@gmail.com"
#define USER_PASSWORD "123456789"

// ===========================
// PINOS DOS SENSORES
// ===========================
#define PINO_TENSAO 35     // ZMPT101B
#define PINO_CORRENTE 36   // ACS712
#define RELAY 26

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
int idMedicao = 1;

// ===========================
// DECLARAÇÃO DE FUNÇÕES
// ===========================
int recuperarUltimoId(); 

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Inicializando sistema de medição...");

  pinMode(RELAY, OUTPUT);

  // Configuração do EmonLib
  emon1.voltage(PINO_TENSAO, CALIBRACAO_TENSAO, AJUSTE_FASE);
  emon1.current(PINO_CORRENTE, CALIBRACAO_CORRENTE);

  // ====== Conexão Wi-Fi ======
  Serial.println("Conectando-se ao Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado!");

  // Configuração do Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);

  // Inicializar Firebase
  Serial.println("Inicializando Firebase...");
  Firebase.begin(&config, &auth);
  config.timeout.serverResponse = 10 * 1000;

  if (Firebase.ready()) {
    Serial.println("Firebase inicializado com sucesso!");
  } else {
    Serial.println("Falha ao conectar com o Firebase!");
  }

  Serial.println("Buscando último ID salvo no Firebase...");

  int ultimo = recuperarUltimoId();
  idMedicao = ultimo + 1;

  Serial.print("Última medição encontrada: ");
  Serial.println(ultimo);
  Serial.print("Próxima medição será: ");
  Serial.println(idMedicao);
}

void loop() {
  int relay_status = digitalRead(RELAY);

  emon1.calcVI(20, 2000);

  // Leitura dos valores RMS e potências
  float tensao = emon1.Vrms;// Volts
  float corrente = emon1.Irms;// Amperes
  float potencia_real = emon1.realPower;// Watts
  float potencia_aparente = emon1.apparentPower; // VA
  float fator_potencia = emon1.powerFactor;

  if(tensao < 80 && contador > 20){
    tensao = 0;
    corrente = 0;
    potencia_real = 0;
  } else if(tensao > 238 && contador > 20){
    tensao = 220;
  } else if(tensao > 130 && tensao < 160 && contador > 20){
    tensao = 127;
  } else if(tensao > 160 && tensao < 200 & contador > 20){
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

  Serial.print("Status do Relay: ");
  if(relay_status == HIGH){
    Serial.println("LIGADO");
  } else if(relay_status == LOW){
    Serial.println("DESLIGADO");
  }

  Serial.print("Leitura nº: ");
  Serial.println(contador);

  // Verifica se a conexão Wi-Fi está ativa
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi perdido, tentando reconectar...");
    WiFi.reconnect();
    delay(1000);
  }

  // ====== Envio para o Firebase ======
  if(contador > 1){
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000)) {
      sendDataPrevMillis = millis();

      char caminho[100];

      sprintf(caminho, "/medidas/Medicao-%d/voltagem", idMedicao);
      Firebase.RTDB.setFloat(&fbdo, caminho, tensao);

      sprintf(caminho, "/medidas/Medicao-%d/corrente", idMedicao);
      Firebase.RTDB.setFloat(&fbdo, caminho, corrente);

      sprintf(caminho, "/medidas/Medicao-%d/potencia_real", idMedicao);
      Firebase.RTDB.setFloat(&fbdo, caminho, potencia_real);

      sprintf(caminho, "/medidas/Medicao-%d/relay_status", idMedicao);
      Firebase.RTDB.setInt(&fbdo, caminho, relay_status);

      Serial.printf("Salvo em: /medidas/Medicao-%d\n", idMedicao);

      idMedicao++;

      // Leitura de comando para controle do relé
      if (Firebase.RTDB.getInt(&fbdo, "/controle_relay")) {
        int relay_command = fbdo.intData();
        if (relay_command == 0) {
          digitalWrite(RELAY, HIGH);
          Serial.println("Relé desligado pelo Firebase (DESLIGADO)");
        } else if (relay_command == 1) {
          digitalWrite(RELAY, LOW);
          Serial.println("Relé acionado pelo Firebase (LIGADO)");
        }
      }
    }
  }
  Serial.println("===================================");

  contador++;
  delay(500);
}

int recuperarUltimoId() {
  if (Firebase.RTDB.get(&fbdo, "/medidas")) {
    if (fbdo.dataType() == "json") {

      FirebaseJson json = fbdo.jsonObject();
      size_t len = json.iteratorBegin();

      int maiorId = 0;

      for (size_t i = 0; i < len; i++) {
        String key, value;
        int type;

        json.iteratorGet(i, type, key, value);

        if (key.startsWith("Medicao-")) {
          int numero = key.substring(9).toInt();
          if (numero > maiorId) {
            maiorId = numero;
          }
        }
      }

      json.iteratorEnd();
      return maiorId;
    }
  }
  return 0;
}
