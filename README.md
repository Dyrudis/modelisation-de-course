# Modélisation de Course à Pied
Projet de modélisation d'une épreuve de course à pied.

# Fonctionnement
Le projet se décompose en 3 parties.

1. Générateur de fichier :
- Description :
Cette partie permet de générer un fichier contenant toutes les informations de 100 coureurs et d'un parcours. Pour déterminer les noms des coureurs, le programme pioche des noms au hasard dans le fichier noms.txt. Après avoir lancé cet exécutable, il faut copier le fichier "file.json" dans le répertoire de la deuxième partie.
- Fichier(s) d'entrée  :
  - noms.txt  (contient une liste d'au moins 100 noms)
- Fichier(s) de sortie :
  - file.json (contient les informations de 100 coureurs et d'un parcours)

2. Course :  
- Description :
Cette partie effectue tous les calculs de distance de chaque coureur pour chaque seconde. Toutes les informations sont alors écrites dans le fichier "result.json". Ce fichier doit également être copié dans le répertoire de la troisième partie pour continuer.
- Fichier(s) d'entrée  :
  - file.json  (fichier à récupérer après exécution de la partie 1)
- Fichier(s) de sortie :
  - result.json (contient les performances des 100 coureurs)

3. Affichage :
- Description :
Cette partie lit le fichier "result.json" puis lance une fenêtre SFML affichant les distances de chaque coureur ainsi qu'un lecteur. Grâce au lecteur, il est possible d'accélérer ou ralentir le temps (avec les touches + et - du pavé numérique), d'avancer ou reculer d'une heure, ou encore de faire pause/play.
- Fichier(s) d'entrée  :
  - results.json (fichier à récupérer après exécution de la partie 2)
  - ressources (dossier contenant 9 images .png et 3 polices de caractères .ttf)
- Fichier(s) de sortie :
  - aucun

# Installation
Pour installer le projet, il suffit de télécharger les 3 dossiers (Générateur de fichier, Course et Affichage) et de les ouvrir dans Visual Studio.

# Compilation
La compilation des parties 1, 2 et 3 se fait dans Visual Studio.
Pour la 3ème partie (Affichage), il faut installer la bonne version de SFML, voici les explications :

La bibliothèque SFML doit être installée sur le disque 'C' comme ceci : "C:/SFML"

Le dossier d'include doit donc être :     "C:\SFML\include"

Et le dossier de bibliothèque doit être : "C:\SFML\lib"

La version utilisée est "SFML 2.5.1 Visual C++ 15 (2017) - 32-bit". Lien de téléchargement (Première version en haut à gauche) : https://www.sfml-dev.org/download/sfml/2.5.1/index-fr.php

La version fournie sur Teams posait des erreurs de link (incompatibilité x86 et x64). Avec cette version x86, le problème ne devrait pas avoir lieu.

Voici la fiche que j'ai suivie pour installer SFML : 
http://geofhagopian.net/CS007B/CS7B-F19/SFML%20Setup%20in%20Visual%20Studio%202019.pdf

Si vous avez des questions, n'hésitez pas à me les envoyer (par mail ou par Teams).
