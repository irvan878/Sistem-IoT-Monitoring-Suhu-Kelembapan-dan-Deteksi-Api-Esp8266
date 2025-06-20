//                                                         //
//        SISTEM IOT UNTUK MONITORING SUHU, KELEMBABAN     //
//            SUHU, KELEMBAPAN, DAN DETEKSI API            //
//        MENGGUNAKAN ESP8266 DAN PLATFORM TELEGRAM        //
//    TEKNOLOGI INFORMASI - FAKULTAS SAINS DAN TEKNOLOGI   //
//                  UIN WALLISONGO SEMARANG                //
//
//
// KELOMPOK: 4                        //
// ANGGOTA:                           //
//         1. Irvan Nurmutakim        //
//         2. M. Lukmanul Khakim      //
//         3. Wildan Nur Yusufi       // 
//
#include <UniversalTelegramBot.h>  // Library untuk menghubungkan ESP8266 dengan Telegram bot
#include <DHT.h>  // Library untuk sensor DHT (suhu dan kelembapan)
#include <LiquidCrystal_I2C.h>  // Library untuk mengontrol LCD via I2C
#include <ESP8266WiFi.h>  // Library untuk koneksi WiFi menggunakan ESP8266

#define DHTPIN D5  // Pin untuk sensor DHT (D5 pada ESP8266)
#define DHTTYPE DHT11  // Jenis sensor DHT yang digunakan (DHT11)

#define MQPIN A0  // Pin analog untuk sensor MQ (untuk deteksi gas)
#define RELAY1_PIN D3  // Pin digital untuk mengontrol Relay 1

DHT dht(DHTPIN, DHTTYPE);  // Membuat objek untuk sensor DHT
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Alamat I2C dan ukuran LCD 16x2

// Wi-Fi credentials
const char* ssid = "Kelompok4";
const char* password = "walisongo";

// Telegram Bot Token (dari BotFather)
const char* botToken = "7594165892:AAGj6NJrNQTVu3ptu-d5cp1bbUAr-q8QTTk";
// Chat ID untuk mengirim pesan (dapat diperoleh melalui @userinfobot di Telegram)
const char* chatID = "931897538";

// WiFiClientSecure untuk koneksi HTTPS
WiFiClientSecure client;  // Klien untuk komunikasi aman
UniversalTelegramBot bot(botToken, client);  // Membuat objek bot Telegram

void setup() {
  Serial.begin(115200);  // Mulai komunikasi serial untuk debugging
  dht.begin();  // Inisialisasi sensor DHT

  // Inisialisasi pin relay
  pinMode(RELAY1_PIN, OUTPUT);  

  // Memastikan relay dalam keadaan mati (HIGH = matikan) saat startup
  digitalWrite(RELAY1_PIN, HIGH);  // Matikan relay 1 pada awalnya

  // Inisialisasi LCD
  lcd.init();       // Menggunakan init() untuk inisialisasi LCD
  lcd.backlight();  // Mengaktifkan lampu latar LCD
  
  // Menampilkan pesan pembuka pada LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MONITORING SUHU,");
  lcd.setCursor(0, 1);
  lcd.print("KELEMBAPAN & API");
  delay(3000);  

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("KELOMPOK 4");
  lcd.setCursor(0, 1);
  lcd.print("IOT SK");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IRVAN, WILDAN");
  lcd.setCursor(0, 1);
  lcd.print("dan LUKMAN");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Prodi TI");
  lcd.setCursor(0, 1);
  lcd.print("UIN Walisongo");
  delay(2000);

  bool wifiConnected = false;
  while (!wifiConnected) {
    WiFi.begin(ssid, password);
    Serial.println("Menghubungkan ke Wi-Fi...");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Menghubungkan");
    lcd.setCursor(0, 1);
    lcd.print("ke Wifi.........");

    unsigned long startAttemptTime = millis();

    // Coba koneksi maksimal 10 detik
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(1000);
      Serial.println("Mencoba koneksi...");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Terhubung ke WiFi");
      client.setInsecure();  // Disable SSL verification
      wifiConnected = true;
    } else {
      Serial.println("Gagal terhubung ke WiFi");

      // Tampilkan "Pakai WiFi Berikut"
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pakai User&Pass");
      lcd.setCursor(0, 1);
      lcd.print("WIFI Berikut....");
      delay(2000);

      // Tampilkan "Kelompok 4" dan "Walisongo"
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("User : Kelompok4");
      lcd.setCursor(0, 1);
      lcd.print("Pass : walisongo");
      delay(8000);

      // Coba lagi koneksi WiFi (loop while akan mengulang)
    }
  }

  client.setInsecure();  // Nonaktifkan verifikasi SSL (untuk koneksi cepat)
}

