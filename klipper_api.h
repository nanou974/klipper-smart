#pragma once
// Wi-Fi et lecture Moonraker (HTTP, port 80).
// Le Wi-Fi est celui déjà enregistré par SmartTFT (espace "wifi", clés "ssid" et "pass").
// L'adresse de l'imprimante est enregistrée dans l'espace "klipper", clé "ip".
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Preferences.h>

char wifiSsid[33] = "";
char wifiPass[65] = "";
char imprimanteIp[16] = "";

// Dernière lecture de l'imprimante
struct Etat {
  bool ok = false;          // dernière lecture réussie
  float extT = 0, extCible = 0;
  float bedT = 0, bedCible = 0;
  float x = 0, y = 0, z = 0;
  char axes[4] = "";        // axes référencés, ex. "xyz"
  char etat[16] = "";       // print_stats.state : standby, printing, paused, complete, error...
  char fichier[48] = "";    // print_stats.filename
  float progres = 0;        // 0 à 100
};
Etat etat;

// Historique des températures pour le graphique (un point toutes les 2 s)
const int NB_HISTO = 30;
float histoBuse[NB_HISTO];
float histoPlateau[NB_HISTO];
int histoPos = 0;
int histoNb = 0;

void reseauCharger() {
  Preferences p;
  p.begin("wifi", true);
  String s = p.getString("ssid", "");
  String k = p.getString("pass", "");
  p.end();
  snprintf(wifiSsid, sizeof wifiSsid, "%s", s.c_str());
  snprintf(wifiPass, sizeof wifiPass, "%s", k.c_str());

  Preferences q;
  q.begin("klipper", true);
  String ip = q.getString("ip", "");
  q.end();
  snprintf(imprimanteIp, sizeof imprimanteIp, "%s", ip.c_str());
}

void reseauSauverIp(const char* ip) {
  snprintf(imprimanteIp, sizeof imprimanteIp, "%s", ip);
  Preferences q;
  q.begin("klipper", false);
  q.putString("ip", imprimanteIp);
  q.end();
}

void wifiConnecter() {
  if (wifiSsid[0] == 0) return;
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid, wifiPass);
}

bool wifiOk() { return WiFi.status() == WL_CONNECTED; }

// Envoie un script G-code à Moonraker (POST /printer/gcode/script). Les retours à la ligne
// du script sont transformés en \n dans le JSON. Renvoie true si Moonraker accepte.
bool gcodeEnvoyer(const char* script) {
  if (!wifiOk() || imprimanteIp[0] == 0) return false;
  char corps[320];
  int n = snprintf(corps, sizeof corps, "{\"script\":\"");
  for (const char* p = script; *p && n < (int)sizeof corps - 8; p++) {
    if (*p == '\n') {
      corps[n++] = '\\';
      corps[n++] = 'n';
    } else {
      corps[n++] = *p;
    }
  }
  corps[n] = 0;
  strncat(corps, "\"}", sizeof corps - strlen(corps) - 1);

  char url[64];
  snprintf(url, sizeof url, "http://%s/printer/gcode/script", imprimanteIp);
  HTTPClient http;
  http.setTimeout(1500);
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  int code = http.POST(String(corps));
  http.end();
  return code == 200;
}

// ---------- Lecture du JSON Moonraker ----------
// Moonraker écrit "clé": valeur avec des espaces possibles autour des deux-points.
// Ces fonctions cherchent une clé entre deux pointeurs (début et fin de l'objet).

// Renvoie un pointeur sur la valeur qui suit "cle" dans [debut, fin), ou nullptr
const char* cherche(const char* debut, const char* fin, const char* cle) {
  char motif[40];
  snprintf(motif, sizeof motif, "\"%s\"", cle);
  int longueur = strlen(motif);
  for (const char* p = debut; p + longueur <= fin; p++) {
    if (strncmp(p, motif, longueur) != 0) continue;
    const char* q = p + longueur;
    while (q < fin && (*q == ' ' || *q == '\t')) q++;
    if (q >= fin || *q != ':') continue;
    q++;
    while (q < fin && (*q == ' ' || *q == '\t')) q++;
    return q;
  }
  return nullptr;
}

// Pointeur sur l'accolade fermante correspondant à l'accolade ouvrante "debut"
const char* finObjet(const char* debut) {
  int profondeur = 0;
  for (const char* p = debut; *p; p++) {
    if (*p == '{') profondeur++;
    else if (*p == '}') {
      profondeur--;
      if (profondeur == 0) return p + 1;
    }
  }
  return debut + strlen(debut);
}

// Nombre de la clé "cle" dans [debut, fin)
float nombreDans(const char* debut, const char* fin, const char* cle) {
  const char* v = cherche(debut, fin, cle);
  return v ? (float)atof(v) : 0.0f;
}

