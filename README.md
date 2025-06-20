# Sistem-IoT-Monitoring-Suhu-Kelembapan-dan-Deteksi-Api-Esp8266
SISTEM IOT UNTUK MONITORING SUHU, KELEMBAPAN, DAN DETEKSI API MENGGUNAKAN ESP8266 DAN PLATFORM TELEGRAM
# 🔥 IoT Monitoring Suhu, Kelembapan, dan Deteksi Api dengan ESP8266 + Telegram

📡 Proyek ini merupakan sistem Internet of Things (IoT) untuk **memantau suhu, kelembapan**, serta **mendeteksi asap/gas yang berpotensi menyebabkan kebakaran**, dan mengirimkan **notifikasi real-time melalui Telegram**. Sistem ini dibangun menggunakan **ESP8266**, sensor **DHT11**, **sensor gas MQ**, dan **Relay** sebagai aktuator.

## 📌 Fitur Utama

- Monitoring suhu dan kelembapan menggunakan sensor DHT11.
- Deteksi asap/gas (kemungkinan api) dengan sensor MQ.
- Otomatisasi aktivasi relay (misalnya untuk pompa air) jika gas terdeteksi.
- Pengiriman notifikasi Telegram secara otomatis saat terdeteksi kondisi berbahaya.
- Permintaan informasi sensor melalui perintah Telegram.
- Tampilan informasi melalui LCD I2C.
- Koneksi otomatis ke WiFi.

## 🧰 Perangkat Keras (Hardware)

- ESP8266 (NodeMCU)
- DHT11 (Sensor suhu & kelembapan)
- MQ2/MQ135 (Sensor gas/asap)
- Relay 1 Channel
- LCD 16x2 I2C
- Breadboard dan jumper kabel
- Koneksi WiFi

## 📡 Perintah Telegram yang Didukung

| Perintah        | Fungsi                                      |
|----------------|---------------------------------------------|
| `Cek suhu`      | Menampilkan suhu saat ini                   |
| `Cek kelembapan`| Menampilkan kelembapan saat ini             |
| `Cek MQ`        | Menampilkan nilai sensor MQ (gas/asap)      |
| `Cek semua`     | Menampilkan suhu, kelembapan, dan MQ Value  |

## 🔧 Instalasi dan Konfigurasi

### 
1. Clone Repository

git clone https://github.com/irvan878/Sistem-IoT-Monitoring-Suhu-Kelembapan-dan-Deteksi-Api-Esp8266

2. Siapkan Library Arduino IDE

Pastikan Anda sudah menginstal library berikut:
- DHT sensor library by Adafruit
- UniversalTelegramBot
- ESP8266WiFi
- LiquidCrystal_I2C
- WiFiClientSecure

3. Ubah Konfigurasi WiFi dan Token Telegram

Pada bagian awal kode, ubah informasi berikut sesuai dengan jaringan dan bot Anda:

const char* ssid = "NamaWiFiAnda";

const char* password = "PasswordWiFi";

const char* botToken = "TOKEN_BOT_ANDA";

const char* chatID = "CHAT_ID_ANDA";

4. Upload ke Board ESP8266
- Pilih board: NodeMCU 1.0 (ESP-12E)
- Hubungkan ke komputer, lalu upload program dari Arduino IDE.

5. Tampilan LCD

LCD akan menampilkan:
- Informasi suhu dan kelembapan secara bergantian.
- Peringatan jika ada gas atau api.
- Identitas kelompok dan institusi saat startup.


Anggota Kelompok 4
- Irvan Nurmutakim
- M. Lukmanul Khakim
- Wildan Nur Yusufi
📍 Program Studi Teknologi Informasi – Fakultas Sains dan Teknologi – UIN Walisongo Semarang
