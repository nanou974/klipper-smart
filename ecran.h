#pragma once
// Écrans : accueil KlipperScreen en direct, Homing, Températures (clavier de cible),
// Actions (extrusion, ventilateur), Print (fichiers, Z offset, pause, reprise, annulation),
// Réglages (Wi-Fi avec recherche et mot de passe, adresse IP).
// Tout est dessiné dans le tampon spr, puis envoyé à l'écran d'un seul coup (pas de clignotement).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum Page { PAGE_MENU, PAGE_CLAVIER, PAGE_HOMING, PAGE_TEMP, PAGE_ACTIONS, PAGE_PRINT,
            PAGE_CONFIG, PAGE_WIFI, PAGE_MDP, PAGE_FICHIERS, PAGE_CIBLE };
Page page = PAGE_MENU;

struct Bouton { int x, y, w, h; };

bool dans(const Bouton& b, int x, int y) {
  return x >= b.x && x < b.x + b.w && y >= b.y && y < b.y + b.h;
}

uint16_t c(uint8_t r, uint8_t g, uint8_t b) { return lcd.color565(r, g, b); }

uint16_t COUL_FOND() { return c(14, 16, 20); }
uint16_t COUL_BARRE() { return c(27, 31, 38); }
uint16_t COUL_TUILE() { return c(27, 31, 38); }
uint16_t COUL_ROUGE() { return c(240, 98, 146); }
uint16_t COUL_GRIS() { return c(170, 176, 187); }
uint16_t COUL_BLEU() { return c(47, 111, 214); }
uint16_t COUL_SARCELLE() { return c(26, 158, 143); }
uint16_t COUL_NEUTRE() { return c(58, 63, 72); }
uint16_t COUL_VERT() { return c(46, 139, 87); }

// Envoie le tampon à l'écran
// Dessin direct à l'écran (pas de tampon en PSRAM) : rien à envoyer
void montrer() {}

// Libellés en ASCII : la police de l'écran ne gère pas les accents
const char* NOM_IMPRIMANTE = "Sidewinder X4";

const char* TUILES[5] = {"Homing", "Temperature", "Actions", "Configuration", "Print"};
const uint8_t COULEURS_TUILES[5][3] = {
  {224, 122, 31}, {194, 24, 91}, {26, 158, 143}, {156, 204, 60}, {224, 122, 31}
};

// Prototypes des pages (définies plus bas)
void pageMenu();
void pageConfig();
void pageTemp();
void pageHoming();
void pageActions();
void pagePrint();
void pageWifi();
void pageMdp();
void pageFichiers();
void pageCible(bool buse);
void pageClavier();
void dessinePrint();
void dessinePrintLignes();
void rafraichirMenu();

const Bouton B_RETOUR     = {8, 420, 304, 52};
const Bouton B_OK         = {8, 370, 304, 44};
const Bouton B_ANNULER    = {8, 424, 304, 44};

Bouton caseTuile(int i) {
  if (i == 4) return {88, 414, 144, 48};
  return {8 + (i % 2) * 160, 302 + (i / 2) * 56, 144, 48};
}

// Clavier numérique (IP et températures)
Bouton caseTouche(int r, int col) { return {8 + col * 104, 120 + r * 62, 96, 54}; }
const char* TOUCHES_IP[4][3] = {{"1", "2", "3"}, {"4", "5", "6"}, {"7", "8", "9"}, {".", "0", "<"}};
const char* TOUCHES_CIBLE[4][3] = {{"1", "2", "3"}, {"4", "5", "6"}, {"7", "8", "9"}, {"<", "0", "OK"}};
char saisie[16] = "";

// Homing et déplacements
const Bouton B_HOME_X    = {8, 52, 96, 50};
const Bouton B_HOME_Y    = {112, 52, 96, 50};
const Bouton B_HOME_Z    = {216, 52, 96, 50};
const Bouton B_HOME_TOUT = {8, 108, 304, 50};
const Bouton B_DIST[4]   = {{8, 170, 72, 40}, {86, 170, 72, 40}, {164, 170, 72, 40}, {242, 170, 72, 40}};
const char* DIST_TXT[4]  = {"0.1", "1", "10", "50"};
int distanceIdx = 1;
const Bouton B_Z_PLUS    = {8, 226, 96, 50};
const Bouton B_Y_PLUS    = {112, 226, 96, 50};
const Bouton B_X_MOINS   = {8, 282, 96, 50};
const Bouton B_X_PLUS    = {216, 282, 96, 50};
const Bouton B_Z_MOINS   = {8, 338, 96, 50};
const Bouton B_Y_MOINS   = {112, 338, 96, 50};

// Températures : boutons -10, -1, +1, +10 puis Eteindre
const Bouton B_TEMP_EXT[4] = {{8, 130, 72, 50}, {86, 130, 72, 50}, {164, 130, 72, 50}, {242, 130, 72, 50}};
const Bouton B_TEMP_BED[4] = {{8, 300, 72, 50}, {86, 300, 72, 50}, {164, 300, 72, 50}, {242, 300, 72, 50}};
const Bouton B_EXT_OFF     = {8, 186, 304, 40};
const Bouton B_BED_OFF     = {8, 356, 304, 40};
const int DELTAS[4] = {-10, -1, 1, 10};
int tgtExt = 0, tgtBed = 0;

// Clavier de cible : buse ou plateau
char cibleTxt[8] = "";
bool cibleBuse = true;

// Actions : distance d'extrusion, extruder, retracter, ventilateur
const Bouton B_EXT_DIST[3] = {{8, 52, 96, 50}, {112, 52, 96, 50}, {216, 52, 96, 50}};
const char* EXT_TXT[3]     = {"1", "5", "10"};
const char* EXT_LIBELLE[3] = {"1 mm", "5 mm", "10 mm"};
int extIdx = 0;
const Bouton B_EXTRUDER    = {8, 116, 144, 56};
const Bouton B_RETRACTER   = {168, 116, 144, 56};
const Bouton B_FAN[5]      = {{8, 206, 56, 50}, {70, 206, 56, 50}, {132, 206, 56, 50},
                              {194, 206, 56, 50}, {256, 206, 56, 50}};
