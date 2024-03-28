#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>

#define MAX_TRANSITIONS 100
#define MAX_etats 100
#define MAX_ALPHABETS 27


// Menu :
char Menu(){
	char choix;
	printf("		****************** Menu *****************\n");
	printf("		a - (Partie 1): Stocker,afficher\n");
    printf("		b - (Partie 2): generer fichier.dot d'un automate1 et fichier png\n");
    printf("		c - (Partie 3): Test d'un mot M\n");
    printf("		d - (Partie 3): Liste des mots engendres pas l'automate\n");
    printf("                e - (partie 4): calcul de l union de deux automate\n ");
    printf("		f - (partie 4): calcule d etoile \n");
    printf("		g - (partie 4): calcule d produit \n");
    printf("		h - (partie 5): supprimer les epsilon \n");
    printf("		i - (partie 5): determiniser l automate  \n");
    printf("		j - (partie 5): minimaliser l automatee \n");
    printf("		l - (Partie 2): generer fichier.dot d'un automate2 et fichier png\n");
    printf("		k - exite \n");
    printf("Votre choix : ");
	scanf(" %c", &choix);
    return choix;
};
// Definition de la structure de transition
struct Transition {
    int etat_de_depart;
    int etat_d_arrive;
    char etiquette_de_transition;
};

// Definition de la structure de l'automate
struct Automate {
    int nb_etats;
    int nb_transitions;
    int nb_etats_initiaux;
    int nb_etats_finaux;
    int nb_alphabet;
    char Alphabet[27];
    int etats[MAX_etats];
    int etats_initiaux[MAX_etats];
    int etats_finaux[MAX_etats];
    int etats_inatteignables[MAX_etats];
    struct Transition transitions[MAX_TRANSITIONS]; 
};
// Fonction pour vérifier l'existence d'un élément dans un tableau
struct Automate lire_automate(char *nom_fichier) {
//ouverture  et lecture du fichier
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }
// initialisation de l automate 
    struct Automate automate;
    automate.nb_etats = 0;
    automate.nb_transitions = 0;
    automate.nb_etats_initiaux = 0;
    automate.nb_etats_finaux = 0;
    for (int i = 0; i < MAX_etats; i++) {
        automate.etats[i] = -1;
    }
    int depart_tr, arrivee_tr;
    char etiquette_tr;
    int i=0 ,existe;
// Boucle pour lire les transitions
    while (fscanf(fichier, "%d %d %c\n", &depart_tr, &arrivee_tr, &etiquette_tr) == 3) {
            automate.transitions[automate.nb_transitions].etat_de_depart = depart_tr;
            automate.transitions[automate.nb_transitions].etat_d_arrive = arrivee_tr;
            automate.transitions[automate.nb_transitions].etiquette_de_transition = etiquette_tr;
            automate.nb_transitions++;
            existe=0;
            for (int j= 0; j < automate.nb_transitions; j++) {
                if (automate.etats[j] == depart_tr) {
                    existe=1;  // L'élément existe dans le tableau
                }
            }
            if (existe==0){
                automate.nb_etats++;
                automate.etats[i] = depart_tr ;
                i++ ;
            }
            existe=0;
            for (int j= 0; j < automate.nb_transitions; j++) {
                if (automate.etats[j] == arrivee_tr) {
                    existe=1;  // L'élément existe dans le tableau
                }
            }
            if (existe==0){
                automate.nb_etats++;
                automate.etats[i] = arrivee_tr ;
                i++ ;
            }
    }
// Lecture des etats initiaux
    char ligne[100];
    while (fgets(ligne, 100, fichier) != NULL) {
        if (strncmp(ligne, "Etats_initiaux :", 16) == 0) {
            char* ptr = strtok(ligne+16, ",");
            while (ptr != NULL) {
                sscanf(ptr, "%d", &automate.etats_initiaux[automate.nb_etats_initiaux]);
                automate.nb_etats_initiaux++;
                ptr = strtok(NULL, ",");
            }
        }
// Lecture des etats finaux
	    if (strncmp(ligne, "Etats_finaux :", 14) == 0) {
            char* ptr = strtok(ligne+14, ",");
            while (ptr != NULL) {
                sscanf(ptr, "%d", &automate.etats_finaux[automate.nb_etats_finaux]);
                automate.nb_etats_finaux++;
                ptr = strtok(NULL, ",");
            }
        }
    }
    fclose(fichier);
    return automate;
}
// Fonction pour afficher les informations d'un automate
void afficher_automate(struct Automate automate) {
	int i;
    printf("Nombre d'etats : %d\n", automate.nb_etats);
    printf("Nombre de transitions : %d\n", automate.nb_transitions);
    printf("Nombre d'etats initiaux : %d\n", automate.nb_etats_initiaux);
    printf("Nombre d'etats finaux : %d\n", automate.nb_etats_finaux);
    printf("Etats initiaux : ");
    for (i = 0; i < automate.nb_etats_initiaux; i++) {
        printf("%d ", automate.etats_initiaux[i]);
    }
    printf("\n");
    printf("Etats finaux : ");
    for (i = 0; i < automate.nb_etats_finaux; i++) {
        printf("%d ", automate.etats_finaux[i]);
    }
    printf("\n");
    printf("les Etats sont : ");
    for (i = 0; i < automate.nb_etats; i++) {
        printf("%d ", automate.etats[i]);
    }
    printf("\n");

}
//afficher l alphabet 
void afficher_alphabe(struct Automate automate){
   // Utilisation d'un tableau de booléens pour marquer les alphabets déjà affichés
   automate.nb_alphabet=0;
   int j=0;
    bool alphabet_present[MAX_ALPHABETS] = {false};
    printf("l esemble d alphabe est : ");
   // Parcours de chaque transition
    for (int i = 0; i < automate.nb_transitions; i++) {
        char alphabet = automate.transitions[i].etiquette_de_transition;
        // Vérification si l'alphabet de la transition n'a pas encore été affiché
        if (!alphabet_present[alphabet - 'a']) {
            printf("%c ", alphabet);
            alphabet_present[alphabet - 'a'] = true;
            automate.Alphabet[j]=alphabet;
            j++;
            automate.nb_alphabet++;
        }
    }
    printf("\n"); 
}

