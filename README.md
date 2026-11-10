# Super Mario ESP

Un jeu de plateforme inspiré de Super Mario, qui tourne sur un ESP32 avec un écran OLED de 128 x 64 pixels. On contrôle le personnage depuis le clavier d'un PC, en WiFi.

Ce README résume ce qui a été ajouté depuis le rapport de suivi (`document.md`). La version rendue avec ce rapport se trouve dans le dossier `rendu_suivi_Ilian_Igoudjil/`.

## Où en était le projet au moment du rapport

- L'ESP32 crée le point d'accès `ESP32-IlianLeBoss` et un serveur TCP sur le port 8080.
- Le script Python `bleScript.py` envoie une lettre à chaque appui sur une touche.
- L'écran affiche l'état de la connexion, la touche reçue, puis un écran d'accueil. La touche Entrée lance la partie.

## Ce qui a été fait depuis

Les trois premières des six étapes prévues sont faites : afficher Mario, gérer la physique et déplacer Mario.

### 1. Afficher Mario

- Mario est un sprite de 16 x 16 pixels, stocké dans le tableau `epd_bitmap_mario_small` en mémoire flash (`PROGMEM`).
- Le sprite est dessiné avec `display.drawBitmap()` à la position `(marioPositionX, marioPositionY)`.
- Le sol est une ligne horizontale à `SOL_Y = 60`.

### 2. Gérer la physique

- Une vitesse verticale `vitesseY` est ajoutée à la position de Mario à chaque image.
- Gravité : quand Mario monte, `vitesseY` augmente de 0,5 par image. Quand il redescend, elle augmente de 2. Le saut monte donc lentement et retombe vite.
- Collision avec le sol : si Mario passe sous `SOL_Y`, il est replacé sur le sol, sa vitesse revient à 0 et `auSol` passe à `true`.

### 3. Déplacer Mario

- La nouvelle fonction `movements(char touche)` traite chaque octet reçu :

| Octet reçu | Effet |
| --- | --- |
| `L` / `l` | Commence / arrête le déplacement à gauche |
| `R` / `r` | Commence / arrête le déplacement à droite |
| `U` | Saut (`vitesseY = -5`), seulement si Mario est au sol |

- Tant que `goLeft` ou `goRight` vaut `true`, Mario avance de 2,5 pixels par image.
- Tous les octets en attente sont lus dans la même boucle, ce qui évite d'en perdre si plusieurs touches arrivent entre deux images.

### Côté Python (`bleScript.py`)

- Le script envoie maintenant le **relâchement** des flèches : `l` pour la gauche, `r` pour la droite. On peut donc garder une flèche enfoncée pour avancer en continu.
- Un ensemble `pressed` ignore la répétition automatique du clavier : une touche maintenue n'est envoyée qu'une seule fois.

## Prochaines étapes

Les niveaux sont prévus en commentaire dans `mario.ino`, et une fonction `niveau1()` vide est déjà en place :

- [ ] Niveau 1 : prendre des pièces
- [ ] Niveau 2 : pièces + casser des blocs
- [ ] Niveau 3 : pièces + blocs + tuer des monstres

Autres points à traiter :

- La touche `D` (flèche bas) est envoyée mais pas encore utilisée par l'ESP32 (s'accroupir).
- Mario n'est pas bloqué sur les bords : il peut sortir de l'écran.
- La boucle du jeu contient deux `delay(50)`, soit environ 100 ms par image.

## Lancer le projet

1. Téléverser `mario.ino` sur l'ESP32 avec l'Arduino IDE. Il faut les bibliothèques Adafruit GFX et Adafruit SSD1306.
2. Connecter le PC au WiFi `ESP32-IlianLeBoss`.
3. Lancer le script :
   ```bash
   python3 -m venv venv
   source venv/bin/activate
   pip install -r requirements.txt
   python bleScript.py
   ```
4. Appuyer sur Entrée pour lancer la partie, utiliser les flèches pour jouer et Échap pour quitter le script.
