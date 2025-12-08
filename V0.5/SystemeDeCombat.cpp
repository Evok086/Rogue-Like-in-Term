//Variables:
    //PV_Joueur, PV_Monstre, Degats :  Entier 
    //Rep_Monstre : Bool
//Le programme décrit comment se passe un combat au sein du jeu ; si le joueur attaque alors l'ennemi prend des degats, dans le cas contraire le joueur perdra de la vie
//Sous algo pour les combats du jeu:

#include <iostream>
#include <string>
using namespace std


struct Personnage{
    string Nom;
    int Vie,Degat,Niveau,xp,Vision,Armure;
};

struct Monstre{
    string Nom,typeIA;
    int Vie,Degat,xp,Armure,Vitesse;
};
void Combat(Personnage &perso, Monstre &monstre){
    string reponse;
    bool attaque;

    attaque = true;
    cout << "Choisissez une option : Attaquer ou defendre ? (A/D)";
    cin >> reponse;
    while (perso.Vie > 0 and monstre.Vie > 0){
        if (perso.Vie <= 10) {
            cout << "Attention à vos points de vie";
        }
        if (reponse == "A" and attaque){
            monstre.Vie -= perso.Degat*2;
            attaque = false;
        }
        else{
            if (reponse == "D"){
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

}