//partie 2---------------------------------------------------------------------------------------------------------------------
void generer_dot(struct Automate automate, char *nom_fichier) {
	int i,k=0;
    FILE *fichier = fopen(nom_fichier, "w");
    fprintf(fichier, "digraph G {\n");
    for (i = 0; i < automate.nb_transitions; i++) {
        int depart = automate.transitions[i].etat_de_depart;
        int arrivee = automate.transitions[i].etat_d_arrive;
        char etiquette = automate.transitions[i].etiquette_de_transition;
        fprintf(fichier, "  %d -> %d [label=\"%c\"];\n", depart, arrivee, etiquette);
    }

    // etats inatteignables
    for (int i = 0; i <automate.nb_etats; i++) {
        int etat_inatteignable = 1;
        for (int j = 0; j < automate.nb_transitions; j++) {
            if (automate.transitions[j].etat_d_arrive == automate.etats[i]) {
                etat_inatteignable = 0;
            }
        }
        if (etat_inatteignable) {
            automate.etats_inatteignables[k]=automate.etats[i];
            fprintf(fichier, "  %d ",automate.etats[i]);
            fprintf(fichier, "[shape = circle, style=filled, color = gray];\n");
            k++;
        } 
    }
    // etats finaux 
    for (i = 0; i < automate.nb_etats_finaux; i++) {
        fprintf(fichier, "  %d [shape=circle, style=filled, color=blue];\n", automate.etats_finaux[i]);
        }

    // etats initiaux 
    for (i = 0; i < automate.nb_etats_initiaux; i++) {
    	fprintf(fichier, "  %d [shape=circle, style=filled, color=green];\n", automate.etats_initiaux[i]);
	}
    
    fprintf(fichier, "}\n");
    fclose(fichier);  
    char commande[100];
    sprintf(commande, "dot -Tpng %s -o automate.png", nom_fichier);

    // Exécution de la commande système
    system(commande);}

//partie 3------------------------------------------------------------------------------------------------------------------------------------
// Fonction permettant de tester si un mot M est engendr� par un automate A.
int est_engendre(struct Automate automate, char *mot) {
    int i, j;
    int nb_etats_actuels, etats_actuels[MAX_etats];
    int etats_suivants[MAX_etats];
    // Initialisation des etats actuels a partir des etats initiaux de l'automate
    nb_etats_actuels = automate.nb_etats_initiaux;
    for (i = 0; i < nb_etats_actuels; i++) {
        etats_actuels[i] = automate.etats_initiaux[i];
    }
    // Parcours du mot lettre par lettre
    for (i = 0; mot[i] != '\0'; i++) {
        // Recherche des transitions etiquetees par la lettre courante
        int nb_etats_suivants = 0;
        for (j = 0; j < automate.nb_transitions; j++) {
            struct Transition transition = automate.transitions[j];
            if (transition.etiquette_de_transition == mot[i]) {
                int k;                                                                                                                                                                                                                                                          
                for (k = 0; k < nb_etats_actuels; k++) {
                    if (etats_actuels[k] == transition.etat_de_depart) {
                        // L'�tat suivant est ajout� � la liste des �tats suivants
                        etats_suivants[nb_etats_suivants] = transition.etat_d_arrive;
                        nb_etats_suivants++;
                    }
                }
            }
        }
        // Si aucun etat suivant n'a t trouve, le mot n'est pas engendre par l'automate
        if (nb_etats_suivants == 0) {
            return 0;
        }
        // Les etats suivants deviennent les nouveaux etats actuels
        nb_etats_actuels = nb_etats_suivants;
        for (j = 0; j < nb_etats_actuels; j++) {
            etats_actuels[j] = etats_suivants[j];
        }
    }

    // Verification que l'un des etats actuels est un etat final de l'automate
    for (i = 0; i < nb_etats_actuels; i++) {
        for (j = 0; j < automate.nb_etats_finaux; j++) {
            if (etats_actuels[i] == automate.etats_finaux[j]) {
                return 1;
            }
        }
    }

    return 0;
}

