# CppSandBox — station au sol du robot lidar, et bac à sable C++

Malgré son nom, ce dépôt contient surtout **la station de contrôle PC du robot
[EspLidar](https://github.com/RigonAj/EspLidar)** : `Lidar.cpp` reçoit en UDP les mesures du robot,
reconstruit la carte, l'affiche, et renvoie les commandes de pilotage.

Le reste — quelques exercices C++ — est listé en fin de page et clairement séparé.

> Sans le [firmware](https://github.com/RigonAj/EspLidar), ce programme n'a rien à afficher : les
> deux dépôts sont les deux moitiés du même système.

---

## La station au sol (`Lidar.cpp`)

Application **C++ / raylib**, sans dépendance lourde.

- **Carte** (`map.h`, `map.cpp`) — jusqu'à 10 000 points, accumulés dans une `RenderTexture2D` pour
  ne pas redessiner tout l'historique à chaque image. Ajout en coordonnées **polaires** (c'est ce que
  le lidar produit) ou cartésiennes, caméra déplaçable, remise à zéro.
- **Robot** (`Robot.h`) — hérite de `Map`. Deux fils d'exécution, un par flux : mesures lidar
  (port 8080) et état du robot (port 8081). Pose reconstruite depuis le compteur de pas et le lacet
  de l'IMU, dessinée par-dessus la carte.
- **Réseau** (`serveur.h`) — un petit serveur UDP en sockets POSIX, sans dépendance.
- **Interface** (`Gui`, `slider`, `TextBoxes`, `inputbox`) — bibliothèque de widgets maison
  au-dessus de raylib : pages, curseurs de réglage (vitesse, accélération, pas), zones de texte.
  Les curseurs règlent le robot **en marche**.

### Ce qui mérite un coup d'œil

**Le même triple buffer que côté firmware, mais côté PC.** Les fils réseau écrivent, le fil de rendu
lit, et personne n'attend :

```cpp
vector<Vector2> Point1, Point2, Point3;
vector<Vector2> *Pointwrite, *Pointtmp, *PointRead;
```

L'affichage tourne à sa cadence, la réception à la sienne. Une trame en retard est écrasée plutôt
que mise en file — pour de la cartographie temps réel, c'est le bon compromis.

**Les structures de trame sont dupliquées à l'identique depuis le firmware** (`Command`,
`RobotState`, `Point`). Il n'y a pas de format d'échange intermédiaire : les octets reçus sont
réinterprétés directement, pour la latence. Contrepartie assumée : toute modification doit être
faite **des deux côtés**, et les deux binaires doivent avoir le même boutisme et le même alignement.

**La latence est mesurée**, pas supposée : `TimeElapsedLidar()` et `TimeElapsedImu()` exposent le
temps écoulé entre deux trames, en millisecondes.

---

## Compilation

Dépendances : `raylib`, un compilateur C++ récent, X11.

```bash
g++ Lidar.cpp inputbox.cpp TextBoxes.cpp map.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o Lidar && ./Lidar
```

Version autonome, avec raylib lié statiquement :

```bash
g++ Lidar.cpp inputbox.cpp TextBoxes.cpp map.cpp /usr/local/lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11 -o Lidar_Portable
```

La police `Font/cascadia-mono/` et l'image `Robot.png` sont chargées depuis le dossier courant :
lancer le binaire depuis la racine du dépôt.

---

## Le reste — le bac à sable, assumé

Ces fichiers sont des exercices et des essais d'API, gardés pour l'historique. Ils ne font pas
partie de la station au sol :

| Fichier | Ce que c'est |
|---|---|
| `EspRead.cpp` | premier essai de lecture des trames de l'ESP32, avant `Robot.h` |
| `client.cpp` / `client.h` | client UDP d'essai, pendant de `serveur.h` |
| `classes.cpp` | exercice sur l'héritage et les classes |
| `main.cpp`, `main2.cpp`, `test.cpp` | essais divers |

## Limites connues

- **Des binaires compilés sont commités** (`Lidar`, `Lidar_Portable`, `main`, `test`, `textbox.o`,
  `textbox.so`) et le dépôt n'a pas de `.gitignore`.
- Pas de système de compilation : les commandes ci-dessus sont recopiées de `Compil.txt`.
- L'adresse du robot et les ports sont en dur dans `Lidar.cpp`.
- L'organisation est plate — tout à la racine. C'est l'état du projet, pas une recommandation.