const int FAN_PCT[5]       = {0, 25, 50, 75, 100};
const char* FAN_LIBELLE[5] = {"Arret", "25%", "50%", "75%", "100%"};
int fanIdx = 0;

// Print : progression, Z offset, pause, reprise, annulation (avec confirmation)
const Bouton B_ZSTEP[4]  = {{8, 226, 72, 36}, {86, 226, 72, 36}, {164, 226, 72, 36}, {242, 226, 72, 36}};
const char* ZSTEP_TXT[4] = {"0.01", "0.05", "0.1", "0.5"};
const int ZSTEP_CENT[4]  = {1, 5, 10, 50};
int zIdx = 1;
const Bouton B_ZMOINS    = {8, 270, 144, 40};
const Bouton B_ZPLUS     = {168, 270, 144, 40};
const Bouton B_PAUSE     = {8, 320, 96, 50};
const Bouton B_REPRENDRE = {112, 320, 96, 50};
const Bouton B_STOP      = {216, 320, 96, 50};
const Bouton B_CHOISIR   = {8, 226, 304, 44};
bool annulerArme = false;

// Fichiers de l'imprimante
char fichiers[24][48];
int nbFich = 0;
int fichOff = 0;
int fichSel = -1;
const Bouton B_FICH_PREC   = {8, 364, 150, 36};
const Bouton B_FICH_SUIV   = {162, 364, 150, 36};
const Bouton B_LANCER      = {8, 410, 150, 52};
const Bouton B_FICH_RETOUR = {162, 410, 150, 52};
Bouton ficheRange(int k) { return {8, 52 + k * 42, 304, 36}; }

// Wi-Fi : liste des réseaux, clavier du mot de passe
char wNoms[8][33];
int wSignal[8];
int wNb = 0;
int wSel = -1;
const Bouton B_WIFI_SCAN = {8, 366, 304, 40};
Bouton wifiRang(int i) { return {8, 56 + i * 46, 304, 40}; }
char mdp[64] = "";
bool maj = false;
const char* CLAVIER_MDP[5] = {"abcdefghij", "klmnopqrst", "uvwxyz0123", "456789-_.@", "!#$%&*+=/?"};
const Bouton B_MAJ         = {8, 300, 72, 40};
const Bouton B_ESPACE      = {86, 300, 148, 40};
const Bouton B_EFFACER     = {240, 300, 72, 40};
const Bouton B_MDP_OK      = {8, 350, 150, 50};
const Bouton B_MDP_ANNULER = {162, 350, 150, 50};
Bouton toucheMdp(int r, int col) { return {8 + col * 31, 100 + r * 38, 30, 34}; }

// Configuration
const Bouton B_WIFI   = {8, 240, 304, 50};
const Bouton B_CHG_IP = {8, 300, 304, 50};
const Bouton B_ORIENT = {8, 360, 304, 50};
bool ecranInverse = false;
void ecranCharger() { Preferences p; p.begin("ecran", true); ecranInverse = p.getBool("inverse", false); p.end(); }
void ecranOrientationAppliquer() { lcd.startWrite(); lcd.writeCommand(0x36); lcd.writeData(ecranInverse ? 0xC8 : 0x48); lcd.endWrite(); }
void ecranOrientationBasculer() { ecranInverse = !ecranInverse; Preferences p; p.begin("ecran", false); p.putBool("inverse", ecranInverse); p.end(); ecranOrientationAppliquer(); pageConfig(); }

// Convertit un nombre en texte à une décimale, sans %f
void fmt1(float v, char* out, int taille) {
  int d = (int)(v * 10.0f + (v >= 0 ? 0.5f : -0.5f));
  snprintf(out, taille, "%d.%d", d / 10, abs(d % 10));
}

int borne(int v, int mini, int maxi) {
  if (v < mini) return mini;
  if (v > maxi) return maxi;
  return v;
}

bool imprimeEnCours() { return strcmp(etat.etat, "printing") == 0; }
bool enPause()        { return strcmp(etat.etat, "paused") == 0; }
bool imprimeOuPause() { return imprimeEnCours() || enPause(); }

// Petit texte centré dans un bouton, réduit si le libellé est trop long
void texteBouton(const Bouton& b, const char* texte, int hauteurUtile) {
  spr.setTextColor(TFT_WHITE);
  spr.setTextSize(1.2f);
  if (spr.textWidth(texte) > b.w - 8) spr.setTextSize(1.0f);
  spr.setCursor(b.x + (b.w - spr.textWidth(texte)) / 2, b.y + hauteurUtile / 2 - 6);
  spr.print(texte);
}

void dessineBouton(const Bouton& b, const char* texte, uint16_t couleur) {
  spr.fillRoundRect(b.x, b.y, b.w, b.h, 10, couleur);
  texteBouton(b, texte, b.h);
}

// Tuile du tableau de bord : fond sombre, liseré coloré en bas
void dessineTuile(int i) {
  Bouton b = caseTuile(i);
  spr.fillRoundRect(b.x, b.y, b.w, b.h, 10, COUL_TUILE());
  spr.fillRoundRect(b.x, b.y + b.h - 4, b.w, 4, 2,
                    c(COULEURS_TUILES[i][0], COULEURS_TUILES[i][1], COULEURS_TUILES[i][2]));
  texteBouton(b, TUILES[i], b.h - 4);
}

void dessineEntete(const char* titre, const char* sousTitre) {
  spr.fillRect(0, 0, 320, 44, COUL_BARRE());
  spr.setTextColor(TFT_WHITE, COUL_BARRE());
  spr.setTextSize(1.6f);
  spr.setCursor(8, 6);
  spr.print(titre);
  spr.setTextSize(1.1f);
  spr.setCursor(8, 28);
  spr.print(sousTitre);
}