/*listez les mot engendrer par  l'automate*/
void list_mots_engendres(struct Automate automate, char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", nom_fichier);
        exit(EXIT_FAILURE);
    }
    char ligne[MAX_TRANSITIONS];
    int nb_mots_engendres = 0;
    char mots_engendres[MAX_TRANSITIONS][MAX_TRANSITIONS];
    while (fgets(ligne, MAX_TRANSITIONS, fichier) != NULL) {
        // Supprime le caract�re de retour � la ligne (\n) de la fin de la cha�ne
        ligne[strcspn(ligne, "\n")] = 0;
        int estEngendre = est_engendre(automate, ligne);
        if (estEngendre == 1) {
            strcpy(mots_engendres[nb_mots_engendres], ligne);
            nb_mots_engendres++;
        }
    }
    //affichage des mots engendrees 
	int i;
    if (nb_mots_engendres!=0) { 
        printf("Les mots suivants sont engendres par l'automate : \n");
        for (i = 0; i < nb_mots_engendres; i++) {
            printf("%s\n", mots_engendres[i]);
        }
    } else {
        printf("Aucun mot n'est engendre par l'automate.\n");
    }

    fclose(fichier);
}
//-----------------------------------------------------------------------------
//Fonction qui calcule union de deux automate
struct Automate Union_Automates(struct Automate automate1, struct Automate automate2) {
    struct Automate union_automate;
    int i;

    union_automate.nb_etats = automate1.nb_etats + automate2.nb_etats + 2; // Ajouter 2 pour les nouveaux etats initial et final
    union_automate.nb_transitions = automate1.nb_transitions + automate2.nb_transitions + automate1.nb_etats_initiaux + automate2.nb_etats_initiaux + automate1.nb_etats_finaux + automate2.nb_etats_finaux;
    union_automate.nb_etats_initiaux = 1;
    union_automate.nb_etats_finaux = 1;

    // Nouvel etat initial et final
    int new_initial_state = 99;
    int new_final_state = 999;
    
    // Copier les etats initiaux et finaux
    union_automate.etats_initiaux[0] = new_initial_state;
    union_automate.etats_finaux[0] = new_final_state;
    // Ajouter les états de l'automate 1
    for (i = 0; i < automate1.nb_etats; i++) {
        union_automate.etats[i] = automate1.etats[i];
    }   
    // Ajouter les états de l'automate 2, en ajustant les numéros d'états
    for (i = 0; i < automate2.nb_etats; i++) {
        union_automate.etats[automate1.nb_etats + i] = automate2.etats[i] + automate1.nb_etats;
    }
    union_automate.etats[union_automate.nb_etats - 2] = new_initial_state;
    union_automate.etats[union_automate.nb_etats-1] = new_final_state;
    memcpy(union_automate.transitions, automate1.transitions, automate1.nb_transitions * sizeof(struct Transition));    
    // Copier les transitions de l'automate 2 et ajuster les numeros des etats
    for (i = 0; i < automate2.nb_transitions; i++) {
        struct Transition transition = automate2.transitions[i];
        transition.etat_de_depart += automate1.nb_etats;
        transition.etat_d_arrive += automate1.nb_etats;
        union_automate.transitions[automate1.nb_transitions + i] = transition;
    }
    // Ajouter les transitions epsilon entre le nouvel etat initial et les etats initiaux des automates 1 et 2
    int epsilon_transition_index = automate1.nb_transitions + automate2.nb_transitions;
    for (i = 0; i < automate1.nb_etats_initiaux; i++) {
        struct Transition epsilon_transition;
        epsilon_transition.etat_de_depart = new_initial_state;
        epsilon_transition.etat_d_arrive= automate1.etats_initiaux[i];
        epsilon_transition.etiquette_de_transition = 'e';
        union_automate.transitions[epsilon_transition_index++] = epsilon_transition;
    }
    for (i = 0; i < automate2.nb_etats_initiaux; i++) {
        struct Transition epsilon_transition;
        epsilon_transition.etat_de_depart= new_initial_state;
        epsilon_transition.etat_d_arrive = automate2.etats_initiaux[i] + automate1.nb_etats;
        epsilon_transition.etiquette_de_transition= 'e';
        union_automate.transitions[epsilon_transition_index++] = epsilon_transition;
    }

