/*
 Programme exemple de l'utilisation de ncurses pour le projet
 Utilise la lib  "lib_projet_ncurses.hpp" 
 Ne pas utiliser directement les fonctions de ncurses sans autorisation

compiler en ajoutant -lncurses  à la commande de compilation
*/
#include <iostream>
#include <string>

#include "lib_projet_ncurses.hpp"

// -------------------
// ----- COULEURS ----
// -------------------

// -----  constantes couleurs ------
// ----- indices qui doivent commencer à 1 ----
const int COULEURS_PAR_DEFAUT = 1;
const int ROUGE_SUR_NOIR = 2;
const int NOIR_SUR_ROUGE = 3;
const int JAUNE_SUR_NOIR = 4;
const int NOIR_SUR_JAUNE = 5;
const int BLEU_SUR_NOIR = 6;
const int NOIR_SUR_BLEU = 7;
const int VERT_SUR_NOIR = 8;
const int NOIR_SUR_VERT = 9;

// tableau des couleurs pour chacune des paires de couleurs , à fournir à l'initialisation
// de la console
const PaireCouleur couleurs[] = {
 {COLOR_WHITE, COLOR_BLACK},   // COULEURS_PAR_DEFAUT = 1;
 {COLOR_RED, COLOR_BLACK},     // ROUGE_SUR_NOIR = 2;
 {COLOR_BLACK, COLOR_RED},     // NOIR_SUR_ROUGE = 3;
 {COLOR_YELLOW, COLOR_BLACK},  // JAUNE_SUR_NOIR = 4;
 {COLOR_BLACK, COLOR_YELLOW},  // NOIR_SUR_JAUNE = 5;
 {COLOR_BLUE, COLOR_BLACK},    // BLEU_SUR_NOIR = 6; 
 {COLOR_BLACK, COLOR_BLUE},    // NOIR_SUR_BLEU = 7; 
 {COLOR_GREEN, COLOR_BLACK},   // VERT_SUR_NOIR = 8;
 {COLOR_BLACK, COLOR_GREEN}    // NOIR_SUR_VERT = 9;
}; 

// -------------------
// ---- procédures utilitaires 
// ---- pourraient être rangées dans un fichier .hpp distinct
// -------------------
// Il manque les rôles et les préconditions

void dessiner_ligne(int longueur, int y, char chr) {
    for (int x = 0; x < longueur; x++) {
        ecrire_char(x, y, chr);
    }
}



// -------------------
//----  Fonctionnalites  -----
// -------------------

//-----------  Menu 

void afficher_instructions() {
    changer_couleur(NOIR_SUR_VERT);
    ecrire_string("Appuyez sur s et d pour des lignes", 0, 0);
    ecrire_string("Appuyez sur z pour des diagonales", 0, 1);
    ecrire_string("Appuyez sur e pour afficher des caracteres aleatoires (k pour quitter)", 0, 2);
    ecrire_string("Appuyez sur a pour quitter", 0, 3);
    couleur_defaut();
}

void alterner_lignes(int indice_couleur_paire, int indice_couleur_impaire, std::string message) {
    for (int y = 0; y < CONSOLE_Y_SIZE; y++) {
        if (y % 2 == 0) {
            changer_couleur(indice_couleur_paire);
        } else {
            changer_couleur(indice_couleur_impaire);
        }
        dessiner_ligne(CONSOLE_X_SIZE, y, '#');
    }

    size_t taille_message = message.length();
    int x = (CONSOLE_X_SIZE / 2) - int(taille_message / 2);
    int y = CONSOLE_Y_SIZE / 2;
    ecrire_string(message, x, y);
}


void diagonales() {
    int pas = 8;
    for (int y = 0; y < CONSOLE_Y_SIZE; y++) {
        for (int x = 0; x < CONSOLE_X_SIZE; x++) {
            if (y > 0 && (x+y) % (pas - 1) == 0) {
                changer_couleur(NOIR_SUR_JAUNE);
                ecrire_char(x, y, ' ');
            }
            else if(x % pas == y % pas) {
                changer_couleur(NOIR_SUR_BLEU);
                ecrire_char(x, y, ' ');
            }            
        }
    }
}

void affichage_aleatoire() {
    for (int y = 0; y < CONSOLE_Y_SIZE; y++) {
        for (int x = 0; x < CONSOLE_X_SIZE; x++) {
            changer_couleur(rand() % 9 +1);
            char val = char(rand() % ('z' - 'A') + 'A');        
            ecrire_char(x, y, val);
        }
    }
    couleur_defaut();
}

void aleatoire(){
  int input='-';
  while(input != 'k') {
     input = saisie_non_bloquante();
     // Affiche des caractères aléatoires à l'écran (voir fonctions_exemples.hpp)
     affichage_aleatoire();
     afficher_instructions();
     attendre_millisecondes(500);
  }

  effacer_console();
}

// ----------------------------
// ------ALGO PRINCIPAL -------
// ----------------------------



int main() {
    // ----- Initialisation de la console -----
    // Si l'initialisation se passe mal, on arrête le programme (voir fonctions_aide.cpp)
    if (!initialiser_console(couleurs, 9)) {
        std::cout << "ERREUR: impossible d'initialiser la console." << std::endl;
        return 1;
    }
    // ----- Fin de l'initialisation de la console -----

    // Variable stockant le caractère tapé par l'utilisateur
    int input = ',';

    afficher_instructions();

    while (input != 'a' &&  input != KEY_LEFT &&  input != KEY_RIGHT &&  input != KEY_UP  &&  input != KEY_DOWN) {
        // Attend le prochain caractère (-> touche appuyée)
        input = saisie_bloquante();

        // Effacer la console avant de réafficher le contenu
        effacer_console();

        if (input == 's') {
            // Affiche des lignes à lécran, alternant entre fond noir et caractère rouge,
            // et fond bleu et caractère noir (voir fonctions_exemples.cpp)
            alterner_lignes(NOIR_SUR_BLEU, ROUGE_SUR_NOIR, "Appuyez sur d pour alterner");
        }
        else if (input == 'd') {
            // Affiche des lignes à lécran, alternant entre fond noir et caractère rouge,
            // et fond bleu et caractère noir (voir fonctions_exemples.cpp)
            alterner_lignes(BLEU_SUR_NOIR, NOIR_SUR_ROUGE, "Appuyez sur s pour alterner");
        }
        else if (input == 'z') {
            // Affiche des diagonales à l'écran (voir fonctions_exemples.cpp)
            diagonales();
        }
        else if (input == 'e') {
            aleatoire();  
        }

        afficher_instructions();
    }

    // ----- Fermeture de la console -----
    fermer_console();  
    // ----- Fin de la fermeture de la console -----
    return 0;
}
