// KlipperSmart : écran de contrôle Klipper (ESP32-S3, ILI9488 320x480, tactile NS2009)
// Dessin direct à l'écran, avec mise à jour partielle des valeurs pour limiter le clignotement.
// Téléversement du firmware par Wi-Fi (ArduinoOTA) une fois connecté.

#include "LGFX_Config.h"

LGFX lcd;
// Le dessin se fait directement sur l'écran (pas de tampon en PSRAM)
#define spr lcd

#include "klipper_api.h"
#include "ecran.h"
#include <ArduinoOTA.h>

const unsigned long PERIODE_LECTURE = 2000;
const unsigned long PERIODE_WIFI    = 15000;
unsigned long derniereLecture = 0;
unsigned long derniereConnexion = 0;
bool otaActif = false;

// Téléversement Wi-Fi : nom réseau « klipper-smart », mot de passe ci-dessous
void otaDemarrer() {
  if (otaActif || !wifiOk()) return;
  ArduinoOTA.setHostname("klipper-smart");
  ArduinoOTA.setPassword("Kl1pperSmart");
  ArduinoOTA.begin();
  otaActif = true;
}

void setup() {
  Serial.begin(115200);
  delay(500);

  lcd.init();
  reseauCharger();
  ecranCharger();
  ecranOrientationAppliquer();
  wifiConnecter();
  pageAccueil();
}

void loop() {
  static bool etaitAppuye = false;
  uint16_t xb, yb;

  bool appuye = lcd.getTouch(&xb, &yb);
  if (appuye && !etaitAppuye) {
    int x = (int)xb, y = (int)yb;
    // En portrait inversé, le tactile est retourné par rapport au dessin
    if (ecranInverse) { x = 319 - x; y = 479 - y; }
    toucher(x, y);
  }
  etaitAppuye = appuye;

  // Reconnexion Wi-Fi si la liaison est tombée
  if (!wifiOk() && millis() - derniereConnexion >= PERIODE_WIFI) {
    derniereConnexion = millis();
    wifiConnecter();
  }

  otaDemarrer();
  if (otaActif) ArduinoOTA.handle();

  if (millis() - derniereLecture >= PERIODE_LECTURE) {
    derniereLecture = millis();
    moonrakerLire();
    rafraichirEcran();
  }

  delay(20);
}