// Texte de la clé "cle" dans [debut, fin), recopié jusqu'au guillemet fermant
bool texteDans(const char* debut, const char* fin, const char* cle, char* sortie, int taille) {
  sortie[0] = 0;
  const char* v = cherche(debut, fin, cle);
  if (!v || *v != '"') return false;
  v++;
  int i = 0;
  while (v < fin && *v && *v != '"' && i < taille - 1) sortie[i++] = *v++;
  sortie[i] = 0;
  return true;
}

// Sous-objet "cle" : renvoie son début et sa fin, ou false si absent
bool objetDans(const char* debut, const char* fin, const char* cle, const char** o, const char** f) {
  const char* v = nullptr;
  for (const char* p = debut; (v = cherche(p, fin, cle)) != nullptr; p = v + 1) {
    if (*v == '{') break;
  }
  if (!v || *v != '{') return false;
  *o = v;
  *f = finObjet(v);
  return true;
}

// ---------- Wi-Fi : sauvegarde et recherche ----------

// Enregistre le réseau choisi (espace "wifi", clés "ssid" et "pass")
void wifiSauver(const char* ssid, const char* pass) {
  snprintf(wifiSsid, sizeof wifiSsid, "%s", ssid);
  snprintf(wifiPass, sizeof wifiPass, "%s", pass);
  Preferences p;
  p.begin("wifi", false);
  p.putString("ssid", wifiSsid);
  p.putString("pass", wifiPass);
  p.end();
}

// Cherche les réseaux visibles. Remplit noms (33 car. max) et signal (dBm).
// Renvoie le nombre de réseaux trouvés, sans doublons.
int wifiRechercher(char noms[][33], int signal[], int max) {
  if (WiFi.getMode() == WIFI_OFF) WiFi.mode(WIFI_STA);
  int n = WiFi.scanNetworks();
  int k = 0;
  for (int i = 0; i < n && k < max; i++) {
    String s = WiFi.SSID(i);
    if (s.length() == 0) continue;
    bool deja = false;
    for (int j = 0; j < k; j++) {
      if (strcmp(noms[j], s.c_str()) == 0) deja = true;
    }
    if (deja) continue;
    snprintf(noms[k], 33, "%s", s.c_str());
    signal[k] = WiFi.RSSI(i);
    k++;
  }
  WiFi.scanDelete();
  return k;
}

// ---------- Fichiers de l'imprimante et impression ----------

// Liste les fichiers .gcode : les derniers imprimés en premier (historique Moonraker),
// puis les autres, les plus récemment modifiés en premier. Renvoie leur nombre.
int fichiersLister(char noms[][48], int max) {
  if (!wifiOk() || imprimanteIp[0] == 0) return 0;

  // 1. Fichiers présents sur l'imprimante, avec leur date de modification
  static char tous[64][48];
  static double dates[64];
  int nbTous = 0;
  {
    char url[96];
    snprintf(url, sizeof url, "http://%s/server/files/list", imprimanteIp);
    HTTPClient http;
    http.setTimeout(3000);
    http.begin(url);
    int code = http.GET();
    String corps = (code == 200) ? http.getString() : String();
    http.end();
    if (corps.length() == 0) return 0;

    const char* debut = corps.c_str();
    const char* fin = debut + corps.length();
    const char* p = debut;
    while (nbTous < 64) {
      const char* v = cherche(p, fin, "path");
      if (!v) break;
      if (*v == '"') {
        v++;
        int i = 0;
        while (v < fin && *v && *v != '"' && i < 47) tous[nbTous][i++] = *v++;
        tous[nbTous][i] = 0;
        // Date de modification : dans le même objet, avant le « path » suivant
        const char* suivant = cherche(v, fin, "path");
        const char* m = cherche(v, suivant ? suivant : fin, "modified");
        dates[nbTous] = m ? atof(m) : 0.0;
        if (i > 0 && strstr(tous[nbTous], ".gcode") != nullptr) nbTous++;
      }
      p = v;
    }
  }

  // Tri par date de modification décroissante
  for (int i = 1; i < nbTous; i++) {
    for (int j = i; j > 0 && dates[j] > dates[j - 1]; j--) {
      char t[48];
      strcpy(t, tous[j]);
      strcpy(tous[j], tous[j - 1]);
      strcpy(tous[j - 1], t);
      double d = dates[j];
      dates[j] = dates[j - 1];
      dates[j - 1] = d;
    }
  }

  int k = 0;

  // 2. D'abord les fichiers imprimés récemment (historique, le plus récent en premier)
  {
    char url[120];
    snprintf(url, sizeof url, "http://%s/server/history/list?limit=50&order=desc", imprimanteIp);
    HTTPClient http;
    http.setTimeout(3000);
    http.begin(url);
    int code = http.GET();
    String corps = (code == 200) ? http.getString() : String();
    http.end();

    const char* p = corps.c_str();
    const char* fin = p + corps.length();
    while (corps.length() > 0 && k < max) {
      const char* v = cherche(p, fin, "filename");
      if (!v) break;
      if (*v == '"') {
        v++;
        char nom[48];
        int i = 0;
        while (v < fin && *v && *v != '"' && i < 47) nom[i++] = *v++;
        nom[i] = 0;
        bool present = false;
        for (int j = 0; j < nbTous; j++) {
          if (strcmp(tous[j], nom) == 0) { present = true; break; }
        }
        bool deja = false;
        for (int j = 0; j < k; j++) {
          if (strcmp(noms[j], nom) == 0) { deja = true; break; }
        }
        if (present && !deja) {
          snprintf(noms[k], 48, "%s", nom);
          k++;
        }
      }
      p = v;
    }
  }

  // 3. Ensuite les autres fichiers, par date de modification
  for (int j = 0; j < nbTous && k < max; j++) {
    bool deja = false;
    for (int m = 0; m < k; m++) {
      if (strcmp(noms[m], tous[j]) == 0) { deja = true; break; }
    }
    if (!deja) {
      snprintf(noms[k], 48, "%s", tous[j]);
      k++;
    }
  }
  return k;
}