void loop() {
  // Membaca suhu dalam Celcius
  float suhu = dht.readTemperature();
  // Membaca kelembapan
  float kelembapan = dht.readHumidity();
  
  // Membaca nilai dari sensor MQ
  int mqValue = analogRead(MQPIN);  // Baca nilai analog dari sensor MQ
  
  // Cek jika pembacaan gagal dari sensor DHT
  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Gagal membaca sensor DHT");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor error");
    return;  // Jika gagal, hentikan fungsi loop
  }

  // Menampilkan suhu, kelembapan, dan nilai MQ ke serial monitor untuk debugging
  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.print(" *C ");
  Serial.print("Kelembapan: ");
  Serial.print(kelembapan);
  Serial.print(" % ");
  Serial.print("MQ Value: ");
  Serial.println(mqValue);  // Menampilkan nilai sensor MQ

  // Menampilkan suhu dan kelembapan di LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(suhu);
  lcd.print(" *C");
  
  lcd.setCursor(0, 1);
  lcd.print("Kelembapan: ");
  lcd.print(kelembapan);
  lcd.print(" %");

  // Kontrol relay berdasarkan nilai sensor MQ
  if (mqValue > 600) {  // Jika nilai sensor MQ lebih dari 600, deteksi api/gas berbahaya
    digitalWrite(RELAY1_PIN, LOW);  // Aktifkan relay (pompa air)
    Serial.println("Relay 1 aktif - Deteksi Api!");
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TERDETEKSI API!!!");
    lcd.setCursor(0, 1);
    lcd.print("Pompa Air Aktif");

    String message = "Peringatan: API terdeteksi! Pompa air aktif.";
    bot.sendMessage(chatID, message, "");

    delay(5000);  // Tahan tampilan selama 5 detik
  } 
  else if (mqValue > 400) {  // Nilai MQ antara 401 - 600 → indikasi suhu/gas ringan
    digitalWrite(RELAY1_PIN, HIGH);  // Jangan aktifkan relay
    Serial.println("Deteksi gas ringan - relay tidak aktif");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gas Terdeteksi!!");
    lcd.setCursor(0, 1);
    lcd.print("Pantau kondisi");

    String message = "Peringatan: Terdeteksi GAS Berbahaya!!!";
    bot.sendMessage(chatID, message, "");

    delay(5000);
  } 
  else {
    // MQ normal (≤400) → relay tetap mati
    digitalWrite(RELAY1_PIN, HIGH);  // Matikan relay
    Serial.println("MQ normal - Relay 1 mati");
  }

  // Cek jika ada pesan baru dari Telegram
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1); // Mulai dari pesan terbaru
  while (numNewMessages) {
    Serial.println("Membaca pesan baru...");
    
    // Iterasi melalui pesan baru
    for (int i = 0; i < numNewMessages; i++) {
      String text = bot.messages[i].text;  // Pesan yang diterima
      String fromName = bot.messages[i].from_name;  // Nama pengirim

      // Jika pesan berisi "cek suhu", kirimkan suhu
      if (text == "Cek suhu") {
        String reply = "Suhu saat ini: " + String(suhu) + " *C";
        bot.sendMessage(chatID, reply, "");
      }
      // Jika pesan berisi "cek kelembapan", kirimkan kelembapan
      else if (text == "Cek kelembapan") {
        String reply = "Kelembapan saat ini: " + String(kelembapan) + " %";
        bot.sendMessage(chatID, reply, "");
      }
      // Jika pesan berisi "cek MQ", kirimkan nilai sensor MQ
      else if (text == "Cek MQ") {
        String reply = "Nilai sensor MQ: " + String(mqValue);
        bot.sendMessage(chatID, reply, "");
      }
      // Jika pesan berisi "cek semua", kirimkan suhu, kelembapan, dan nilai sensor MQ
      else if (text == "Cek semua") {
        String reply = "Suhu: " + String(suhu) + " *C\nKelembapan: " + String(kelembapan) + " %\nMQ Value: " + String(mqValue);
        bot.sendMessage(chatID, reply, "");
      }
    }
    numNewMessages = bot.getUpdates(bot.last_message_received + 1); // Periksa pembaruan pesan lagi
  }

  // Delay agar pembacaan tidak terlalu cepat
  delay(2000);  // Update setiap 2 detik
  Serial.println("Menerima pesan...");
}