// Message dans la ligne sous le titre
void dessineMessage(const char* message, uint16_t couleur) {
  spr.fillRect(0, 28, 320, 16, COUL_BARRE());
  spr.setTextColor(couleur, COUL_BARRE());
  spr.setTextSize(1.1f);
  spr.setCursor(8, 28);
  spr.print(message);
  montrer();
}

// Retour visuel à l'appui : liseré blanc, puis l'action
bool appui(const Bouton& b, int x, int y) {
  if (!dans(b, x, y)) return false;
  montrer();
  delay(120);
  return true;
}

// Envoie un G-code et signale l'échec à l'écran
bool envoyerGcode(const char* script) {
  bool ok = gcodeEnvoyer(script);
  if (!ok) dessineMessage("Commande non envoyee", COUL_ROUGE());
  return ok;
}

// Vrai si une action de mouvement ou de chauffe est interdite (impression en cours)
bool bloqueParImpression() {
  if (imprimeEnCours()) {
    dessineMessage("Bloque : impression en cours", COUL_ROUGE());
    return true;
  }
  return false;
}

// Une ligne « nom  valeur » avec séparateur
void ligneValeur(int y, const char* nom, const char* valeur) {
  spr.fillRect(12, y, 296, 24, COUL_FOND());
  spr.setTextColor(TFT_WHITE, COUL_FOND());
  spr.setTextSize(1.2f);
  spr.setCursor(12, y + 4);
  spr.print(nom);
  spr.setCursor(308 - spr.textWidth(valeur), y + 4);
  spr.print(valeur);
  spr.drawFastHLine(12, y + 26, 296, c(45, 51, 61));
}

// ---------- Barre de statut et accueil ----------

void dessineBarreStatut() {
  spr.fillRect(0, 0, 320, 26, COUL_BARRE());
  spr.setTextSize(1.1f);
  char t[24];
  char a[12];
  fmt1(etat.extT, a, sizeof a);
  snprintf(t, sizeof t, "Buse %s", etat.ok ? a : "--");
  spr.setTextColor(c(255, 183, 77), COUL_BARRE());
  spr.setCursor(8, 8);
  spr.print(t);
  fmt1(etat.bedT, a, sizeof a);
  snprintf(t, sizeof t, "Plateau %s", etat.ok ? a : "--");
  spr.setTextColor(c(79, 195, 247), COUL_BARRE());
  spr.setCursor(112, 8);
  spr.print(t);
  const char* w = etat.ok ? "En ligne" : "Hors ligne";
  spr.setTextColor(etat.ok ? c(156, 204, 60) : COUL_ROUGE(), COUL_BARRE());
  spr.setCursor(312 - spr.textWidth(w), 8);
  spr.print(w);
}

void dessineTitreMenu() {
  char titre[64];
  snprintf(titre, sizeof titre, "%s | %s", NOM_IMPRIMANTE,
           etat.ok ? (etat.etat[0] ? etat.etat : "?") : "pas de connexion");
  spr.fillRect(0, 30, 320, 20, COUL_FOND());
  spr.setTextColor(TFT_WHITE, COUL_FOND());
  spr.setTextSize(1.2f);
  spr.setCursor(8, 34);
  spr.print(titre);
}

// Une ligne de température : liseré, nom, "actuelle / cible"
void ligneTemp(int y, const char* nom, float actuelle, float cible, uint16_t couleur) {
  spr.fillRect(8, y, 4, 28, couleur);
  spr.fillRect(20, y, 300, 28, COUL_FOND());
  char v[32];
  if (etat.ok) {
    char a[12], b[12];
    fmt1(actuelle, a, sizeof a);
    fmt1(cible, b, sizeof b);
    snprintf(v, sizeof v, "%s / %s", a, b);
  } else {
    snprintf(v, sizeof v, "-- / --");
  }
  spr.setTextColor(TFT_WHITE, COUL_FOND());
  spr.setTextSize(1.2f);
  spr.setCursor(20, y + 8);
  spr.print(nom);
  spr.setCursor(312 - spr.textWidth(v), y + 8);
  spr.print(v);
}

void dessineTemperatures() {
  spr.setTextColor(COUL_GRIS(), COUL_FOND());
  spr.setTextSize(1.0f);
  const char* indice = "Temp (C) - toucher pour la cible";
  spr.setCursor(312 - spr.textWidth(indice), 62);
  spr.print(indice);
  ligneTemp(80, "Extruder", etat.extT, etat.extCible, c(224, 122, 31));
  ligneTemp(116, "Heater bed", etat.bedT, etat.bedCible, c(26, 158, 143));
}

// Ordonnée d'une température sur le graphique (échelle 0 à 50 C)
int yGraphe(float v) {
  if (v < 0) v = 0;
  if (v > 50) v = 50;
  return 286 - (int)(v * 1.92f);
}

// Trace l'historique dans l'ordre chronologique
void tracerHisto(const float* v, uint16_t couleur) {
  if (histoNb < 2) return;
  int debut = (histoPos - histoNb + NB_HISTO) % NB_HISTO;
  for (int k = 1; k < histoNb; k++) {
    int a = (debut + k - 1) % NB_HISTO;
    int b = (debut + k) % NB_HISTO;
    int x0 = 28 + ((k - 1) * 280) / (NB_HISTO - 1);
    int x1 = 28 + (k * 280) / (NB_HISTO - 1);
    spr.drawLine(x0, yGraphe(v[a]), x1, yGraphe(v[b]), couleur);
  }
}

void dessineGraphique() {
  spr.fillRect(29, 191, 278, 94, COUL_FOND());
  spr.drawRect(28, 190, 280, 96, c(45, 51, 61));
  spr.setTextColor(COUL_GRIS(), COUL_FOND());
  spr.setTextSize(1.0f);
  spr.setCursor(4, 186);
  spr.print("50");
  spr.setCursor(4, yGraphe(25) - 4);
  spr.print("25");
  spr.setCursor(12, 280);
  spr.print("0");
  tracerHisto(histoBuse, c(224, 122, 31));
  tracerHisto(histoPlateau, c(26, 158, 143));
}