// Lance l'impression d'un fichier (POST /printer/print/start?filename=...)
bool imprimerFichier(const char* nom) {
  if (!wifiOk() || imprimanteIp[0] == 0) return false;
  char enc[160];
  int n = 0;
  for (const char* p = nom; *p && n < (int)sizeof enc - 4; p++) {
    if (*p == ' ') {
      enc[n++] = '%';
      enc[n++] = '2';
      enc[n++] = '0';
    } else {
      enc[n++] = *p;
    }
  }
  enc[n] = 0;
  char url[240];
  snprintf(url, sizeof url, "http://%s/printer/print/start?filename=%s", imprimanteIp, enc);
  HTTPClient http;
  http.setTimeout(3000);
  http.begin(url);
  int code = http.POST("");
  http.end();
  return code == 200;
}

// Décale le Z offset de cent/100 mm (cent peut être négatif), avec déplacement immédiat
bool zOffsetAjuster(int cent) {
  char s[64];
  int a = cent < 0 ? -cent : cent;
  snprintf(s, sizeof s, "SET_GCODE_OFFSET Z_ADJUST=%c%d.%02d MOVE=1", cent < 0 ? '-' : '+',
           a / 100, a % 100);
  return gcodeEnvoyer(s);
}

// Interroge Moonraker et met à jour etat. Renvoie true si la lecture a réussi.
bool moonrakerLire() {
  if (!wifiOk() || imprimanteIp[0] == 0) {
    etat.ok = false;
    return false;
  }
  char url[96];
  snprintf(url, sizeof url,
           "http://%s/printer/objects/query?extruder&heater_bed&toolhead&print_stats&display_status",
           imprimanteIp);
  HTTPClient http;
  http.setTimeout(1500);
  http.begin(url);
  int code = http.GET();
  String corps = (code == 200) ? http.getString() : String();
  http.end();
  if (corps.length() == 0) {
    etat.ok = false;
    return false;
  }

  const char* debut = corps.c_str();
  const char* fin = debut + corps.length();
  const char *o, *f;

  if (objetDans(debut, fin, "extruder", &o, &f)) {
    etat.extT = nombreDans(o, f, "temperature");
    etat.extCible = nombreDans(o, f, "target");
  }
  if (objetDans(debut, fin, "heater_bed", &o, &f)) {
    etat.bedT = nombreDans(o, f, "temperature");
    etat.bedCible = nombreDans(o, f, "target");
  }
  if (objetDans(debut, fin, "toolhead", &o, &f)) {
    const char* p = cherche(o, f, "position");
    if (p && *p == '[') {
      p++;
      etat.x = atof(p);
      const char* v = strchr(p, ',');
      if (v) {
        etat.y = atof(v + 1);
        v = strchr(v + 1, ',');
        if (v) etat.z = atof(v + 1);
      }
    }
    texteDans(o, f, "homed_axes", etat.axes, sizeof etat.axes);
  }
  if (objetDans(debut, fin, "print_stats", &o, &f)) {
    texteDans(o, f, "state", etat.etat, sizeof etat.etat);
    texteDans(o, f, "filename", etat.fichier, sizeof etat.fichier);
  }
  if (objetDans(debut, fin, "display_status", &o, &f)) {
    etat.progres = nombreDans(o, f, "progress") * 100.0f;
  }

  histoBuse[histoPos] = etat.extT;
  histoPlateau[histoPos] = etat.bedT;
  histoPos = (histoPos + 1) % NB_HISTO;
  if (histoNb < NB_HISTO) histoNb++;

  etat.ok = true;
  return true;
}
