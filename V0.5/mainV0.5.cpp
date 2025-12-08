
/*
    V0.5 Projet Rogue-like
    Vincent Clementine Evan Celestin
*/



#include <iostream>
#include <string>
#include <fstream> 
#include <cmath>   
#include "lib_projet_ncurses.hpp" 


using namespace std;


// Definition des couleurs
// Tableau de configuration des paires de couleurs (Texte, Fond)
const PaireCouleur couleurs[] = {
    {COLOR_WHITE, COLOR_BLACK},  // 1 : Texte blanc, fond noir
    {COLOR_RED, COLOR_BLACK},    // 2 : Texte rouge, fond noir
    {COLOR_BLACK, COLOR_RED},    // 3 : Texte noir, fond rouge
    {COLOR_YELLOW, COLOR_BLACK}, // 4 : Texte jaune, fond noir
    {COLOR_BLACK, COLOR_YELLOW}, // 5 : Texte noir, fond jaune
    {COLOR_BLUE, COLOR_BLACK},   // 6 : Texte bleu, fond noir
    {COLOR_BLACK, COLOR_BLUE},   // 7 : Texte noir, fond bleu
    {COLOR_GREEN, COLOR_BLACK},  // 8 : Texte vert, fond noir
    {COLOR_BLACK, COLOR_GREEN},   // 9 : Texte noir, fond vert
}; 


// Constantes pour appeler facilement les couleur dans le code
const int COULEURS_PAR_DEFAUT = 1;
const int ROUGE_SUR_NOIR = 2;
const int NOIR_SUR_ROUGE = 3;
const int JAUNE_SUR_NOIR = 4;
const int NOIR_SUR_JAUNE = 5;
const int BLEU_SUR_NOIR = 6;
const int NOIR_SUR_BLEU = 7;
const int VERT_SUR_NOIR = 8;
const int NOIR_SUR_VERT = 9;



// Les constantes globaux du jeu pour pouvoir les modifier facilement.
const int MAX_LIGNES = 30;    // Hauteur max de la carte
const int MAX_COLONNES = 100; // Largeur max de la carte
const int NB_MONSTRES_MAX = 50; // Capacité maximale du tableau de monstres


// variable gloabl
bool OnEstDejaPasseIci[MAX_LIGNES][MAX_COLONNES]; // savoir si on a déja vu ce qu'il y a en face de nous




// Structure du personnage
struct Personnage {
    string Nom;
    int x, y; // Position sur la carte
    int Vie, Degat, Niveau, xp, Vision, Armure;
    int xpPourNiveau; // nombre d'xp qu'il faut pour gagner un niveau
    bool cle; // Inventaire(un peu nul) : possède la clé ou non
};


// Structure des monstres
struct Monstre{
    string Nom;
    char Symbole; // Caractère affiché ('T', 'D')
    int x, y; // Position du monstre
    int Vie, Degat, xpdonne, Armure, vitesse;
    int Vision; // Distance de détection du joueur
};




// Déclaration des fonctions avant leur utilisation pour que le compilateur les connaisse ça evite de devoir mettre les fonctions plus haut ou plus bas
void Combat(Personnage &p, Monstre &m, bool &jeu_lance);
bool test_collision(int x, int y, char carte[MAX_LIGNES][MAX_COLONNES], Personnage &p, int hauteur_reelle, Monstre monstres[], int nb_monstres, bool &jeu_lance);
void gerer_deplacement(Personnage &p, int hauteur_reelle, int input, char carte[MAX_LIGNES][MAX_COLONNES], Monstre monstres[], int nb_monstres, bool &jeu_lance);
void charger_carte(string nom_fichier, char carte[MAX_LIGNES][MAX_COLONNES], int &hauteur_reelle, Monstre monstres[], int &nb_monstres_trouves);
void deplacer_tous_les_monstres(Monstre monstres[], int nb_monstres, Personnage p, char carte[MAX_LIGNES][MAX_COLONNES]);





/*
    Rôle        : Initialise toutes les statistiques du héros. (pour commencer et en cas de mort)
    Entrée      : h (Personnage) passé par référence.
    sortie      : aucune
    précondition: aucune aussi
*/

void initialiser_heros(Personnage &h) {
    h.Nom = "Aventurier";
    h.x = 2; 
    h.y = 2;
    h.Vie = 100;
    h.Vision = 2;
    h.xp = 0;
    h.Niveau = 1;
    h.xpPourNiveau = 20;
    h.cle = false;
    h.Armure = 2;
    h.Degat = 5;
}