void rafraichirMenu() {
  dessineBarreStatut();
  dessineTitreMenu();
  dessineTemperatures();
  dessineGraphique();
}

// ---------- Pages principales ----------

// Au démarrage : accueil directement, sauf si aucune imprimante n'est encore réglée
void pageAccueil() {
  if (imprimanteIp[0] == 0) pageConfig();
  else pageMenu();
}

void pageMenu() {
  page = PAGE_MENU;
  spr.fillScreen(COUL_FOND());
  rafraichirMenu();
  for (int i = 0; i < 5; i++) dessineTuile(i);
}

// ---------- Homing et déplacements ----------

void dessineDistances() {
  for (int i = 0; i < 4; i++) {
    dessineBouton(B_DIST[i], DIST_TXT[i], i == distanceIdx ? COUL_BLEU() : COUL_TUILE());
  }
}

void pageHoming() {
  page = PAGE_HOMING;
  spr.fillScreen(COUL_FOND());
  dessineEntete("Homing", etat.ok ? (imprimeEnCours() ? "Bloque : impression" : "Deplacements")
                                  : "Imprimante hors ligne");
  dessineBouton(B_HOME_X, "Home X", COUL_BLEU());
  dessineBouton(B_HOME_Y, "Home Y", COUL_BLEU());
  dessineBouton(B_HOME_Z, "Home Z", COUL_BLEU());
  dessineBouton(B_HOME_TOUT, "Home tout", COUL_SARCELLE());
  dessineDistances();
  dessineBouton(B_Z_PLUS, "Z+", COUL_TUILE());
  dessineBouton(B_Y_PLUS, "Y+", COUL_TUILE());
  dessineBouton(B_X_MOINS, "X-", COUL_TUILE());
  dessineBouton(B_X_PLUS, "X+", COUL_TUILE());
  dessineBouton(B_Z_MOINS, "Z-", COUL_TUILE());
  dessineBouton(B_Y_MOINS, "Y-", COUL_TUILE());
  dessineBouton(B_RETOUR, "Retour", COUL_NEUTRE());
}

// Déplace un axe d'une distance relative, signe + ou -
void deplacer(char axe, char signe) {
  if (bloqueParImpression()) return;
  int vitesse = (axe == 'Z') ? 600 : 3000;
  char s[64];
  snprintf(s, sizeof s, "G91\nG1 %c%c%s F%d\nG90", axe, signe, DIST_TXT[distanceIdx], vitesse);
  envoyerGcode(s);
}

// ---------- Températures ----------

void dessineValeurTemp(int y, const char* nom, float actuelle, int cible) {
  spr.fillRect(8, y - 4, 304, 26, COUL_FOND());
  char a[12], v[40];
  if (etat.ok) fmt1(actuelle, a, sizeof a);
  snprintf(v, sizeof v, "%s / %d C", etat.ok ? a : "--", cible);
  spr.setTextColor(TFT_WHITE, COUL_FOND());
  spr.setTextSize(1.2f);
  spr.setCursor(12, y);
  spr.print(nom);
  spr.setCursor(308 - spr.textWidth(v), y);
  spr.print(v);
}

void pageTemp() {
  page = PAGE_TEMP;
  spr.fillScreen(COUL_FOND());
  dessineEntete("Temperature", etat.ok ? "Cibles en C" : "Imprimante hors ligne");
  dessineValeurTemp(74, "Extruder", etat.extT, tgtExt);
  for (int k = 0; k < 4; k++) {
    char t[8];
    snprintf(t, sizeof t, "%+d", DELTAS[k]);
    dessineBouton(B_TEMP_EXT[k], t, COUL_TUILE());
  }
  dessineBouton(B_EXT_OFF, "Eteindre extruder", COUL_NEUTRE());
  dessineValeurTemp(244, "Heater bed", etat.bedT, tgtBed);
  for (int k = 0; k < 4; k++) {
    char t[8];
    snprintf(t, sizeof t, "%+d", DELTAS[k]);
    dessineBouton(B_TEMP_BED[k], t, COUL_TUILE());
  }
  dessineBouton(B_BED_OFF, "Eteindre plateau", COUL_NEUTRE());
  dessineBouton(B_RETOUR, "Retour", COUL_NEUTRE());
}

// Change la cible d'une source de chauffe (buse ou plateau) et l'envoie à Klipper
void reglerTemp(bool buse, int delta) {
  char s[24];
  if (buse) {
    tgtExt = borne(tgtExt + delta, 0, 300);
    snprintf(s, sizeof s, "M104 S%d", tgtExt);
  } else {
    tgtBed = borne(tgtBed + delta, 0, 120);
    snprintf(s, sizeof s, "M140 S%d", tgtBed);
  }
  envoyerGcode(s);
}

// Champ de saisie de la cible
void dessineCible() {
  spr.fillRoundRect(20, 70, 280, 40, 8, COUL_TUILE());
  spr.setTextColor(TFT_WHITE, COUL_TUILE());
  spr.setTextSize(1.6f);
  spr.setCursor(30, 80);
  if (cibleTxt[0]) {
    spr.print(cibleTxt);
    spr.print(" C");
  } else {
    spr.print("---");
  }
}

// Clavier numérique pour la cible de la buse ou du plateau
void pageCible(bool buse) {
  page = PAGE_CIBLE;
  cibleBuse = buse;
  cibleTxt[0] = 0;
  spr.fillScreen(COUL_FOND());
  dessineEntete(buse ? "Buse : cible" : "Plateau : cible", "Tape la temperature en C");
  dessineCible();
  for (int r = 0; r < 4; r++) {
    for (int col = 0; col < 3; col++) {
      const char* t = TOUCHES_CIBLE[r][col];
      dessineBouton(caseTouche(r, col), t, (t[0] == 'O') ? COUL_VERT() : COUL_TUILE());
    }
  }
  dessineBouton(B_ANNULER, "Annuler", COUL_NEUTRE());
}