    // Ajouter les transitions epsilon entre les etats finaux des automates 1 et 2 et le nouvel �tat final
    for (i = 0; i < automate1.nb_etats_finaux; i++) {
        struct Transition epsilon_transition;
        epsilon_transition.etat_de_depart= automate1.etats_finaux[i];
        epsilon_transition.etat_d_arrive = new_final_state;
        epsilon_transition.etiquette_de_transition = 'e';
        union_automate.transitions[epsilon_transition_index++] = epsilon_transition;
    }
     for (i = 0; i < automate2.nb_etats_finaux; i++) {
        struct Transition epsilon_transition;
        epsilon_transition.etat_de_depart= automate2.etats_finaux[i] + automate1.nb_etats;
        epsilon_transition.etat_d_arrive = new_final_state;
        epsilon_transition.etiquette_de_transition = 'e';
        union_automate.transitions[epsilon_transition_index++] = epsilon_transition;
    }
    return union_automate;
}
// Fonction pour mettre l'etoile un automate
struct Automate etoile_automate(struct Automate automate) {
    struct Automate etoile;
    int i,j;
    etoile.nb_etats = automate.nb_etats + 2; // Un nouvel �tat initial et un nouvel �tat final
    etoile.nb_transitions = automate.nb_transitions + automate.nb_etats_initiaux + automate.nb_etats_finaux + automate.nb_etats_initiaux * automate.nb_etats_finaux;
    etoile.nb_etats_initiaux = 1;
    etoile.nb_etats_finaux = 1;
    etoile.etats_initiaux[0] = 99; // Nouvel etat initial
    etoile.etats_finaux[0] = 999; // Nouvel etat final
    // Copie des transitions de l'automate d'origine
    for (i = 0; i < automate.nb_transitions; i++) {
        etoile.transitions[i] = automate.transitions[i];
    }
    //remplissage des etsts
    for (i = 0; i < etoile.nb_etats; i++) {
        etoile.etats[i] = automate.etats[i];
    }
    etoile.etats[etoile.nb_etats-2]=99;
    etoile.etats[etoile.nb_etats-1]=999;
    int index_transition = automate.nb_transitions;
    // Relier le nouvel etat initial aux etats initiaux de l'automate d'origine avec des transitions epsilon
    for (i = 0; i < automate.nb_etats_initiaux; i++) {
        etoile.transitions[index_transition].etat_de_depart = etoile.etats_initiaux[0];
        etoile.transitions[index_transition].etat_d_arrive = automate.etats_initiaux[i];
        etoile.transitions[index_transition].etiquette_de_transition = 'e';
        index_transition++;
    }
    // Relier les etats finaux de l'automate d'origine e l'etat final de etoile_automate avec des transitions epsilon
    for (i = 0; i < automate.nb_etats_finaux; i++) {
        etoile.transitions[index_transition].etat_de_depart = automate.etats_finaux[i];
        etoile.transitions[index_transition].etat_d_arrive = etoile.etats_finaux[0];
        etoile.transitions[index_transition].etiquette_de_transition = 'e';
        index_transition++;
    }

    // Relier le nouvel etat initial de etoile_automate au nouvel etat final avec des transitions epsilon
    etoile.transitions[index_transition].etat_de_depart = etoile.etats_initiaux[0];
    etoile.transitions[index_transition].etat_d_arrive = etoile.etats_finaux[0];
    etoile.transitions[index_transition].etiquette_de_transition = 'e';
    index_transition++;

    // Relier les etats finaux de l'automate d'origine aux etats initiaux de l'automate d'origine avec des transitions epsilon
    for (i = 0; i < automate.nb_etats_finaux; i++) {
        for (j = 0; j < automate.nb_etats_initiaux; j++) {
	        etoile.transitions[index_transition].etat_de_depart = automate.etats_finaux[i];
	        etoile.transitions[index_transition].etat_d_arrive = automate.etats_initiaux[j];
	        etoile.transitions[index_transition].etiquette_de_transition = 'e';
	        index_transition++;
        }
    }
	etoile.nb_transitions = index_transition;
    return etoile;
}
// Fonction pour mettre le produit d'un automate
struct Automate produit1_automates(struct Automate automate1, struct Automate automate2) {
    struct Automate produit;
    int existe;
    int i,j;
    produit.nb_etats = 0;
    produit.nb_transitions = 0;
    produit.nb_etats_initiaux = 0;
    produit.nb_etats_finaux = 0;

