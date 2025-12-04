#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <EmonLib.h>

// CONFIGURAÇÕES DE REDE
#define WIFI_SSID "PEI"
#define WIFI_PASS "123456789"

// CONFIGURAÇÕES DO FIREBASE
#define API_KEY "AIzaSyC8bqCXthUftcj2c0dzHIW3j_gReOd4rPE"
#define DATABASE_URL "arquitetura-de-computado-54e7e-default-rtdb.firebaseio.com"
#define USER_EMAIL "victorfragoso88@gmail.com"
#define USER_PASSWORD "123456789"

// PINOS DOS SENSORES
#define PINO_TENSAO 35     // ZMPT101B
#define PINO_CORRENTE 36   // ACS712
#define RELAY 26

// CALIBRAÇÃO DOS SENSORES
#define CALIBRACAO_TENSAO 40.9
#define CALIBRACAO_CORRENTE 1.98
#define AJUSTE_FASE 1.7

// OBJETOS DA BIBLIOTECA
EnergyMonitor emon1;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;

// VARIÁVEIS GLOBAIS
int contador = 0;
int idMedicao = 1;

// DECLARAÇÃO DE FUNÇÕES
int recuperarUltimoId(); 


void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Inicializando sistema de medição...");

  pinMode(RELAY, OUTPUT);

  // Configuração do EmonLib
  emon1.voltage(PINO_TENSAO, CALIBRACAO_TENSAO, AJUSTE_FASE);
  emon1.current(PINO_CORRENTE, CALIBRACAO_CORRENTE);

  // Conexão WiFi
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

  Serial.print("Último ID encontrado: ");
  Serial.println(ultimo);
  Serial.print("Próxima medição: ");
  Serial.println(idMedicao);

  digitalWrite(RELAY, HIGH);
}

void loop() {
  int relay_status = digitalRead(RELAY);

  emon1.calcVI(20, 2000);

  // Leitura dos valores RMS e potências
  float tensao = emon1.Vrms;// Volts
  float corrente = emon1.Irms;// Amperes
  float potencia_real = emon1.realPower*-1;// Watts

  if(tensao < 80 && contador > 15){
    tensao = 0;
    corrente = 0;
    potencia_real = 0;
  } else if(tensao > 238 && contador > 15){
    tensao = 220;
  } else if(tensao > 130 && tensao < 160 && contador > 15){
    tensao = 127;
  } else if(tensao > 160 && tensao < 200 & contador > 15){
    tensao = 220;
  }

  // Exibição no monitor serial
  Serial.println(" ");
  Serial.print("Tensão RMS: ");
  Serial.print(tensao, 2);
  Serial.println(" V");

  Serial.print("Corrente RMS: ");
  Serial.print(corrente, 3);
  Serial.println(" A");

  Serial.print("Potência Real: ");
  Serial.print(potencia_real, 2);
  Serial.println(" W");

  Serial.print("Status do Relay: ");
  if(relay_status == HIGH){
    Serial.println("LIGADO");
  } else if(relay_status == LOW){
    Serial.println("DESLIGADO");
  }

  Serial.print("Leitura nº: ");
  Serial.println(contador);

  // Verifica a conexão WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi perdido, tentando reconectar...");
    WiFi.reconnect();
    delay(1000);
  }

  // Envio para o Firebase
  if(contador > 15){
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

      Firebase.RTDB.setInt(&fbdo, "/ultimo_id", idMedicao);

      idMedicao++;

      // Leitura de comando para controle do relay
      if (Firebase.RTDB.getInt(&fbdo, "/controle_relay")) {
        int relay_command = fbdo.intData();
        if (relay_command == 0) {
          digitalWrite(RELAY, HIGH);
          Serial.println("Tomada acionada pelo Firebase (RELAY DESLIGADO)");
        } else if (relay_command == 1) {
          digitalWrite(RELAY, LOW);
          Serial.println("Tomada desativada pelo Firebase (RELAY LIGADO)");
        }
      }
    }
  }
  Serial.println(" ");

  contador++;
  delay(500);
}

int recuperarUltimoId() {
  if (Firebase.RTDB.getInt(&fbdo, "/ultimo_id")) {
    return fbdo.intData();
  }
  return 0;
}