// Vérifie si le joueur a assez d'XP pour monter de niveau
void GagnerNiveau(Personnage &p){
    if (p.xp >= p.xpPourNiveau){
        p.xp = p.xp - p.xpPourNiveau; 
        p.xpPourNiveau = p.xpPourNiveau * 1.5; 
        p.Niveau = p.Niveau + 1;
        p.Degat = p.Degat + 2; // Bonus de dégâts
        p.Vie = p.Vie + 5;   //plus de vie
    }
}

// Affiche les informations du joueur en bas 
void afficher_hero(Personnage heros, int hauteur_reelle){
    // Affichage de la Vie
    changer_couleur(ROUGE_SUR_NOIR);
    ecrire_string ("Vie du perso :", 0, hauteur_reelle+5);
    ecrire_string (std::to_string(heros.Vie) + "  ", 15, hauteur_reelle+5);
    
    // Affichage du Niveau
    changer_couleur (BLEU_SUR_NOIR);
    ecrire_string ("niveau : ", 0, hauteur_reelle+6);
    ecrire_string (std::to_string(heros.Niveau), 15, hauteur_reelle+6);
    
    // Affichage de l'XP
    changer_couleur (VERT_SUR_NOIR);
    ecrire_string ("Xp : ", 0, hauteur_reelle+7);
    ecrire_string (std::to_string(heros.xp) + "/" + std::to_string(heros.xpPourNiveau), 15, hauteur_reelle+7);
    
    // Affichage de l'inventaire (Clé)
    changer_couleur (NOIR_SUR_ROUGE);
    if (heros.cle == true){
        ecrire_string ("tu as une clé", 0, hauteur_reelle+8);
    } else {
        changer_couleur (COULEURS_PAR_DEFAUT);
        ecrire_string ("             ", 0, hauteur_reelle+8); 

    }
    changer_couleur (VERT_SUR_NOIR);
    ecrire_string ("Dégat du hero :", 0, hauteur_reelle+9);
    ecrire_string (std::to_string(heros.Degat), 16, hauteur_reelle+9);

    changer_couleur (VERT_SUR_NOIR);
    ecrire_string ("Vision : ", 0, hauteur_reelle+10);
    ecrire_string (std::to_string(heros.Vision), 15, hauteur_reelle+10);
}


void Combat(Personnage &p, Monstre &m, bool &jeu_lance){
    p.Vie = p.Vie - m.Degat; // Le joueur prend des dégâts
    m.Vie = m.Vie - p.Degat; // Le monstre prend des dégâts

    // Vérification de la mort du joueur
    if (p.Vie <= 0){    
        jeu_lance = false; // Arrête la boucle de jeu
        p.Vie = 0; 
    }
}

// Déplcaement des monstres.
void deplacer_tous_les_monstres(Monstre monstres[], int nb_monstres, Personnage p, char carte[MAX_LIGNES][MAX_COLONNES]) {
    
    // On parcourt tout le tableau de monstres
    for (int i = 0; i < nb_monstres; i++) {
        // Si le monstre est mort ou est un Tank (immobile), on passe au suivant
        if (monstres[i].Vie <= 0) continue; // en gros si il est mort on arrete (continue permet de s'arreter, en mode break en python)
        if (monstres[i].Nom == "Tank") continue; // de même si déja vu

        // Calcul de la distance entre le monstre et le joueur
        int diffX = p.x - monstres[i].x;
        int diffY = p.y - monstres[i].y;

        // Si le joueur est hors de la zone de vision, le monstre ne bouge pas
        if (abs(diffX) > monstres[i].Vision || abs(diffY) > monstres[i].Vision) { //j'avoue j'ai demandé a chat gpt j'en pouvais plus de ne pas trouver ... sorry
            continue; 
        }

        // La direction ou il doit aller 
        int directionX = 0;
        if (diffX > 0) directionX = 1; else if (diffX < 0) directionX = -1;

        int directionY = 0;
        if (diffY > 0) directionY = 1; else if (diffY < 0) directionY = -1;

        // Coordonnées cibles potentielles
        int futurX = monstres[i].x + directionX;
        int futurY = monstres[i].y + directionY;

        // Détection des obstacles (Murs)
        bool murEnX = (carte[monstres[i].y][futurX] == '#');
        bool murEnY = (carte[futurY][monstres[i].x] == '#');

        // Détection du joueur (pour ne pas marcher sur lui)
        bool joueurEnX = (futurX == p.x && monstres[i].y == p.y);
        bool joueurEnY = (monstres[i].x == p.x && futurY == p.y);

        
        // Si Le joueur est plus loin horizontalement (Priorité X)
        if (abs(diffX) > abs(diffY)) {
            // Essai déplacement X
            if (!murEnX && !joueurEnX) {
                monstres[i].x = futurX;
            }
            // Si X bloqué, essai contournement par Y
            else if (directionY != 0 && !murEnY && !joueurEnY) {
                monstres[i].y = futurY;
            }
        } 
        // Si Le joueur est plus loin verticalement (Priorité Y)
        else {
            // Essai déplacement Y
            if (!murEnY && !joueurEnY) {
                monstres[i].y = futurY;
            }
            // Si Y bloqué, essai contournement par X
            else if (directionX != 0 && !murEnX && !joueurEnX) {
                monstres[i].x = futurX;
            }
        }
    }
}

