/*
    V0.1 Projet Rogue-like
    Createurs : Evan, Vincent, Celestin, Clementine
    Role : Test d'un début de jeu 
*/

#include <iostream>
#include <string>
#include <fstream>
#include "lib_projet_ncurses.hpp"

using namespace std;

struct Personnage{
    string Nom;
    int Vie,Degat,Niveau,xp,Vision,Armure;
};


// ----- Constantes Couleurs -----
const int COULEURS_PAR_DEFAUT = 1;
const int ROUGE_SUR_NOIR = 2;
const int NOIR_SUR_ROUGE = 3;
const int JAUNE_SUR_NOIR = 4;
const int NOIR_SUR_JAUNE = 5;
const int BLEU_SUR_NOIR = 6;
const int NOIR_SUR_BLEU = 7;
const int VERT_SUR_NOIR = 8;
const int NOIR_SUR_VERT = 9;

// Tableau de configuration des couleurs
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

// ----- Fonctions Utilitaires -----

// Fonction pour lire le fichier carte
int lire_carte(string nom_fic) {
    fstream flux;
    flux.open(nom_fic, ios::in);

    if (flux.is_open()) {
        ecrire_string("Succes : La carte est ouverte.", 0, 6);
        flux.close(); 
        return 1;     
    } else {
        ecrire_string("Erreur : Impossible d'ouvrir la carte.", 0, 6);
        return 0;     
    }
}

void DessinerMap(int MaxX, int MaxY, string nom_fichier){
    int x = 0;
    while (x<MaxX){
        cout << x;
        x = x+1;
    } 


}


// Fonction pour dessiner une ligne horizontale
void dessiner_ligne(int longueur, int y, char chr) {
    for (int x = 0; x < longueur; x++) {
        ecrire_char(x, y, chr);
    }
}




// Fonction pour afficher le menu principal
void afficher_instructions() {
    ecrire_string("[p] : Jouer", 0, 1);
    ecrire_string("[a] : Quitter", 0, 2);
    couleur_defaut();
}

// ----- Main -----

int main() {
    // 1. Initialisation de la console ncurses
    if (!initialiser_console(couleurs, 9)) {
        std::cout << "ERREUR: impossible d'initialiser la console." << std::endl;
        return 1;
    }

    //variable utilisé 
    string nom_fichier = "maptest.txt";
    int input = 0; // Stocke la touche appuyée

    //affiche le menu de base
    afficher_instructions();

    // boucle du jeu, si on appuie sur A ça coupe le jeu (Pour l'instant)
    while (input != 'a') {
        
        // On attend une touche utilisateur
        input = saisie_bloquante();

        // on efface et on reécrit a chaque fois pour ne pas se perdre
        effacer_console();
        
        if (input == 'p') { 
            // Lancement du jeu
            ecrire_string("Partie lancé", 0, 5);
            lire_carte(nom_fichier);
            DessinerMap(8,10,nom_fichier);
        } 
        else {
            //sinon on réaffiche comme a chaque fois
            afficher_instructions();
        }
    }

    //ferme la console pour pouvoir continuer avec la console.
    fermer_console();  
    return 0;
}