// ---------- Actions : extrusion et ventilateur ----------

void pageActions() {
  page = PAGE_ACTIONS;
  spr.fillScreen(COUL_FOND());
  dessineEntete("Actions", imprimeEnCours() ? "Bloque : impression en cours"
                                            : (etat.ok ? "Extrusion et ventilateur"
                                                       : "Imprimante hors ligne"));
  for (int i = 0; i < 3; i++) {
    dessineBouton(B_EXT_DIST[i], EXT_LIBELLE[i], i == extIdx ? COUL_BLEU() : COUL_TUILE());
  }
  dessineBouton(B_EXTRUDER, "Extruder", COUL_SARCELLE());
  dessineBouton(B_RETRACTER, "Retracter", COUL_TUILE());

  spr.setTextColor(COUL_GRIS(), COUL_FOND());
  spr.setTextSize(1.2f);
  spr.setCursor(8, 186);
  spr.print("Ventilateur");
  for (int i = 0; i < 5; i++) {
    dessineBouton(B_FAN[i], FAN_LIBELLE[i], i == fanIdx ? COUL_BLEU() : COUL_TUILE());
  }
  spr.fillRect(8, 262, 304, 20, COUL_FOND());
  char v[40];
  snprintf(v, sizeof v, "Vitesse envoyee : %d %%", FAN_PCT[fanIdx]);
  spr.setTextColor(TFT_WHITE, COUL_FOND());
  spr.setTextSize(1.1f);
  spr.setCursor(8, 264);
  spr.print(v);
  dessineBouton(B_RETOUR, "Retour", COUL_NEUTRE());
}

// Extrusion (avance) ou rétraction de la distance choisie
void extruder(bool avance) {
  if (bloqueParImpression()) return;
  char s[48];
  snprintf(s, sizeof s, "M83\nG1 E%s%s F%d\nM82", avance ? "" : "-", EXT_TXT[extIdx],
           avance ? 300 : 1800);
  envoyerGcode(s);
}

// Ventilateur à la vitesse choisie : 0 coupe le ventilateur
void regleVentilateur() {
  if (bloqueParImpression()) return;
  char s[24];
  if (FAN_PCT[fanIdx] == 0) snprintf(s, sizeof s, "M107");
  else snprintf(s, sizeof s, "M106 S%d", FAN_PCT[fanIdx] * 255 / 100);
  envoyerGcode(s);
  pageActions();
}

// ---------- Print : état, Z offset, pause, reprise, annulation ----------

void dessinePrintLignes() {
  spr.fillRect(0, 50, 320, 150, COUL_FOND());
  ligneValeur(60, "Etat", etat.etat[0] ? etat.etat : "--");
  char nom[24];
  snprintf(nom, sizeof nom, "%.22s", etat.fichier[0] ? etat.fichier : "--");
  ligneValeur(96, "Fichier", nom);
  char v[16];
  snprintf(v, sizeof v, "%d %%", (int)(etat.progres + 0.5f));
  ligneValeur(132, "Progres", v);
  spr.drawRoundRect(8, 170, 304, 18, 6, c(45, 51, 61));
  int larg = (int)(etat.progres * 302.0f / 100.0f);
  if (larg < 0) larg = 0;
  if (larg > 302) larg = 302;
  spr.fillRoundRect(9, 171, 302, 16, 5, COUL_FOND());
  if (larg > 0) spr.fillRoundRect(9, 171, larg, 16, 5, c(224, 122, 31));
  if (!etat.ok) {
    spr.setTextColor(COUL_ROUGE(), COUL_FOND());
    spr.setTextSize(1.1f);
    spr.setCursor(12, 200);
    spr.print("Imprimante hors ligne");
  }
}

void dessineBoutonStop() {
  dessineBouton(B_STOP, annulerArme ? "Confirmer ?" : "Annuler", COUL_ROUGE());
}

// Page Print complète : redessinée à chaque rafraîchissement, sans clignotement
void dessinePrint() {
  spr.fillScreen(COUL_FOND());
  dessineEntete("Print", etat.ok ? (imprimeOuPause() ? "Impression en direct" : "Aucune impression")
                                 : "Imprimante hors ligne");
  dessinePrintLignes();
  if (imprimeOuPause()) {
    spr.setTextColor(COUL_GRIS(), COUL_FOND());
    spr.setTextSize(1.1f);
    spr.setCursor(8, 212);
    spr.print("Z offset (mm)");
    for (int i = 0; i < 4; i++) {
      dessineBouton(B_ZSTEP[i], ZSTEP_TXT[i], i == zIdx ? COUL_BLEU() : COUL_TUILE());
    }
    dessineBouton(B_ZMOINS, "Z -", COUL_TUILE());
    dessineBouton(B_ZPLUS, "Z +", COUL_TUILE());
    dessineBouton(B_PAUSE, "Pause", COUL_BLEU());
    dessineBouton(B_REPRENDRE, "Reprendre", COUL_SARCELLE());
    dessineBoutonStop();
  } else {
    dessineBouton(B_CHOISIR, "Choisir un fichier", COUL_BLEU());
  }
  dessineBouton(B_RETOUR, "Retour", COUL_NEUTRE());
}

void pagePrint() {
  page = PAGE_PRINT;
  annulerArme = false;
  dessinePrint();
}

// ---------- Fichiers de l'imprimante ----------