// Fonction de déplacement 
// Vérifie si le joueur PEUT aller sur la case (x,y) et gère les interactions
bool test_collision(int x, int y, char carte[MAX_LIGNES][MAX_COLONNES], Personnage &p, int hauteur_reelle, Monstre monstres[], int nb_monstres, bool &jeu_lance) {
    
    // Vérifie qu'on ne sort pas du tableau
    if (x < 0 || x >= MAX_COLONNES || y < 0 || y >= MAX_LIGNES) {
        return false; // C'est interdit
    }

    // Vérifie si c'est un mur
    if (carte[y][x] == '#') {
        return false; // C'est un mur, interdit(logique ma gueule !)
    }
    
    // Interaction : Potion
    if (carte[y][x] == 'p'){ 
        p.Vie = p.Vie + 5;
        carte[y][x] = '.'; // On retire l'objet
        afficher_hero(p, hauteur_reelle);
        return true;
    }
    
    // Interaction : Sortie de niveau / Fin de partie
    // Si le joueur marche sur 'H', on termine immédiatement la partie
    if (carte[y][x] == 'H'){
        jeu_lance = false; // Retour au menu principal
        return true;       // Autorise le déplacement final sur la case
    }
    
    // Interaction : Clé
    if (carte[y][x] == 'k'){
        p.cle = true;
        carte[y][x] = '.';
        afficher_hero(p, hauteur_reelle);
        return true;
    }
    
    // Interaction : Porte
    if (carte[y][x] == 'O'){
        if (p.cle == true){
            carte[y][x] = '.';
            p.cle = false;
            afficher_hero(p, hauteur_reelle);
            return true;
        }
        // Pas de clé = Mur
        return false;
    }
    
    for(int i=0; i < nb_monstres; i++) {
        // Si la case cible contient un monstre VIVANT
        if (x == monstres[i].x && y == monstres[i].y && monstres[i].Vie > 0) {
            
            // On lance le combat
            Combat(p, monstres[i], jeu_lance);
            
            // Affichage vie du monstre attaqué
            changer_couleur(ROUGE_SUR_NOIR);
            string msg = monstres[i].Nom + ": " + to_string(monstres[i].Vie) + "PV   ";
            ecrire_string(msg, 20, hauteur_reelle + 9);

            // Si le monstre meurt
            if (monstres[i].Vie <= 0) {
                p.xp = p.xp + monstres[i].xpdonne;
                GagnerNiveau(p);
                
                changer_couleur(VERT_SUR_NOIR);
                ecrire_string(monstres[i].Nom + " vaincu !     ", 20, hauteur_reelle + 9);
            }
            return false; // Le joueur ne bouge pas, il reste devant le monstre
        }
    }

    // Si on arrive ici, c'est que la voie est libre
    return true;
}


// Fonction qui calcule le mouvement du joueur selon la touche appuyée
void gerer_deplacement(Personnage &p, int hauteur_reelle, int input, char carte[MAX_LIGNES][MAX_COLONNES], Monstre monstres[], int nb_monstres, bool &jeu_lance) {    

    int futurX = p.x; 
    int futurY = p.y; 

    // Détermine la case visée
    if (input == 'z') futurY--; // Haut
    else if (input == 's') futurY++; // Bas
    else if (input == 'q') futurX--; // Gauche
    else if (input == 'd') futurX++; // Droite
    else return; // Autre touche ignorée

    // Vérifie la validité du mouvement via test_collision
    if (test_collision(futurX, futurY, carte, p, hauteur_reelle, monstres, nb_monstres, jeu_lance) == true) {
        // C'est validé, on met à jour le joueur
        p.x = futurX;
        p.y = futurY;
    }
}


