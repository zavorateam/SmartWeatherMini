#include <ESP8266WiFi.h>
#include <AutoOTA.h> 

#define V "0.1" // Версия прошивки
#define PATH "zavorateam/SmartWeatherMini/main/project.json"
AutoOTA ota(V, PATH);
const int L = LED_BUILTIN; 

// --- НАСТРОЙКИ Wi-Fi ---
const char* S = "XXX"; // SSID вашей сети, название
const char* P = "********"; // Пароль от Wi-Fi

void B(int n) {
  for (int i = 0; i < n * 2; i++) {
    digitalWrite(L, i & 1 ? HIGH : LOW); // Используем побитовую операцию
    delay(200);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(L, OUTPUT);
  digitalWrite(L, HIGH); // Выключаем светодиод

  Serial.print("V: "); // В монитор порта выводим номер версии
  Serial.println(V);

  // 1. ПОДКЛЮЧЕНИЕ
  WiFi.mode(WIFI_STA);
  WiFi.begin(S, P);

  int r = 0;
  while (WiFi.status() != WL_CONNECTED && r++ < 40) {
    delay(500); Serial.print(".");
  }

  // 2. ПРОВЕРКА СТАТУСА и OTA
  if (WiFi.status() == WL_CONNECTED) {
    char buffer[48];
    snprintf(buffer, sizeof(buffer), "\nConnected! IP: %s", WiFi.localIP().toString().c_str());
    Serial.println(buffer);
    
    B(2); // 2 моргания

    if (ota.checkUpdate()) {
      Serial.println("Update finded. Install");
      B(3); // 3 моргания
      if (!ota.updateNow()) {
        Serial.println("Error: Update failed");
      }
    } else {
      Serial.println("No updates");
    }
  } else {
    Serial.println("\nError: Connection fail. Halt");
  }
}

void loop() {
  ota.tick();
}