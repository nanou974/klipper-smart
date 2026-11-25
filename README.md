# KlipperSmart

Écran de contrôle pour imprimante 3D Klipper, sur carte ESP32-S3 avec écran tactile ILI9488 320 × 480 (contrôleur tactile NS2009).

Fonctions : accueil avec températures en direct, réglages du Wi-Fi (recherche des réseaux) et de l'adresse IP de l'imprimante, orientation portrait ou portrait inversé, déplacements et homing, températures et clavier de cible, extrusion, ventilateur, suivi d'impression avec réglage du Z offset, choix et lancement de fichiers G-code.

## Documentation

- [Mode d'emploi (PDF)](docs/mode_emploi_klipper.pdf) : chaque écran et chaque action, avec les maquettes, puis la notice de téléversement du firmware (USB et Wi-Fi) et le changement du mot de passe OTA.
- [Étiquette pour l'écran (SVG)](docs/etiquette_ecran.svg) : à imprimer à 100 % (sans mise à l'échelle), format 85 × 128 mm avec une marge de 10 mm autour du sticker de 65 × 108 mm. La fenêtre est découpée pour laisser voir l'écran.

## Contenu

- `KlipperSmart.ino` : programme principal (Wi-Fi, tactile, mise à jour Moonraker, mise à jour Wi-Fi).
- `ecran.h` : pages et boutons.
- `klipper_api.h` : communication avec Moonraker.
- `LGFX_Config.h` : configuration de l'écran.

## Installation

- Carte : ESP32-S3 (Arduino IDE : « ESP32S3 Dev Module »).
- Paquet de cartes « esp32 » d'Espressif, version 3.2.1.
- Bibliothèque LovyanGFX, version 1.2.33.
- Options : Flash Size 16 Mo, PSRAM OPI, Partition Scheme « 16M Flash (3MB APP / 9.9MB FATFS) ».

Le premier téléversement se fait par USB. Les téléversements suivants peuvent se faire par Wi-Fi : l'écran apparaît alors sous le nom `klipper-smart`.

Le mot de passe de mise à jour Wi-Fi est défini dans `KlipperSmart.ino`, dans la fonction `otaDemarrer()`. Change-le avant le premier téléversement.