void dessineFichiers() {
  spr.fillRect(0, 50, 320, 310, COUL_FOND());
  for (int k = 0; k < 7; k++) {
    int idx = fichOff + k;
    if (idx >= nbFich) break;
    char l[34];
    snprintf(l, sizeof l, "%.32s", fichiers[idx]);
    dessineBouton(ficheRange(k), l, idx == fichSel ? COUL_BLEU() : COUL_TUILE());
  }
  dessineBouton(B_FICH_PREC, "Precedent", COUL_NEUTRE());
  dessineBouton(B_FICH_SUIV, "Suivant", COUL_NEUTRE());
  dessineBouton(B_LANCER, "Lancer", COUL_SARCELLE());
  dessineBouton(B_FICH_RETOUR, "Retour", COUL_NEUTRE());
}

void pageFichiers() {
  page = PAGE_FICHIERS;
  fichSel = -1;
  fichOff = 0;
  spr.fillScreen(COUL_FOND());
  dessineEntete("Fichiers", "Chargement...");
  montrer();
  nbFich = fichiersLister(fichiers, 24);
  dessineEntete("Fichiers", nbFich ? "Choisis un fichier puis Lancer"
                                   : (imprimanteIp[0] ? "Aucun fichier G-code" : "Adresse non definie"));
  dessineFichiers();
}

// ---------- Réglages : imprimante et Wi-Fi ----------

void pageConfig() {
  page = PAGE_CONFIG;
  spr.fillScreen(COUL_FOND());
  dessineEntete("Reglages", "Imprimante et Wi-Fi");
  ligneValeur(60, "Imprimante", NOM_IMPRIMANTE);
  ligneValeur(96, "Adresse", imprimanteIp[0] ? imprimanteIp : "non definie");
  ligneValeur(132, "Wi-Fi", wifiSsid[0] ? wifiSsid : "non enregistre");
  ligneValeur(168, "Etat Wi-Fi", wifiOk() ? "connecte" : "non connecte");
  dessineBouton(B_WIFI, "Reseau Wi-Fi", COUL_SARCELLE());
  dessineBouton(B_CHG_IP, "Adresse imprimante (IP)", COUL_BLEU());
  dessineBouton(B_ORIENT, ecranInverse ? "Orientation : portrait inverse" : "Orientation : portrait", COUL_NEUTRE());
  dessineBouton(B_RETOUR, "Retour", COUL_NEUTRE());
}

// Recherche les réseaux Wi-Fi visibles et les affiche
void pageWifi() {
  page = PAGE_WIFI;
  spr.fillScreen(COUL_FOND());
  dessineEntete("Wi-Fi", "Recherche des reseaux...");
  montrer();
  wNb = wifiRechercher(wNoms, wSignal, 8);
  wSel = -1;
  dessineEntete("Wi-Fi", wNb ? "Choisis un reseau" : "Aucun reseau trouve");
  for (int i = 0; i < wNb; i++) {
    char l[40];
    snprintf(l, sizeof l, "%.24s  %d dBm", wNoms[i], wSignal[i]);
    dessineBouton(wifiRang(i), l, COUL_TUILE());
  }
  dessineBouton(B_WIFI_SCAN, "Rechercher", COUL_BLEU());
  dessineBouton(B_RETOUR, "Retour", COUL_NEUTRE());
}

void dessineMdp() {
  spr.fillRoundRect(8, 52, 304, 36, 8, COUL_TUILE());
  spr.setTextColor(TFT_WHITE, COUL_TUILE());
  spr.setTextSize(1.3f);
  spr.setCursor(16, 60);
  int n = strlen(mdp);
  spr.print(n > 24 ? mdp + n - 24 : mdp);
}

void dessineClavierMdp() {
  for (int r = 0; r < 5; r++) {
    for (int col = 0; col < 10; col++) {
      char l[2];
      l[0] = CLAVIER_MDP[r][col];
      l[1] = 0;
      if (maj && l[0] >= 'a' && l[0] <= 'z') l[0] = l[0] - 32;
      dessineBouton(toucheMdp(r, col), l, COUL_TUILE());
    }
  }
  dessineBouton(B_MAJ, maj ? "MAJ ON" : "Maj", maj ? COUL_BLEU() : COUL_TUILE());
  dessineBouton(B_ESPACE, "Espace", COUL_TUILE());
  dessineBouton(B_EFFACER, "Effacer", COUL_TUILE());
}

void pageMdp() {
  page = PAGE_MDP;
  mdp[0] = 0;
  maj = false;
  spr.fillScreen(COUL_FOND());
  char sub[48];
  snprintf(sub, sizeof sub, "Mot de passe : %.24s", wNoms[wSel]);
  dessineEntete("Mot de passe", sub);
  dessineMdp();
  dessineClavierMdp();
  dessineBouton(B_MDP_OK, "OK", COUL_VERT());
  dessineBouton(B_MDP_ANNULER, "Annuler", COUL_NEUTRE());
}

// ---------- Pages de saisie ----------

void dessineSaisie() {
  spr.fillRoundRect(20, 70, 280, 40, 8, COUL_TUILE());
  spr.setTextColor(TFT_WHITE, COUL_TUILE());
  spr.setTextSize(1.6f);
  spr.setCursor(30, 80);
  spr.print(saisie[0] ? saisie : "---");
}

void pageClavier() {
  page = PAGE_CLAVIER;
  strncpy(saisie, imprimanteIp, sizeof saisie - 1);
  saisie[sizeof saisie - 1] = 0;
  spr.fillScreen(COUL_FOND());
  dessineEntete("Adresse IP", "Saisis l'adresse de l'imprimante");
  dessineSaisie();
  for (int r = 0; r < 4; r++) {
    for (int col = 0; col < 3; col++) {
      dessineBouton(caseTouche(r, col), TOUCHES_IP[r][col], COUL_TUILE());
    }
  }
  dessineBouton(B_OK, "OK", COUL_VERT());
  dessineBouton(B_ANNULER, "Annuler", COUL_NEUTRE());
}

void saisieAjouter(const char* t) {
  int n = strlen(saisie);
  if (t[0] == '<') {
    if (n > 0) saisie[n - 1] = 0;
    return;
  }
  if (n >= 15) return;
  if (t[0] == '.' && (n == 0 || saisie[n - 1] == '.')) return;
  saisie[n] = t[0];
  saisie[n + 1] = 0;
}

