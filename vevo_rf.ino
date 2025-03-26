#include <RH_ASK.h>
#include <SPI.h> // Kötelező a RadioHead könyvtárnak
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// TFT kijelző pinjei
#define TFT_CS   10
#define TFT_RST  9
#define TFT_DC   8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// RH_ASK(driverSpeed, rxPin, txPin, pttPin)
// RX pin mostantól: D12
RH_ASK rf_driver(2000, 12, 255, 255); // 2000 bps, RX: D12, TX/PTT nem használt

void setup() {
  Serial.begin(9600);

  // Kijelző inicializálás
  tft.initR(INITR_MINI160x80);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
  tft.setCursor(0, 10);
  tft.println("Varakozas...");
  delay(1000);

  // RF vevő indítása
  if (!rf_driver.init()) {
    tft.fillScreen(ST77XX_RED);
    tft.setCursor(0, 10);
    tft.println("RF hiba!");
    while (1);
  }
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

   tft.fillScreen(ST77XX_BLACK);
   tft.setCursor(0, 20);

  if (rf_driver.recv(buf, &buflen)) {
    buf[buflen] = '\0'; // null-terminátor
    String received = String((char*)buf);

    Serial.print("Fogadva: ");
    Serial.println(received);

    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 20);

    if (received == "1") {
      tft.setTextColor(ST77XX_WHITE);
      tft.println("Mozgas van!");
    } else {
      tft.setTextColor(ST77XX_RED);
      tft.println("Nincs mozg.");
    }
  }

  delay(100);
}
