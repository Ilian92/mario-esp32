## Super Mario ESP : suivi du projet

Ilian Igoudjil

## Objectif du projet

Créer un clone de Super Mario sur un microcontrôleur ESP32 équipé d'un écran OLED de 128 x 64 pixels. Tu contrôles le personnage depuis ton ordinateur, en WiFi, avec les touches de ton clavier.

À ce jour, la communication entre le PC et l'ESP32 fonctionne. L'ESP32 reçoit chaque touche et affiche un écran d'accueil qui lance la partie avec la touche Entrée.

## Matériel et technologies

Le projet repose sur deux programmes : un code Arduino en C++ sur l'ESP32 et un script Python sur ton PC.

| Élément ESP32 | Rôle dans le projet Exécute le jeu et crée le réseau WiFi |
| --- | --- |
| Écran OLED (128 x 64) | Affiche le jeu |
| Arduino IDE (C++) | Compile et téléverse le code sur l'ESP32 |
| WiFi.h Wire.h Adafruit_GFX et Adafruit_SSD1306 Python 3 socket (Python) pynput (Python) venv | Crée le point d'accès et le serveur TCP sur le port 8080 Gère le bus I2C de l'écran (broches 17 et 18) Dessinent le texte et les formes sur l'écran Lit le clavier et envoie les touches Ouvre la connexion TCP vers 192.168.4.1:8080 Détecte les appuis sur les touches du clavier Isole les dépendances Python du projet |

## Ce qui a été fait

Six étapes sont terminées et testées.

- 1. Point d'accès WiFi : l'ESP32 crée le réseau ESP32-IlianLeBoss. Le PC s'y connecte et reçoit une adresse en 192.168.4.x.

- 2. Serveur TCP : l'ESP32 écoute sur le port 8080 et garde la connexion du client ouverte.


- 3. Script Python : le script se connecte à l'ESP32 et envoie une lettre à chaque appui sur une touche.

- 4. Affichage d'état : l'écran indique le nombre d'utilisateurs connectés, ou l'adresse IP quand personne n'est connecté.

- 5. Lecture des touches : l'ESP32 lit la lettre reçue et l'affiche en grand sur l'écran.

- 6. Écran d'accueil : la touche Entrée lance la partie grâce à la variable isGameStarted.

## Fonctionnement et protocole

Chaque touche correspond à une lettre. Le script Python envoie cette lettre sous forme d'un octet, par exemple b"L" pour la flèche gauche.

| Touche | Octet envoyé | Usage prévu |
| --- | --- | --- |
| Flèche gauche | L | Aller à gauche |
| Flèche droite | R | Aller à droite |
| Flèche haut | U | Sauter |
| Flèche bas | D | S'accroupir |
| Entrée | E | Lancer la partie |
| Échap | aucun | Quitter le script Python |

L'ESP32 lit l'octet avec client.read() et le range dans une variable de type char. La boucle loop() tourne toutes les 50 ms et dessine un seul écran par tour : le jeu, l'accueil ou l'attente de connexion.

## Problèmes rencontrés et solutions

| Problème | Cause | Solution |
| --- | --- | --- |
| ModuleNotFoundErr | Librairie absente | Installer pynput dans un venv |
| or: pynput |   | avec pip |
| BrokenPipeError | client = server.available() à chaque tour | Remplacer le client |
| après quelques | remplace le client par un client vide et ferme la | uniquement quand un |
| touches | connexion | nouveau client arrive |
| Écran lent à réagir | Trois delay(2000) dans loop() | Un seul delay(50) |
| Erreur de | Un char comparé à une chaîne de caractères | Écrire 'E' avec des |
| compilation sur |   | apostrophes |
| touche == "E" |   |   |


| Problème | Cause | Solution |
| --- | --- | --- |
| Partie lancée dès le | = au lieu de == dans le if | Comparer avec == et utiliser if |
| démarrage |   | / else if / else |

## Prochaines étapes

Six étapes restent à réaliser, dans cet ordre. Chaque étape s'appuie sur la précédente.

- Afficher Mario

- Gérer la physique

- Déplacer Mario

- Prendre des pièces

- Casser des blocs

- Tuer des monstres