    // Recherche des etats initiaux communs
    for (i = 0; i < automate1.nb_etats_initiaux; i++) {
        for (j = 0; j < automate2.nb_etats_initiaux; j++) {
            if (automate1.etats_initiaux[i] == automate2.etats_initiaux[j]) {
                produit.etats_initiaux[produit.nb_etats_initiaux++] = automate1.etats_initiaux[i];
            }
        }
    }
    // Recherche des etats finaux communs
    for (i = 0; i < automate1.nb_etats_finaux; i++) {
        for (j = 0; j < automate2.nb_etats_finaux; j++) {
            if (automate1.etats_finaux[i] == automate2.etats_finaux[j]) {
                produit.etats_finaux[produit.nb_etats_finaux++] = automate1.etats_finaux[i];
            }
        }
    }
    // Recherche des transitions communes
    for (i = 0; i < automate1.nb_transitions; i++) {
        for (j = 0; j < automate2.nb_transitions; j++) {
            if (automate1.transitions[i].etat_de_depart == automate2.transitions[j].etat_de_depart&&
                automate1.transitions[i].etat_d_arrive == automate2.transitions[j].etat_d_arrive &&
                automate1.transitions[i].etiquette_de_transition == automate2.transitions[j].etiquette_de_transition) {

                produit.transitions[produit.nb_transitions].etat_de_depart = automate1.transitions[i].etat_de_depart;
                produit.transitions[produit.nb_transitions].etat_d_arrive = automate1.transitions[i].etat_d_arrive;
                produit.transitions[produit.nb_transitions].etiquette_de_transition = automate1.transitions[i].etiquette_de_transition;
                produit.nb_transitions++;
            }
        }
    }
    // Mise ajour du nombre d'etats
    int max_etat = 0;
    for (i = 0; i < produit.nb_transitions; i++) {
        if (produit.transitions[i].etat_de_depart > max_etat) {
            max_etat = produit.transitions[i].etat_de_depart;
        }
        if (produit.transitions[i].etat_d_arrive > max_etat) {
            max_etat = produit.transitions[i].etat_d_arrive;
        }
    }
    produit.nb_etats = max_etat + 1;
    //remplissage tableau etats 
    for (int i = 0; i < produit.nb_etats; i++){
        struct Transition transition =produit.transitions[i];
        existe=0;
        for (int j= 0; j < produit.nb_transitions; j++) {
            if (produit.etats[j] == transition.etat_de_depart) {
                existe=1;  // L'élément existe dans le tableau
            }
        }
        if (existe==0){
            produit.etats[i] = transition.etat_de_depart ;
        }
        existe=0;
        for (int j= 0; j < produit.nb_transitions; j++) {
            if (produit.etats[j] == transition.etat_d_arrive) {
                existe=1;  // L'élément existe dans le tableau
            }
        }
        if (existe==0){
            produit.etats[i] = transition.etat_d_arrive ;
        }
    }    
    return produit;
}
struct Automate supprimer_epsilon(struct Automate a){
    struct Automate na;
    na.nb_etats = a.nb_etats;
    na.nb_transitions = 0;
    na.nb_etats_initiaux = 0;
    na.nb_etats_finaux = 0;
    // Copie des etats initiaux et finaux de l'automate
    int i;
    for (i = 0; i < a.nb_etats_initiaux; ++i)
    {
        na.etats_initiaux[i] = a.etats_initiaux[i];
        na.nb_etats_initiaux++;
    }
    for ( i = 0; i < a.nb_etats_finaux; ++i)
    {
        na.etats_finaux[i] = a.etats_finaux[i];
        na.nb_etats_finaux++;
    }
    // Copie des transitions de l'automate sans les epsilon transitions
    for (i = 0; i < a.nb_transitions; ++i)
    {
        if (a.transitions[i].etiquette_de_transition != 'e')
        {
            na.transitions[na.nb_transitions] = a.transitions[i];
            na.nb_transitions++;
        }
    }

    // Suppression des epsilon transitions
    for ( i = 0; i < a.nb_etats; ++i){
        for (int j = 0; j < a.nb_etats; ++j)
        {
            if (i == j){
                continue;
            }
            for (int k = 0; k < a.nb_transitions; ++k)
            {
                if (a.transitions[k].etat_de_depart == i && a.transitions[k].etat_d_arrive == j && a.transitions[k].etiquette_de_transition == 'e')
                {
                	
                    for (int l = 0; l < a.nb_transitions; ++l)
                    {
                        if (a.transitions[l].etat_de_depart == j && a.transitions[l].etiquette_de_transition != 'e')
                        {
                            struct Transition t = { i, a.transitions[l].etat_d_arrive, a.transitions[l].etiquette_de_transition };
                            na.transitions[na.nb_transitions] = t;
                            na.nb_transitions++;
                        }
                    }
                }
            }
        }
    }

    return na;
}

//fontion pour determiniser un automate
struct Automate determinisation(struct Automate  a) {
    struct Automate resultat; // Automate résultant après la déterminisation
    resultat.nb_transitions = 0;
    resultat.nb_etats_initiaux = 0;
    resultat.nb_etats_finaux = 0;
    resultat.nb_etats = 0;

    int i, j, k, found;
    char etiquete[MAX_TRANSITIONS][MAX_ALPHABETS]; // Tableau pour stocker les étiquettes des transitions
    char nb_etiquete = 0;

    // Recherche des étiquettes uniques dans les transitions de l'automate d'entrée
    for (i = 0; i < a.nb_transitions; i++) {
        found = 0;
        for (k = 0; k < nb_etiquete; k++) {
            if (etiquete[k][0] == a.transitions[i].etiquette_de_transition) {
                found = 1;
                break;
            }
        }
        if (!found) {
            etiquete[nb_etiquete][0] = a.transitions[i].etiquette_de_transition;
            nb_etiquete++;
        }
    }

