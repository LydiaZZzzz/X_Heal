#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1X15.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// BLE UUID
#define SERVICE_UUID        "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

Adafruit_ADS1115 ads;
BLECharacteristic *pCharacteristic;

// 采集配置
#define ANGLE_INPUT_CHANNEL 0
#define SAMPLE_COUNT 10
float voltageBuffer[SAMPLE_COUNT];
int sampleIndex = 0;
unsigned long lastSampleTime = 0;
unsigned long lastUploadTime = 0;
float previousSmoothedVoltage = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(6, 7);

  if (!ads.begin()) {
    Serial.println("❌ ADS1115 初始化失败！");
    while (true);
  }
  ads.setGain(GAIN_TWOTHIRDS);
  Serial.println("✅ ADS1115 初始化成功");

  // BLE 初始化
  BLEDevice::init("DeviceA_Angle");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  BLEDevice::startAdvertising();
  Serial.println("BLE device started, advertising...");
}

void loop() {
  unsigned long now = millis();

  // 每 25ms 采一次电压（更高频）
  if (now - lastSampleTime >= 25 && sampleIndex < SAMPLE_COUNT) {
    lastSampleTime = now;
    int16_t raw = ads.readADC_SingleEnded(ANGLE_INPUT_CHANNEL);
    float voltage = raw * 0.1875 / 1000.0;
    voltageBuffer[sampleIndex++] = voltage;
  }

  // 每 250ms 处理一次数据并发送（每秒4次）
  if (sampleIndex >= SAMPLE_COUNT && now - lastUploadTime >= 250) {
    lastUploadTime = now;

    float sumVoltage = 0;
    for (int i = 0; i < SAMPLE_COUNT; i++) sumVoltage += voltageBuffer[i];
    float avgVoltage = sumVoltage / SAMPLE_COUNT;

    float smoothedVoltage = previousSmoothedVoltage * 0.2 + avgVoltage * 0.8;
    previousSmoothedVoltage = smoothedVoltage;

    float angle = smoothedVoltage * 360.0 / 5.0;
    if (angle < 0) angle = 0;
    if (angle > 360) angle = 360;

    int angleInt = round(angle);
    int adjustedAngle = 180 - angleInt;
    if (adjustedAngle < 0) adjustedAngle = 0;

    // BLE 发送
    char buf[8];
    sprintf(buf, "%d", adjustedAngle);
    pCharacteristic->setValue((uint8_t*)buf, strlen(buf));
    pCharacteristic->notify();

    Serial.print("📤 BLE Adjusted Angle (180 - angle): ");
    Serial.println(adjustedAngle);

    // 清空缓冲区
    sampleIndex = 0;
  }
}