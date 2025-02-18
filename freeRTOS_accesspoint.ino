#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Ticker.h>
#include <Wire.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "MPU9250.h"

MPU9250 mpu;

const char* ssid = "Esp32_Wifi";
const char* password = "12345678";
const IPAddress local_IP(192, 168, 4, 1);
const IPAddress gateway(192, 168, 4, 1);
const IPAddress subnet(255, 255, 255, 0);

unsigned long startTime = 0;
float ax, ay, az, gx, gy, gz, mx, my, mz;

Ticker timer;
WiFiUDP Udp;
const unsigned int localUdpPort = 8888;
IPAddress laptopIP(192, 168, 4, 2); // Địa chỉ IP cố định của máy chủ

bool laptopReady = false;

void setupAccessPoint() {
  Serial.println("Setting up Access Point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_IP, gateway, subnet);


  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void updateSensorData() {
  mpu.update();
  ax = mpu.getAccX(); ay = mpu.getAccY(); az = mpu.getAccZ();
  gx = mpu.getGyroX(); gy = mpu.getGyroY(); gz = mpu.getGyroZ();
  // mx = mpu.getMagX(); my = mpu.getMagY(); mz = mpu.getMagZ();
}

void sendDataToServer(void * parameter) {
  if (!Udp.begin(localUdpPort)) {
    Serial.println("UDP Server setup failed!");
    while (true) {};
  } else {
    Serial.print("UDP Server started at port ");
    Serial.println(localUdpPort);
  }

  while (!laptopReady) {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      char incomingPacket[10];
      int len = Udp.read(incomingPacket, 10);
      if (len > 0) {
        incomingPacket[len] = '\0';
      }
      if (strcmp(incomingPacket, "READY") == 0) {
        laptopReady = true;
        Serial.println("Laptop is ready. Starting data transmission.");
        startTime = millis(); // Bắt đầu đếm thời gian từ khi nhận được tín hiệu từ laptop
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // Kiểm tra tín hiệu từ laptop mỗi 10ms
  }

  char buffer[128]; // Buffer để chứa bản tin gửi đi

  for (;;) {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      char incomingPacket[10];
      int len = Udp.read(incomingPacket, 10);
      if (len > 0) {
        incomingPacket[len] = '\0';
      }
      if (strcmp(incomingPacket, "END") == 0) {
        laptopReady = false;
        Serial.println("Received END signal. Stopping data transmission.");
        break; // Thoát khỏi vòng lặp để ngừng gửi dữ liệu
      }
    }

    unsigned long currentMillis = millis();
    float elapsedTime = (float)(currentMillis - startTime) / 1000.0;
    // snprintf(buffer, sizeof(buffer), "Time = %.2fs; Accel: %.2f, %.2f, %.2f; Gyro: %.2f, %.2f, %.2f; Mag: %.2f, %.2f, %.2f", elapsedTime, ax, ay, az, gx, gy, gz, mx, my, mz);
    // snprintf(buffer, sizeof(buffer), "Time = %.6fs; Accel: %.6f, %.6f, %.6f; Gyro: %.6f, %.6f, %.6f; Mag: %.6f, %.6f, %.6f", elapsedTime, ax, ay, az, gx, gy, gz, mx, my, mz);
    snprintf(buffer, sizeof(buffer), "Time = %.6fs; Accel: %.6f, %.6f, %.6f; Gyro: %.6f, %.6f, %.6f", elapsedTime, ax, ay, az, gx, gy);

    if (Udp.beginPacket(laptopIP, localUdpPort)) {
      Udp.write((uint8_t*)buffer, strlen(buffer)); // Chuyển đổi char* sang uint8_t*
      Udp.endPacket();
    }
    
    vTaskDelay(10 / portTICK_PERIOD_MS); // Gửi dữ liệu mỗi 15ms
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  setupAccessPoint();

  if (!mpu.setup(0x68)) {
    Serial.println("MPU connection failed. Please check your connection.");
    while (true) {
      delay(1000);
    }
  }

  mpu.verbose(false);  
  timer.attach_ms(10, updateSensorData); // Cập nhật dữ liệu mỗi 20ms

  xTaskCreate(sendDataToServer, "Send Data", 10000, NULL, 1, NULL);
}

void loop() {
  if (!laptopReady) {
    // Chờ tín hiệu "READY" từ laptop để bắt đầu gửi dữ liệu
    char incomingPacket[10];
    int packetSize = Udp.parsePacket();
    if (packetSize) {
      int len = Udp.read(incomingPacket, 10);
      if (len > 0) {
        incomingPacket[len] = '\0';
      }
      if (strcmp(incomingPacket, "READY") == 0) {
        laptopReady = true;
        Serial.println("Laptop is ready. Starting data transmission.");
        startTime = millis(); // Bắt đầu đếm thời gian từ khi nhận được tín hiệu từ laptop
        xTaskCreate(sendDataToServer, "Send Data", 10000, NULL, 1, NULL);
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); // Kiểm tra tín hiệu từ laptop mỗi 10ms
  }
}