// Vrai si le texte est une adresse IPv4 : quatre nombres de 0 à 255 séparés par des points
bool ipValide(const char* s) {
  int points = 0, chiffres = 0, valeur = 0;
  for (const char* p = s;; p++) {
    if (*p == '.' || *p == 0) {
      if (chiffres == 0 || valeur > 255) return false;
      if (*p == 0) return points == 3;
      points++;
      chiffres = 0;
      valeur = 0;
    } else if (*p >= '0' && *p <= '9') {
      valeur = valeur * 10 + (*p - '0');
      chiffres++;
      if (chiffres > 3) return false;
    } else {
      return false;
    }
  }
}

// ---------- Mise à jour périodique et appuis ----------

// Mise à jour des pages affichées, puis envoi à l'écran
void rafraichirEcran() {
  if (page == PAGE_MENU) {
    rafraichirMenu();
  } else if (page == PAGE_TEMP) {
    dessineValeurTemp(74, "Extruder", etat.extT, tgtExt);
    dessineValeurTemp(244, "Heater bed", etat.bedT, tgtBed);
  } else if (page == PAGE_PRINT) {
    dessinePrintLignes();   // seules les valeurs sont mises à jour, pas tout l'écran
  }
  montrer();
}

// Traite un appui sur l'écran selon la page affichée
void toucherPage(int x, int y) {
  if (page == PAGE_MENU) {
    if (y < 26) { pageConfig(); return; }                       // barre de statut : réglages
    if (y >= 78 && y < 111) { pageCible(true); return; }        // ligne buse
    if (y >= 111 && y < 148) { pageCible(false); return; }      // ligne plateau
    if (appui(caseTuile(0), x, y)) { pageHoming(); return; }
    if (appui(caseTuile(1), x, y)) {
      tgtExt = (int)(etat.extCible + 0.5f);
      tgtBed = (int)(etat.bedCible + 0.5f);
      pageTemp();
      return;
    }
    if (appui(caseTuile(2), x, y)) { pageActions(); return; }
    if (appui(caseTuile(3), x, y)) { pageConfig(); return; }
    if (appui(caseTuile(4), x, y)) { pagePrint(); return; }

  } else if (page == PAGE_HOMING) {
    if (appui(B_RETOUR, x, y)) { pageMenu(); return; }
    if (appui(B_HOME_X, x, y)) { if (!bloqueParImpression()) envoyerGcode("G28 X"); return; }
    if (appui(B_HOME_Y, x, y)) { if (!bloqueParImpression()) envoyerGcode("G28 Y"); return; }
    if (appui(B_HOME_Z, x, y)) { if (!bloqueParImpression()) envoyerGcode("G28 Z"); return; }
    if (appui(B_HOME_TOUT, x, y)) { if (!bloqueParImpression()) envoyerGcode("G28"); return; }
    for (int i = 0; i < 4; i++) {
      if (appui(B_DIST[i], x, y)) { distanceIdx = i; pageHoming(); return; }
    }
    if (appui(B_Z_PLUS, x, y)) { deplacer('Z', '+'); return; }
    if (appui(B_Y_PLUS, x, y)) { deplacer('Y', '+'); return; }
    if (appui(B_X_MOINS, x, y)) { deplacer('X', '-'); return; }
    if (appui(B_X_PLUS, x, y)) { deplacer('X', '+'); return; }
    if (appui(B_Z_MOINS, x, y)) { deplacer('Z', '-'); return; }
    if (appui(B_Y_MOINS, x, y)) { deplacer('Y', '-'); return; }

  } else if (page == PAGE_TEMP) {
    if (appui(B_RETOUR, x, y)) { pageMenu(); return; }
    for (int k = 0; k < 4; k++) {
      if (appui(B_TEMP_EXT[k], x, y)) { reglerTemp(true, DELTAS[k]); pageTemp(); return; }
      if (appui(B_TEMP_BED[k], x, y)) { reglerTemp(false, DELTAS[k]); pageTemp(); return; }
    }
    if (appui(B_EXT_OFF, x, y)) { reglerTemp(true, -1000); pageTemp(); return; }
    if (appui(B_BED_OFF, x, y)) { reglerTemp(false, -1000); pageTemp(); return; }

  } else if (page == PAGE_CIBLE) {
    if (appui(B_ANNULER, x, y)) { pageMenu(); return; }
    for (int r = 0; r < 4; r++) {
      for (int col = 0; col < 3; col++) {
        if (!appui(caseTouche(r, col), x, y)) continue;
        const char* t = TOUCHES_CIBLE[r][col];
        if (t[0] == '<') {
          int n = strlen(cibleTxt);
          if (n > 0) cibleTxt[n - 1] = 0;
        } else if (t[0] == 'O') {
          int v = atoi(cibleTxt);
          int maxi = cibleBuse ? 300 : 120;
          if (cibleTxt[0] == 0 || v > maxi) {
            dessineMessage("Valeur hors limites", COUL_ROUGE());
            return;
          }
          char s[24];
          if (cibleBuse) { tgtExt = v; snprintf(s, sizeof s, "M104 S%d", v); }
          else { tgtBed = v; snprintf(s, sizeof s, "M140 S%d", v); }
          bool ok = gcodeEnvoyer(s);
          pageMenu();
          if (!ok) dessineMessage("Commande non envoyee", COUL_ROUGE());
          return;
        } else {
          int n = strlen(cibleTxt);
          if (n < 3) {
            cibleTxt[n] = t[0];
            cibleTxt[n + 1] = 0;
          }
        }
        dessineCible();
        return;
      }
    }

  } else if (page == PAGE_ACTIONS) {
    if (appui(B_RETOUR, x, y)) { pageMenu(); return; }
    for (int i = 0; i < 3; i++) {
      if (appui(B_EXT_DIST[i], x, y)) { extIdx = i; pageActions(); return; }
    }
    if (appui(B_EXTRUDER, x, y)) { extruder(true); return; }
    if (appui(B_RETRACTER, x, y)) { extruder(false); return; }
    for (int i = 0; i < 5; i++) {
      if (appui(B_FAN[i], x, y)) { fanIdx = i; regleVentilateur(); return; }
    }

  } else if (page == PAGE_PRINT) {
    if (appui(B_RETOUR, x, y)) { pageMenu(); return; }
    if (!imprimeOuPause()) {
      if (appui(B_CHOISIR, x, y)) { pageFichiers(); return; }
      return;
    }
    for (int i = 0; i < 4; i++) {
      if (appui(B_ZSTEP[i], x, y)) { zIdx = i; dessinePrint(); return; }
    }
    if (appui(B_ZMOINS, x, y)) { zOffsetAjuster(-ZSTEP_CENT[zIdx]); return; }
    if (appui(B_ZPLUS, x, y)) { zOffsetAjuster(ZSTEP_CENT[zIdx]); return; }
    if (appui(B_PAUSE, x, y)) {
      annulerArme = false;
      if (imprimeEnCours()) envoyerGcode("PAUSE");
      else dessineMessage("Pas d'impression en cours", COUL_ROUGE());
      return;
    }
    if (appui(B_REPRENDRE, x, y)) {
      annulerArme = false;
      if (enPause()) envoyerGcode("RESUME");
      else dessineMessage("Impression non en pause", COUL_ROUGE());
      return;
    }
    if (appui(B_STOP, x, y)) {
      if (!annulerArme) {
        annulerArme = true;
        dessineMessage("Appuie encore sur Confirmer pour annuler", COUL_ROUGE());
      } else {
        annulerArme = false;
        envoyerGcode("CANCEL_PRINT");
      }
      return;
    }

  } else if (page == PAGE_FICHIERS) {
    if (appui(B_FICH_RETOUR, x, y)) { pagePrint(); return; }
    if (appui(B_FICH_PREC, x, y)) {
      if (fichOff >= 7) fichOff -= 7;
      dessineFichiers();
      return;
    }
    if (appui(B_FICH_SUIV, x, y)) {
      if (fichOff + 7 < nbFich) fichOff += 7;
      dessineFichiers();
      return;
    }
    for (int k = 0; k < 7; k++) {
      int idx = fichOff + k;
      if (idx >= nbFich) break;
      if (appui(ficheRange(k), x, y)) {
        fichSel = idx;
        dessineFichiers();
        return;
      }
    }
    if (appui(B_LANCER, x, y)) {
      if (fichSel < 0) { dessineMessage("Choisis d'abord un fichier", COUL_ROUGE()); return; }
      if (imprimeOuPause()) { dessineMessage("Une impression est deja en cours", COUL_ROUGE()); return; }
      if (imprimerFichier(fichiers[fichSel])) pagePrint();
      else dessineMessage("Echec du lancement", COUL_ROUGE());
      return;
    }

  } else if (page == PAGE_WIFI) {
    if (appui(B_RETOUR, x, y)) { pageConfig(); return; }
    if (appui(B_WIFI_SCAN, x, y)) { pageWifi(); return; }
    for (int i = 0; i < wNb; i++) {
      if (appui(wifiRang(i), x, y)) {
        wSel = i;
        pageMdp();
        return;
      }
    }

  } else if (page == PAGE_MDP) {
    if (appui(B_MDP_ANNULER, x, y)) { pageWifi(); return; }
    if (appui(B_MAJ, x, y)) { maj = !maj; dessineClavierMdp(); return; }
    if (appui(B_ESPACE, x, y)) {
      int n = strlen(mdp);
      if (n < 63) { mdp[n] = ' '; mdp[n + 1] = 0; }
      dessineMdp();
      return;
    }
    if (appui(B_EFFACER, x, y)) {
      int n = strlen(mdp);
      if (n > 0) mdp[n - 1] = 0;
      dessineMdp();
      return;
    }
    for (int r = 0; r < 5; r++) {
      for (int col = 0; col < 10; col++) {
        if (!appui(toucheMdp(r, col), x, y)) continue;
        char ch = CLAVIER_MDP[r][col];
        if (maj && ch >= 'a' && ch <= 'z') ch = ch - 32;
        int n = strlen(mdp);
        if (n < 63) { mdp[n] = ch; mdp[n + 1] = 0; }
        dessineMdp();
        return;
      }
    }
    if (appui(B_MDP_OK, x, y)) {
      wifiSauver(wNoms[wSel], mdp);
      WiFi.disconnect();
      wifiConnecter();
      pageConfig();
      dessineMessage("Connexion Wi-Fi en cours...", COUL_SARCELLE());
      return;
    }

  } else if (page == PAGE_CONFIG) {
    if (appui(B_RETOUR, x, y)) { pageMenu(); return; }
    if (appui(B_WIFI, x, y)) { pageWifi(); return; }
    if (appui(B_CHG_IP, x, y)) { pageClavier(); return; }
    if (appui(B_ORIENT, x, y)) { ecranOrientationBasculer(); return; }

  } else if (page == PAGE_CLAVIER) {
    for (int r = 0; r < 4; r++) {
      for (int col = 0; col < 3; col++) {
        if (appui(caseTouche(r, col), x, y)) {
          saisieAjouter(TOUCHES_IP[r][col]);
          dessineSaisie();
          return;
        }
      }
    }
    if (appui(B_OK, x, y)) {
      if (ipValide(saisie)) {
        reseauSauverIp(saisie);
        pageMenu();
      } else {
        dessineMessage("Adresse invalide", COUL_ROUGE());
      }
      return;
    }
    if (appui(B_ANNULER, x, y)) pageConfig();
  }
}

// Appui sur l'écran : traitement puis envoi du tampon à l'écran
void toucher(int x, int y) {
  toucherPage(x, y);
  montrer();
}