void BrouillardDeGuerre(Personnage heros,int hauteur_carte){
    int rayon = heros.Vision;
    for (int y = heros.y - rayon; y <= heros.y + rayon; y=y+1){
        for (int x = heros.x - rayon; x <= heros.x + rayon; x=x+1){
            if (x >=0 && x < MAX_COLONNES && y >=0 && y < hauteur_carte){
                if (abs(heros.x - x) + abs(heros.y - y) <= rayon+1){
                    OnEstDejaPasseIci[y][x] = true;
                }
            }
        }
    }
}





// Charge la carte depuis le fichier texte et initialise les monstres
void charger_carte(string nom_fichier, char carte[MAX_LIGNES][MAX_COLONNES], int &hauteur_reelle, Monstre monstres[], int &nb_monstres_trouves) { 
    ifstream file(nom_fichier); // Ouverture du fichier
    string ligne; 
    
    // Nettoyage du tableau carte
    for(int i=0; i<MAX_LIGNES; i=i+1) {  
        for(int j=0; j<MAX_COLONNES; j=j+1) { 
            carte[i][j] = ' '; 
            OnEstDejaPasseIci[i][j] = false;
        }
    }
    
    hauteur_reelle = 0; 
    nb_monstres_trouves = 0; // Réinitialisation du compteur de monstres

    if (file.is_open()) { 
        while (getline(file, ligne) && hauteur_reelle < MAX_LIGNES) { 
            for (int x = 0; x < ligne.length() && x < MAX_COLONNES; x++) { 
                char c = ligne[x];
                
                // On créer des monstres quand on les detextes sur la carte
                
                // TANK 
                if(c == 'T'){
                    if (nb_monstres_trouves < NB_MONSTRES_MAX) {
                        monstres[nb_monstres_trouves].Nom = "Tank";
                        monstres[nb_monstres_trouves].Symbole = 'T';
                        monstres[nb_monstres_trouves].Vie = 30;
                        monstres[nb_monstres_trouves].Degat = 5;
                        monstres[nb_monstres_trouves].xpdonne = 50;
                        monstres[nb_monstres_trouves].x = x;
                        monstres[nb_monstres_trouves].y = hauteur_reelle;
                        monstres[nb_monstres_trouves].Vision = 0; // Aveugle (en fait il bouge pas mais vu que je suis un flemard je m'embetes pas a faire une autre ia ! )

                        nb_monstres_trouves = nb_monstres_trouves +1;
                        carte[hauteur_reelle][x] = '.'; // On remplace par du sol
                    }
                }
                // DRACKSON
                else if(c == 'D'){
                    if (nb_monstres_trouves < NB_MONSTRES_MAX) {
                        monstres[nb_monstres_trouves].Nom = "Drackson";
                        monstres[nb_monstres_trouves].Symbole = 'D';
                        monstres[nb_monstres_trouves].Vie = 10;
                        monstres[nb_monstres_trouves].Degat = 2;
                        monstres[nb_monstres_trouves].xpdonne = 15;
                        monstres[nb_monstres_trouves].x = x;
                        monstres[nb_monstres_trouves].y = hauteur_reelle;
                        monstres[nb_monstres_trouves].Vision = 5; // Vision moyenne (5 cases)

                        nb_monstres_trouves = nb_monstres_trouves +1;
                        carte[hauteur_reelle][x] = '.'; 
                    }
                }
                //Autre (Mur, Sol, Item)
                else {
                     carte[hauteur_reelle][x] = ligne[x]; 
                }
            }
            hauteur_reelle = hauteur_reelle+1;
        }
        file.close();
    } else {
        ecrire_string("ERREUR: Carte introuvable !", 0, 0);
    }
}


