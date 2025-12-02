/*
    V0.3 Projet Rogue-like
    Vincent Clementine Evan Celestin
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include "lib_projet_ncurses.hpp"

using namespace std;

// la taille max du tableau
const int MAX_LIGNES = 30;    // Hauteur max de la carte Pour utiliser la carte
const int MAX_COLONNES = 100; // Largeur //                        //

struct Personnage {
    string Nom;
    int x, y; 
    int Vie, Degat, Niveau, xp, Vision, Armure;
};

struct Monstre {
    string Nom, typeIA;
    int x, y;
    int Vie, Degat, xp, Armure, Vitesse;
};

struct Carte {
    int xMax, yMax;
    char carte[1000][1000];
};

// Prototype de l'écran de défaite
// Retourne 'r' pour rejouer, ne retourne pas en cas de quit (le programme quitte)
char perdu();

// Les couleurs si besoin
const PaireCouleur couleurs[] = {
    {COLOR_WHITE, COLOR_BLACK},  // 1
    {COLOR_RED, COLOR_BLACK},    // 2
    {COLOR_BLACK, COLOR_RED},    // 3
    {COLOR_YELLOW, COLOR_BLACK}, // 4
    {COLOR_BLACK, COLOR_YELLOW}, // 5
    {COLOR_BLUE, COLOR_BLACK},   // 6 
    {COLOR_BLACK, COLOR_BLUE},   // 7 
    {COLOR_GREEN, COLOR_BLACK},  // 8
    {COLOR_BLACK, COLOR_GREEN}   // 9
}; 

// Fonction de déplacement (qui fonctionne les noobs !!)

// Fonction qui verifie SI ET SEULEMENT SIIIII on a le droit d'aller sur la case (x,y)
// Elle renvoie TRUE si c'est bon, FALSE si c'est un mur ou hors map (logique lol)
bool test_collision(int x, int y, const Carte &c) {
    
    // n verifie qu'on sort pas du tableau (au cas ouu)
    if (x < 0 || y < 0 || y >= c.yMax || x >= c.xMax) {
        return false; // C'est interdit
    }

    // On verifie si c'est un mur
    if (c.carte[y][x] == '#') {
        return false; // C'est un mur, interdit(logique ma gueule !)
    }

    // Les interactions spéciales (portes, sorties, etc.) seront gérées ailleurs

    // Si on arrive ici, c'est que la voie est libre(en mode splinter cell)
    return true;
}

// Fonction qui calcule le mouvement et déplace le héros si possible
// IMPORTANT : On met "Personnage &p" avec un & pour modifier le vrai héros parce que sinon azy ça marche pas (j'y ai passé 30 minutes ...ahah)
void gerer_deplacement(Personnage &p, int input, const Carte &c) {
    
    int futurX = p.x; //on modifie la pos x 
    int futurY = p.y; // idem avec y et j'appelle ca futur parce que POURQUOI PAS 

    // On regarde quelle touche a été appuyée
    if (input == 'z') futurY--; // Haut
    else if (input == 's') futurY++; // Bas
    else if (input == 'q') futurX--; // Gauche
    else if (input == 'd') futurX++; // Droite
    else return; // Si c'est une autre touche, on fait rien

    // On demande à la fonction de verif si on peut y aller
    if (test_collision(futurX, futurY, c) == true) {
        // C'est validé, on met à jour le joueur
        p.x = futurX;
        p.y = futurY;
    }
}



// Les fonctions de la carte

void charger_carte(string nom_fichier, Carte &c) { // le nom du fichier, et notre enregistrement Carte
    ifstream file(nom_fichier); // on ouvre le fichier
    string ligne; // variable pour chaque ligne
    
    // Initialisation : On remplit tout de vide pour éviterles bugs d'affichage
    for(int i=0; i<MAX_LIGNES; i=i+1) {  // une boucle qui commence a la ligne 0, jusqu'a avoir fait toute les lignes
        for(int j=0; j<MAX_COLONNES; j=j+1) { // La on fait toute les colonnes de chaque ligne 
            c.carte[i][j] = ' '; // on est obligé de faire ça sinon on se retrouve avec des valeurs au pif dans le tableau
        }
    }
    
    c.yMax = 0; // hauteur réelle de la carte
    c.xMax = 0; // largeur réelle max rencontrée

    if (file.is_open()) { // si le fichier est ouvert
        // Lecture ligne par ligne
        while (getline(file, ligne) && c.yMax < MAX_LIGNES) { //&& ca veut dire Et, on lit ligne par ligne, et si on a finit on arrete
            // On copie chaque caractère de la ligne dans le tableau
            int largeur_ligne = (int)ligne.length();
            if (largeur_ligne > MAX_COLONNES) largeur_ligne = MAX_COLONNES;
            for (int x = 0; x < largeur_ligne; x++) { // et la on fait colonne par colonne ! 
                c.carte[c.yMax][x] = ligne[x]; // et on met dans le tableau la valeur de la map ! 
            }
            if (largeur_ligne > c.xMax) c.xMax = largeur_ligne;
            c.yMax = c.yMax+1;
        }
        file.close();
    } else {
        ecrire_string("ERREUR: Carte introuvable !", 0, 0);// tu as vu en mode vrai dev
    }
}

void Combat(Personnage &perso, Monstre &monstre, Carte &c){
    char reponse;
    bool attaque;

    attaque = true;
    ecrire_string("Choisissez une option : Attaquer ou defendre ? (A/D)", c.yMax+15, 15);
    reponse = saisie_bloquante();
    while (perso.Vie > 0 and monstre.Vie > 0){
        if (reponse == 'A' and attaque){
            monstre.Vie -= perso.Degat*2;
            attaque = false;
        }
        else{
            if (reponse == 'D'){
                perso.Vie -= monstre.Degat*0.75;
            }
            else {
                perso.Vie -= monstre.Degat;
            };
        }
        if (monstre.Vie > 0){
            perso.Vie -= monstre.Degat;
        }
    }
    if (monstre.Vie <= 0){
        perso.xp += monstre.xp;
    }
    else {
        // Laisse le joueur choisir de rejouer ou quitter
        char choix = perdu();
        if (choix == 'r') {
            // On sort du combat et on laisse le flux d'exécution reprendre.
            return;
        }
    }
}

// Fonction pour afficher le contenu du tableau à l'écran
void afficher_carte(const Carte &c) {
    for (int y = 0; y < c.yMax; y=y+1) {
        for (int x = 0; x < c.xMax; x=x+1) {
            char ch = c.carte[y][x];
            // On affiche seulement si ce n'est pas du vide (optimisation)
            if (ch != ' ' && ch != '\0') {
                ecrire_char(x, y, ch);
            }
        }
    }
}

//LE menu pour commencer

void afficher_instructions() {
    ecrire_string ("ROGUE LIKE V0.3 , Vincent, Clem, Evan, Celestin", 20, 10);
    ecrire_string("[p] : Jouer", 20, 12);
    ecrire_string("[a] : Quitter", 20, 13);
}

// Sous-algorithme de défaite
// Propose de rejouer ou quitter. Retourne 'r' si rejouer est choisi; en cas de
// choix de quitter, la fonction ferme la console et termine le programme.
char perdu() {
    while (true) {
        effacer_console();
        ecrire_string("Vous avez perdu...", 20, 10);
        ecrire_string("[r] Rejouer", 20, 12);
        ecrire_string("[a] Quitter", 20, 13);
        int in = saisie_bloquante();
        if (in == 'r' || in == 'R') {
            return 'r';
        }
        if (in == 'a' || in == 'A') {
            fermer_console();
            exit(0);
        }
        // sinon, boucle et redemande
    }
}

//main

int main() {
    // Initialisation
    if (!initialiser_console(couleurs, 9)) {
        cout << "Erreur console" << endl;
        return 1;
    }

    // Variables du Jeu 
    string nom_fichier = "maptest.txt";
    Carte carte; // La carte du jeu via l'enregistrement Carte
    bool carte_chargee = false; //savoir si on a reussi a charger la map
    bool jeu_lance = false; // Pour savoir si on est dans le menu ou en jeu comme ça on peut différencié quand on se déplace

    // Le joueurs 
    Personnage heros; // on utilise l'enregistrement Perso qu'on appelle hero
    heros.x = 2; // Position de départ X
    heros.y = 2; // Position de départ Y
    heros.Nom = "Aventurier"; // on lui donne un nom (pas sur de l'idée on s'en fou je pense pour l'instant mais azy il est 23 heures j'en peu plus)

    int input = 0; // en gros a la place d'utiliser saisie bloquante je fais ça pour garder ce qu'on appuie

    //  boucle Principale 
    while (input != 'a') { // 'a' pour quitter totalement
        
        //Effacer l'écran précédent
        effacer_console();

        // gestion de différence quand on clique sur p 
        if (!jeu_lance) {
            // affiche le truc de base
            afficher_instructions();
            
            // On attend une touche
            input = saisie_bloquante();

            if (input == 'p') {
                jeu_lance = true; // on va en mode jeu
                // On charge la carte une seule fois au lancement
                if (!carte_chargee) { // on charge si on a pas chargé
                    charger_carte(nom_fichier, carte);
                    carte_chargee = true;
                }
            }
        } 
        else {
            // Mode jeu
            
            // Affichage
            afficher_carte(carte); // Le décor
            ecrire_char(heros.x, heros.y, '@');   // Le joueur (et boom j'ai reussi ma gueule)
            
            // Attente Joueur
            input = saisie_bloquante();

            // On lui donne le héros, la touche appuyée, et la carte
            gerer_deplacement(heros, input, carte);
            
        }
    }

    fermer_console();  
    return 0;
}