    char etats[MAX_TRANSITIONS][MAX_etats][2]; // Tableau pour stocker les états de l'automate résultant
    int nb_etat = 0, nb_current_etat = 0; // Compteurs pour les états dans l'automate résultant
    char tmp[MAX_TRANSITIONS][2]; // Tableau temporaire pour stocker les états suivants
    int nbr_etat[MAX_TRANSITIONS], tmp_var = 0; // Tableaux pour le nombre d'états et les états temporaires
    char etat_courant[MAX_TRANSITIONS * MAX_etats * 2] = ""; // Chaîne de caractères pour l'état courant
    int mapping[MAX_etats]; // Tableau pour mapper les anciens numéros d'états aux nouveaux numéros d'états

    // Initialiser le tableau de mappage à -1
    for (i = 0; i < MAX_etats; i++) {
        mapping[i] = -1;
    }

    // Ajouter les états initiaux de l'automate d'entrée à l'état initial de l'automate résultant
    for (i = 0; i < a.nb_etats_initiaux; i++) {
        etats[nb_etat][i][0] = a.etats_initiaux[i] + '0';
        strcat(etat_courant, etats[nb_etat][i]);
        strcat(etat_courant, ",");
    }

    etat_courant[strlen(etat_courant) - 1] = '\0';
    resultat.etats_initiaux[resultat.nb_etats_initiaux] = atoi(etat_courant); // Enregistrer l'état initial
    mapping[atoi(etat_courant)] = resultat.nb_etats; // Mapper l'ancien numéro d'état au nouveau numéro d'état
    nbr_etat[nb_etat] = a.nb_etats_initiaux;
    nb_etat++;
    resultat.nb_etats_initiaux++;
    resultat.nb_etats++;

