# Projet_robot
une application de contrôle de robot en C communicant avec un simulateur Java
# Robot MRPiZ - Simulateur Intox

## Description

projet fait à 2 dans le cadre de mon cursus universitaire (Sylvain Francius et ROBIN Maxime)

Application de contrôle de robot en langage C qui communique avec le simulateur Intox MrPizz (programme Java). Ce projet permet de contrôler un robot virtuel avec différents modes de déplacement et trajectoires prédéfinies.

## Langage et Technologies

- **Langage**: C (standard GNU11)
- **Compilateur**: GCC
- **Bibliothèques**: 
  - MRPiZ (communication avec le simulateur)
  - Bibliothèques POSIX (unistd, termios, fcntl)
  - pthread (multitâche)
- **Simulateur**: Intox MrPizz v0.9 (Java 11+)

## Architecture du Projet

```
Projet_robot/
├── intox_v0.9-mrpiz_v0.5-java_11.jar    # Simulateur Java
├── lib_mrpiz/                            # Bibliothèque MRPiZ
│   ├── include/                          # Fichiers d'en-tête
│   └── lib/                              # Bibliothèques compilées
└── RM_Final/
    ├── src/                              # Code source C
    │   ├── main.c                        # Point d'entrée principal
    │   ├── Makefile                      # Script de compilation
    │   ├── robot_app/
    │   │   ├── pilot.h/c                 # Contrôle des mouvements
    │   │   ├── copilot.h/c               # Gestion des chemins
    │   │   ├── robot.h/c                 # Interface robot
    │   │   ├── app_manager.h/c           # Gestion de l'application
    │   │   └── IHM.h/c                   # Interface utilisateur
    │   └── utils.h                       # Utilitaires de débogage
    └── bin/                              # Exécutables compilés
        └── go                            # Exécutable principal
```

## Prérequis

### Logiciels nécessaires

- **GCC** (GNU Compiler Collection)
- **Make**
- **Java Runtime Environment (JRE) 11 ou supérieur**
- **Système d'exploitation**: Linux (testé sur Ubuntu)

### Installation des dépendances

```bash
# Installer les outils de développement
sudo apt-get update
sudo apt-get install build-essential gcc make

# Installer Java (si non installé)
sudo apt-get install default-jre

# Vérifier les versions
gcc --version
make --version
java -version
```

## Configuration

### 1. Structure des répertoires

Assurez-vous que la bibliothèque `lib_mrpiz` est au bon emplacement (même niveau que le répertoire racine du projet).

### 2. Configuration du port

Le simulateur et l'application doivent utiliser le même port. Par défaut, le port est configuré dans le `Makefile`:

```makefile
CCFLAGS = -DINTOX -DINTOX_ADDRESS=127.0.0.1 -DINTOX_PORT=12345
```

Les ports courants sont: **12301**, **12341**, ou **12345**.

Pour vérifier le port utilisé par le simulateur:
Les ports disponibles sont affichés au lancement du .jar

## Compilation

Depuis le répertoire `RM_Final/src/`:

```bash
# Compiler le projet
make all

# Nettoyer les fichiers compilés
make clean

# Recompiler complètement
make clean all
```

L'exécutable sera généré dans `../bin/go`.

## Lancement

### Étape 1: Démarrer le simulateur Intox

Depuis le répertoire racine du projet:

```bash
java -jar intox_v0.9-mrpiz_v0.5-java_11.jar
```

Une fenêtre graphique devrait s'ouvrir affichant l'environnement de simulation du robot.

### Étape 2: Lancer l'application

Dans un **nouveau terminal**, depuis `RM_Final/src/`:

```bash
../bin/go
```

Ou avec le chemin complet:
```bash
./RM_Final/bin/go

```

## Fonctionnalités

### Menu principal

L'application offre plusieurs modes de contrôle:

