//////////////////////////////////////////////////////////////////////////////////////////
/// \file tictactoe.c
/// \author Benjamin Ganty
/// \author Gérôme Pasquier
/// \date 22 janvier 2015
/// \brief Tictactoe game.
///
/// This program is a game of Tictactoe for 2 players.
//////////////////////////////////////////////////////////////////////////////////////////

#include "ulibc.h"

#define BUFFER_SIZE 512
#define SECONDE 1000

void printGame(int * jeu);
int verifieGagner(int * jeu);
int placerPion(int position, int * jeu, int joueur);
int joueurSuivant(int joueur);
void printScore(int* score);
void printScreen(char* fileName);

//////////////////////////////////////////////////////////////////////////////////////////
void main()
{
    printScreen("tictactoeacueil.txt");

    sleep(5*SECONDE);

    printScreen("tictactoejeu.txt");

    int jeu[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    int score[3] = {0, 0, 0};
    int joueur = 1;
    int nb_coup = 0;

    while(true)
    {
        set_cursor(23,3);
        printf("Au tour du joueur %d                  ", joueur);

        // Recupere et place (si possible) une X ou un O la ou le joueur veux jouer
        while(true)
        {
            char c = getc();
            if(c >= '1' && c <= '9')
            {
                if(placerPion(c-'0', jeu, joueur) == 0)
                {
                    break;
                }
            }
            if(c == 'q')
            {
                clear_display();
                set_cursor(0, 0);
                exit();
            }
        }

        nb_coup++;

        // Affichage de l'etat du jeux
        printGame(jeu);

        //Verification de victoire => réinitialiser le jeux
        if(verifieGagner(jeu))
        {
            set_cursor(23, 3);
            printf("Le joueur %d a gagne felicitation", joueur);
            score[joueur]++;
            printScore(score);
            while(true)
            {
                char c = getc();
                if (c == 'q')
                {
                    clear_display();
                    set_cursor(0,0);
                    exit();
                }
                if(c == 'r')
                {
                    nb_coup = 0;
                    for (int i = 0; i < 9; ++i)
                    {
                        jeu[i] = 0;
                    }
                    joueur = joueurSuivant(joueur);
                    printGame(jeu);
                    break;
                }
            }
        }

        //Si fin du jeux sur une egalite => réinitialiser le jeux
        if(nb_coup == 9)
        {
            set_cursor(23, 3);
            printf("Personne n'as gagne ...", joueur);
            while(true)
            {
                char c = getc();
                if (c == 'q')
                {
                    exit();
                }
                if(c == 'r')
                {
                    nb_coup = 0;
                    for (int i = 0; i < 9; ++i)
                    {
                        jeu[i] = 0;
                    }
                    joueur = joueurSuivant(joueur);
                    printGame(jeu);
                    break;
                }
            }
        }

        joueur = joueurSuivant(joueur);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void printGame(int * jeu)
{
    int ligne = 8, colonne = 26;
    for (int i = 0; i < 9; ++i)
    {
        set_cursor(ligne + (i/3)*3, colonne + (i%3)*6);
        switch (jeu[i])
        {
            case 0:
                putc(' ');
                break;
            case 1:
                putc('O');
                break;
            case 2:
                putc('X');
                break;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
int verifieGagner(int * jeu)
{
    for (int i = 0; i < 3; ++i)
    {
        //Verification des lignes
        if((jeu[3*i]==jeu[3*i+1] && jeu[3*i+1]==jeu[3*i+2]) && (jeu[3*i]!=0))
            return jeu[3*i];
        //Verification des colonnes
        if((jeu[i]==jeu[i+3] && jeu[i+3]==jeu[i+6]) && (jeu[i]!=0))
            return jeu[i];
    }
    //Verifie diagonale
    if((jeu[0]==jeu[4] && jeu[4]==jeu[8]) && (jeu[0]!=0))
        return jeu[0];
    if((jeu[2]==jeu[4] && jeu[4]==jeu[6]) && (jeu[2]!=0))
        return jeu[2];

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
int placerPion(int position, int * jeu, int joueur)
{
    position = position - 1;
    if (jeu[position]==0)
    {
        jeu[position] = joueur;
        return 0;
    }
    else
    {
        return -1;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
int joueurSuivant(int joueur)
{
    if (joueur == 1)
    {
        return 2;
    }
    return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
void printScore(int* score)
{
    set_cursor(12, 58);
    printf("%d",score[1]);
    set_cursor(12, 68);
    printf("%d", score[2]);
}

//////////////////////////////////////////////////////////////////////////////////////////
void printScreen(char* fileName)
{
    stat_t st;
    clear_display();
    set_cursor(0, 0);
    get_stat(fileName, &st);
    char data[st.size];
    read_file(fileName, (uint8_t*)data);
    puts(data);
}