    // Tant qu'il y a des états à traiter
    while (nb_current_etat < nb_etat) {
        int final = 0; // Variable pour indiquer si l'état courant est final
        // Copier les états de l'état courant
        for (j = 0; j < nbr_etat[nb_current_etat]; j++) {
            strcpy(etats[nb_etat][j], etats[nb_current_etat][j]);
            // Vérifier si l'un des états de l'état courant est final
            for (k = 0; k < a.nb_etats_finaux; k++) {
                if (etats[nb_current_etat][j][0] == (a.etats_finaux[k] + '0')) {
                    final = 1;
                    break;
                }
            }
        }

        // Si l'état courant est final, l'ajouter aux états finaux de l'automate résultant
        if (final) {
            char etat_courant[MAX_TRANSITIONS *MAX_etats * 2] = "";
            for (j = 0; j < nbr_etat[nb_current_etat]; j++) {
                strcat(etat_courant, etats[nb_current_etat][j]);
                strcat(etat_courant, ",");
            }
            etat_courant[strlen(etat_courant) - 1] = '\0';
            resultat.etats_finaux[resultat.nb_etats_finaux++] = mapping[atoi(etat_courant)]; // Enregistrer l'état final
        }

        // Pour chaque étiquette d'une transition dans l'automate d'entrée
        for (i = 0; i < nb_etiquete; i++) {
            tmp_var = 0;
            // Pour chaque état de l'état courant
            for (j = 0; j < nbr_etat[nb_current_etat]; j++) {
                // Pour chaque transition dans l'automate d'entrée
                for (k = 0; k < a.nb_transitions; k++) {
                    // Si la transition correspond à l'état courant et à l'étiquette actuelle
                    if ((etats[nb_current_etat][j][0] == a.transitions[k].etat_de_depart + '0') && (etiquete[i][0] == a.transitions[k].etiquette_de_transition)) {
                        tmp[tmp_var][0] = a.transitions[k].etat_d_arrive + '0';
                        tmp_var++;
                    }
                }
            }

            // Supprimer les doublons des états suivants
            int size = tmp_var;
            char uniqueArr[size][2];
            int uniqueSize = 0;
            int isDuplicate = 0;
            for (k = 0; k < size; k++) {
                isDuplicate = 0;
                for (j = 0; j < uniqueSize; j++) {
                    if (strcmp(tmp[k], uniqueArr[j]) == 0) {
                        isDuplicate = 1;
                        break;
                    }
                }
                if (!isDuplicate) {
                    strcpy(uniqueArr[uniqueSize], tmp[k]);
                    uniqueSize++;
                }
            }

            // Trier les états suivants
            for (k = 0; k < uniqueSize - 1; k++) {
                for (j = 0; j < uniqueSize - k - 1; j++) {
                    if (strcmp(uniqueArr[j], uniqueArr[j + 1]) > 0) {
                        char temp[2];
                        strcpy(temp, uniqueArr[j]);
                        strcpy(uniqueArr[j], uniqueArr[j + 1]);

                    }
                }
            }

            // Vérification de la présence de l'état courant dans les états déjà explorés
            int found = 0;
            for (j = 0; j < nb_etat; j++) {
                for (k = 0; k < uniqueSize; k++) {
                    if ((uniqueArr[k][0] != etats[j][k][0]) || nbr_etat[j] != uniqueSize) {
                        break;
                    }
                    else if (k == (uniqueSize - 1)) {
                        found = 1;
                    }
                }
            }

            // Si l'état courant n'a pas été rencontré auparavant, le traiter
            if (uniqueSize != 0) {
                char etat_courant[MAX_TRANSITIONS * MAX_etats* 2] = "", etat_suivant[MAX_TRANSITIONS * MAX_etats * 2] = "";

                // Construction de la chaîne représentant l'état suivant
                for (j = 0; j < uniqueSize; j++) {
                    strcat(etat_suivant, uniqueArr[j]);
                    strcat(etat_suivant, ",");
                }
                etat_suivant[strlen(etat_suivant) - 1] = '\0';

                // Construction de la chaîne représentant l'état courant
                for (j = 0; j < nbr_etat[nb_current_etat]; j++) {
                    strcat(etat_courant, etats[nb_current_etat][j]);
                    strcat(etat_courant, ",");
                }
                etat_courant[strlen(etat_courant) - 1] = '\0';

                // Vérification si l'état courant est déjà mappé
                int etat_courant_mapped = mapping[atoi(etat_courant)];

                // Si l'état courant n'est pas mappé, mapper et incrémenter le nombre d'états
                if (etat_courant_mapped == -1) {
                    mapping[atoi(etat_courant)] = resultat.nb_etats;
                    etat_courant_mapped = resultat.nb_etats;
                    resultat.nb_etats++;
                }

                // Vérification si l'état suivant est déjà mappé
                int etat_suivant_mapped = mapping[atoi(etat_suivant)];
                if (etat_suivant_mapped == -1) {
                    mapping[atoi(etat_suivant)] = resultat.nb_etats;
                    etat_suivant_mapped = resultat.nb_etats;
                    resultat.nb_etats++;
                }

                // Avant d'ajouter une transition, vérification si elle pointe vers le même état
                if (strcmp(etat_courant, etat_suivant) != 0) {
                    // Ajout de la transition à l'automate résultant
                    resultat.transitions[resultat.nb_transitions].etat_de_depart = etat_courant_mapped;
                    resultat.transitions[resultat.nb_transitions].etat_d_arrive = etat_suivant_mapped;
                    resultat.transitions[resultat.nb_transitions].etiquette_de_transition = etiquete[i][0];
                    resultat.nb_transitions++;
                }

                // Si l'état suivant est final, l'ajouter aux états finaux de l'automate résultant
                for (j = 0; j < a.nb_etats_finaux; j++) {
                    if (atoi(etat_suivant) == a.etats_finaux[j]) {
                        resultat.etats_finaux[resultat.nb_etats_finaux++] = etat_suivant_mapped;
                        break;
                    }
                }

                // Si l'état courant est final, l'ajouter aux états finaux de l'automate résultant
                for (j = 0; j < a.nb_etats_finaux; j++) {
                    if (atoi(etat_courant) == a.etats_finaux[j]) {
                        resultat.etats_finaux[resultat.nb_etats_finaux++] = etat_courant_mapped;
                        break;
                    }
                }

                // Si l'état n'a pas été rencontré précédemment, l'ajouter pour traitement ultérieur
                if (!found) {
                    for (j = 0; j < uniqueSize; j++) {
                        strcpy(etats[nb_etat][j], uniqueArr[j]);
                    }
                    nbr_etat[nb_etat] = uniqueSize;
                    nb_etat++;
                }
            }
        }
        nb_current_etat++;
    }
    return resultat;
}
// Fonction pour inverser un automate
struct Automate inverser_automate_1(struct Automate automate) {
    struct Automate automate_inverse;
    // Échanger les états initiaux et finaux
    automate_inverse.nb_etats_initiaux = automate.nb_etats_finaux;
    automate_inverse.nb_etats_finaux = automate.nb_etats_initiaux;
    for (int i = 0; i < automate.nb_etats_finaux; i++) {
        automate_inverse.etats_initiaux[i] = automate.etats_finaux[i];
    }
    for (int i = 0; i < automate.nb_etats_initiaux; i++) {
        automate_inverse.etats_finaux[i] = automate.etats_initiaux[i];
    }

    // Renverser les transitions
    automate_inverse.nb_transitions = automate.nb_transitions;
    for (int i = 0; i < automate.nb_transitions; i++) {
        automate_inverse.transitions[i].etat_de_depart = automate.transitions[i].etat_d_arrive;
        automate_inverse.transitions[i].etat_d_arrive = automate.transitions[i].etat_de_depart;
        automate_inverse.transitions[i].etiquette_de_transition= automate.transitions[i].etiquette_de_transition;
    }
    