```
************************************
*        Application  Robot        *
************************************
* 1. avancer (8)                   *
* 2. droite (6)                    *
* 3. gauche (4)                    *
* 4. demie tour (2)                *
* 5. Path definie (7)              *
* 6. Path definie (9)              *
* 7 - Suivi du mur droit(1)        *
* 0. Quitter                       *
************************************
```

### Modes de fonctionnement

1. **Trajectoires prédéfinies (options 1-6)**: 
   - Choisir une option (1-6)
   - Sélectionner la vitesse (1-10)
   - Le robot exécute la séquence de mouvements

2. **Mode suivi de mur (option 7)**:
   - Le robot suit automatiquement le mur à sa droite
   - Appuyer sur 't' ou 'T' pour arrêter et revenir au menu
  
3. **Suivi du mur a droite (option 1)**:
   - Permet au Robot de sortir du labirynte en suivant le mur droite

3. **Quitter (option 0)**:
   - Ferme proprement l'application

### Contrôles

- **Ctrl+C**: Arrêt d'urgence du programme
- **Touche 't'**: Arrêter le mode suivi de mur
- **Chiffres 0-9**: Sélection des options du menu

## Architecture logicielle

### États de l'application

Le programme utilise une machine à états:

- `STATE_SELECT_PATH`: Sélection du mode/chemin
- `STATE_EXECUTE_PATH`: Exécution d'une trajectoire
- `STATE_CHECK_COMPLETION`: Vérification de fin de parcours
- `STATE_FOLLOW_WALL`: Mode suivi de mur actif

### Modules principaux

- **main.c**: Boucle principale et gestion des états
- **pilot**: Contrôle bas niveau des mouvements individuels
- **copilot**: Gestion des séquences de mouvements
- **robot**: Interface avec le simulateur
- **IHM**: Interface homme-machine (affichage)
- **app_manager**: Gestion des chemins prédéfinis

## Débogage

### Mode Debug

Le projet est compilé en mode debug par défaut. Pour basculer en mode release:

Dans le `Makefile`, commenter/décommenter:
```makefile
# Mode Release (production)
# CCFLAGS += -O3 -DNDEBUG -D_FORTIFY_SOURCE

# Mode Debug (développement)
CCFLAGS += -Og -g -DDEBUG
```

### Arrêter le programme

Si le programme est bloqué:
```bash
make kill
# ou
killall -s INT go
```

### Problèmes courants

**Erreur: "Erreur lors du démarrage du simulateur de robot"**
- Vérifier que le simulateur Java est lancé
- Vérifier que le port configuré est correct
- Vérifier la connexion réseau locale (127.0.0.1)

**Erreur: "No rule to make target 'all'"**
- Vous n'êtes pas dans le bon répertoire
- Aller dans `RM_Final/src/` avant de lancer `make`

**Erreur: "Unable to access jarfile"**
- Vérifier le chemin vers le fichier `.jar`
- Le fichier s'appelle `intox_v0.9-mrpiz_v0.5-java_11.jar`

## Sécurité

Le projet compile avec des options de sécurité recommandées par l'ANSSI (Agence française de cybersécurité):

- Analyse statique: `-fanalyzer`
- Protection de la pile: `-fstack-protector-strong`
- Vérifications de format: `-Wformat=2`
- Détection de débordement: `-ftrapv`, `-fsanitize=undefined`
- Position Independent Executable: `-fPIE`

## Auteurs
- Maxime Robin (eleves)
- Francius Sylvain (eleves)
- Matthias Brun (matthias.brun@eseo.fr) - Makefile original
- Jérôme Delatour (jerome.delatour@eseo.fr) - Adaptation pour simulateur Intox

## Licence

Projet éducatif - ESEO

## Notes techniques

- Le terminal est configuré en mode non-canonique pour la saisie interactive
- Utilisation de signaux POSIX pour la gestion de Ctrl+C
- Multitâche avec pthread si nécessaire
- Communication réseau via sockets TCP/IP (127.0.0.1)

---

**Dernière mise à jour**: Janvier 2026
