#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Định nghĩa chân kết nối DHT11
#define DHTPIN D4  
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// Blynk Auth Token
char auth[] = "YourBlynkAuthToken";

// Thông tin WiFi
char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

// Khởi tạo Timer Blynk
BlynkTimer timer;

// Hàm gửi dữ liệu nhiệt độ và độ ẩm lên Blynk
void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Lỗi đọc dữ liệu từ DHT11!");
    return;
  }

  // Gửi dữ liệu nhiệt độ và độ ẩm lên Blynk
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);

  // In dữ liệu ra Serial Monitor
  Serial.print("Nhiệt độ: ");
  Serial.print(t);
  Serial.print("°C, Độ ẩm: ");
  Serial.print(h);
  Serial.println("%");
}

void setup() {
  // Thiết lập kết nối Serial
  Serial.begin(9600);

  // Kết nối với WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Đang kết nối với WiFi...");
  }
  Serial.println("Đã kết nối với WiFi");

  // Kết nối Blynk
  Blynk.begin(auth, ssid, pass);

  // Khởi động cảm biến DHT11
  dht.begin();

  // Thiết lập thời gian lặp gửi dữ liệu mỗi 2 giây
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();  // Chạy Blynk
  timer.run();  // Chạy timer
}