    return automate_inverse;
}

//fonction minimaliser 
// Fonction pour minimiser un automate par la méthode de Brzozowski
struct Automate minimiser_automate(struct Automate automate) {

    // Étape 1 : Inverser l'automate
    struct Automate inverse = inverser_automate_1(automate);

    // Étape 2 : Déterminiser l'automate inversé
    struct Automate deterministe_inverse = determinisation(inverse);

    // Étape 3 : Inverser à nouveau l'automate déterminisé
    struct Automate inverse_deterministe_inverse = inverser_automate_1(deterministe_inverse);

    // Étape 4 : Déterminiser à nouveau l'automate inversé-déterminisé
    struct Automate minimal = determinisation(inverse_deterministe_inverse);
    minimal.etats_initiaux[0]=minimal.transitions[0].etat_de_depart;
    minimal.etats_finaux[0]=minimal.transitions[minimal.nb_etats-1].etat_d_arrive;


    return minimal;
}

// Fonction principale pour tester les fonctions
int main() {
    struct Automate automate1;
    struct Automate automate2;
    struct Automate automate_union;
    struct Automate automate_etoile;
    
    char choix;
    while (1)
    {
        choix = Menu();
        getchar();
        switch (choix) {
            case 'a': 
                automate1 = lire_automate("automate1.txt");
                afficher_automate(automate1);
                afficher_alphabe(automate1);
                break;
            case 'b':
                automate1 = lire_automate("automate1.txt");
                afficher_automate(automate1);
                generer_dot(automate1, "automate.dot");
                afficher_alphabe(automate1);
                break;
            case  'c': 
                automate1 = lire_automate("automate1.txt");
            	printf("Question 1: Test de mot:\n");
            	char mot[MAX_TRANSITIONS];
			    printf("Entrez un mot : ");
			    fgets(mot, MAX_TRANSITIONS, stdin);
			    // Supprime le caract�re de retour � la ligne (\n) de la fin de la cha�ne
			    mot[strcspn(mot, "\n")] = 0;
                int estEngendre = est_engendre(automate1, mot);
                if (estEngendre == 1) {
                    printf("Le mot est engendre par l'automate.\n");
                } else {
                    /*printf("Le mot n'est pas engendre par l'automate.\n");*/
                    
                }
                break;
            case 'd':
                automate1 = lire_automate("automate1.txt");
                printf("Question 2: La liste des mots engendres pas l'automate dans le fichier :\n");
			    list_mots_engendres(automate1, "Listedesmots.txt");
        	    break;
            case 'e':
                automate1 = lire_automate("automate1.txt");
                afficher_automate(automate1);
                printf("------------------------------------------------------------------------\n");
                automate2 = lire_automate("automate2.txt");
                afficher_automate(automate2);
                printf("------------------------------------------------------------------------\n");
                automate_union = Union_Automates(automate1, automate2);
                afficher_automate(automate_union);
                generer_dot(automate_union, "automate.dot");
                automate_etoile=supprimer_epsilon(automate_union);
                generer_dot(automate_etoile, "automate.dot");

                break;
            case 'f':
                automate1 = lire_automate("automate1.txt");
                printf("Question : l etoile dun automate :\n");
			    automate_etoile= etoile_automate(automate1);
                afficher_automate(automate_etoile);
                generer_dot(automate_etoile, "automate.dot");
                break;
            case  'g':
                automate1 = lire_automate("automate1.txt");
                afficher_automate(automate1);
                printf("------------------------------------------------------------------------\n");
                automate2 = lire_automate("automate2.txt");
                afficher_automate(automate2);
                printf("------------------------------------------------------------------------\n");
                automate_etoile = produit1_automates(automate1, automate2);
                afficher_automate(automate_etoile);
                generer_dot(automate_etoile, "automate.dot");
                break;
            break;
            case 'h':
                automate1 = lire_automate("automate1.txt");
                afficher_automate(automate1);
                automate_etoile=supprimer_epsilon(automate1);
                generer_dot(automate_etoile, "automate.dot");
                break;
            case 'i':
                automate1=lire_automate("automate1.txt");
                afficher_automate(automate1);
                afficher_alphabe(automate1);
                automate_etoile=determinisation(automate1);
                afficher_automate(automate_etoile);
                generer_dot(automate_etoile, "automate.dot");
                break;
            case 'j':
                automate1 =lire_automate("automate1.txt");
                afficher_automate(automate1);
                printf("-----------\n");
                automate_etoile=minimiser_automate(automate1);
                printf("-----------\n");
                afficher_automate(automate_etoile);
                generer_dot(automate_etoile, "automate.dot");
                break;
            case 'l':
                automate1 = lire_automate("automate2.txt");
                afficher_automate(automate1);
                generer_dot(automate1, "automate2.dot");
                afficher_alphabe(automate1);
                break;
            default:
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 5.\n");
                break;
        }
    }
    
    
    return 0;
}