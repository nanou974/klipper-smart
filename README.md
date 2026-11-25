# KlipperSmart

Écran de contrôle pour imprimante 3D Klipper, sur carte ESP32-S3 avec écran tactile ILI9488 320 × 480 (contrôleur tactile NS2009).

Fonctions : accueil avec températures en direct, réglages du Wi-Fi (recherche des réseaux) et de l'adresse IP de l'imprimante, orientation portrait ou portrait inversé, déplacements et homing, températures, extrusion, ventilateur, suivi d'impression avec réglage du Z offset, choix et lancement de fichiers G-code.

## Documentation

- [Mode d'emploi (PDF)](docs/mode_emploi_klipper.pdf) : chaque écran et chaque action, avec les maquettes, puis la notice de téléversement du firmware (USB et Wi-Fi) et le changement du mot de passe OTA.
- [Schéma de branchement et brochage (SVG)](docs/brochage.svg) : ESP32-S3, écran TFT35 SPI et breakout USB-C pour l'alimentation.
- [Étiquette pour l'écran (SVG)](docs/etiquette_ecran.svg) : à imprimer à 100 % (sans mise à l'échelle), format 85 × 128 mm avec une marge de 10 mm autour du sticker de 65 × 108 mm. La fenêtre est découpée pour laisser voir l'écran.

## Contenu

- `KlipperSmart.ino` : programme principal (Wi-Fi, tactile, mise à jour Moonraker, mise à jour Wi-Fi).
- `ecran.h` : pages et boutons.
- `klipper_api.h` : communication avec Moonraker.
- `LGFX_Config.h` : configuration de l'écran.

## Matériel

- Carte **ESP32-S3 DevKitC-1-N16R8** (16 Mo de flash, 8 Mo de PSRAM).
- Écran **BIGTREETECH TFT35 SPI V2.1** : 3,5 pouces, 480 × 320, contrôleur ILI9488, tactile NS2009 (I2C), alimentation 5 V.
- **Breakout USB-C à 14 broches avec résistances**, pour l'alimentation en 5 V.

Le brochage complet est dans le mode d'emploi (annexe) et dans le schéma ci-dessous.

## Installation

- Carte : ESP32-S3 (Arduino IDE : « ESP32S3 Dev Module »).
- Paquet de cartes « esp32 » d'Espressif, version 3.2.1.
- Bibliothèque LovyanGFX, version 1.2.33.
- Options : Flash Size 16 Mo, PSRAM OPI, Partition Scheme « 16M Flash (3MB APP / 9.9MB FATFS) ».

Le premier téléversement se fait par USB. Les téléversements suivants peuvent se faire par Wi-Fi : l'écran apparaît alors sous le nom `klipper-smart`.

Le mot de passe de mise à jour Wi-Fi est défini dans `KlipperSmart.ino`, dans la fonction `otaDemarrer()`. Change-le avant le premier téléversement.