// Affiche La carte
void afficher_carte(char carte[MAX_LIGNES][MAX_COLONNES], int hauteur_reelle,Personnage heros) {
    BrouillardDeGuerre(heros, hauteur_reelle);
    for (int y = 0; y < hauteur_reelle; y=y+1) {
        for (int x = 0; x < MAX_COLONNES; x=x+1) {
            
            bool visible_maintenant = false;
            if (abs(heros.x - x)+abs(heros.y - y)<= heros.Vision +1){
                visible_maintenant = true;
            }
            
            char c = carte[y][x];
            if (visible_maintenant){
                if (c =='#') changer_couleur(COULEURS_PAR_DEFAUT);
                else if (c == 'p') changer_couleur(VERT_SUR_NOIR);
                else if (c == 'k') changer_couleur(JAUNE_SUR_NOIR);
                else if (c == 'O') changer_couleur(BLEU_SUR_NOIR);
                else if (c == 'H') changer_couleur(JAUNE_SUR_NOIR);
                else changer_couleur(COULEURS_PAR_DEFAUT);

                ecrire_char(x, y, c);
            }

            else if (OnEstDejaPasseIci[y][x] == true){
                changer_couleur(COULEURS_PAR_DEFAUT);

                if(c == '#' || c == 'O' || c == '.' || c == 'T' ||  c == 'D' || c == 'O' || c == 'k' || c == 'H'){
                    changer_couleur(VERT_SUR_NOIR);
                    ecrire_char(x,y,c);
                }
                else{
                    ecrire_char(x,y,'.');
                }
            }
            else {
                changer_couleur(COULEURS_PAR_DEFAUT); // On nettoie le pinceau
                ecrire_char(x,y,' ');
            }
        }
    }
}


// Affiche le menu principal
void afficher_instructions() {
    ecrire_string ("ROGUE LIKE V0.5, Gomez, Jan, Beauvivre, Celestin", 20, 10);
    ecrire_string("[p] pour Jouer", 20, 12);
    ecrire_string("[a] pour Quitter", 20, 13);
}


// Main
int main() {
    // Initialisation du système graphique
    if (!initialiser_console(couleurs, 9)) {
        cout << "Erreur console" << endl;
        return 1;
    }

    // Variables d'état du Jeu 
    string nom_fichier = "maptest.txt";
    char carte[MAX_LIGNES][MAX_COLONNES]; 
    int hauteur_carte = 0;
    bool carte_chargee = false; 
    bool jeu_lance = false; 
    
    // Gestion des monstres (Tableau + Compteur)
    Monstre Les_monstre[NB_MONSTRES_MAX]; 
    int Nb_monstre_trouve = 0; 
    
    // Gestion du joueur
    Personnage heros; 
    initialiser_heros(heros); // on initialise le perso pour pouvoir le refaire à chaque fois 

    int input = 0; 

    // Boucle Principale du Jeu
    while (input != 'a') { 
        
        effacer_console(); // Nettoyage de l'écran

        // Menu
        if (!jeu_lance) {
            afficher_instructions();
            
            // Message de Game Over
            if (heros.Vie <= 0) {
                 changer_couleur(ROUGE_SUR_NOIR);
                 ecrire_string("GAME OVER - Tu es mort (nooob) !", 20, 15);
            }
            
            input = saisie_bloquante();

            // Lancement du jeu
            if (input == 'p'|| input =='P') {
                // Reset complet pour nouvelle partie
                initialiser_heros(heros);
                carte_chargee = false; 
                jeu_lance = true; 
                
                // Chargement de la carte et des monstre
                if (!carte_chargee) { 
                    charger_carte(nom_fichier, carte, hauteur_carte, Les_monstre, Nb_monstre_trouve);
                    changer_couleur (COULEURS_PAR_DEFAUT);
                    carte_chargee = true;
                }
            }
        } 
        // jeu
        else {
            // Affichage Décors
            afficher_carte(carte, hauteur_carte,heros); 
            ecrire_char(heros.x, heros.y, '@');   
            
             //Affichage des Monstres Vivants
            for(int i=0; i<Nb_monstre_trouve; i++) {
                if (Les_monstre[i].Vie > 0) {
                     int dist = abs(heros.x - Les_monstre[i].x) + abs(heros.y - Les_monstre[i].y);

                     if (dist <= heros.Vision +2){
                        changer_couleur(ROUGE_SUR_NOIR);
                        ecrire_char(Les_monstre[i].x,Les_monstre[i].y,Les_monstre[i].Symbole);
                     }
                }
            }

            afficher_hero(heros, hauteur_carte);
            changer_couleur (COULEURS_PAR_DEFAUT);
            
            input = saisie_bloquante(); // Attente touche joueur

            // Tour du Joueur (Mouvement + Attaque)
            gerer_deplacement(heros, hauteur_carte ,input, carte, Les_monstre, Nb_monstre_trouve, jeu_lance);
            
            // Tour des Monstres (IA)
            if (input == 'z' || input == 'q' || input == 's' || input == 'd') {
                deplacer_tous_les_monstres(Les_monstre, Nb_monstre_trouve, heros, carte);
            }
        }
    }

    fermer_console();  
    return 0;
}
