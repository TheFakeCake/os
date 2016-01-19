#include "ulibc.h"

#define BUFFER_SIZE 512

int get_nb_args(char* str);
void print_help();


void main()
{
    char bufferInput[BUFFER_SIZE];

    while(true)
    {
        printf("\\kernel $ :");
        //Lecture des entrees
        gets(bufferInput);

        //Split des argments entre
        int nb_args = get_nb_args(bufferInput);
        char tab_args[nb_args][BUFFER_SIZE];
        split(bufferInput, ' ', (char*)tab_args, nb_args, BUFFER_SIZE);

        if(strcmp(tab_args[0], "ls"))
        {
            if(nb_args != 1)
            {
                puts("\tErreur d'arguments\n");
                puts("\tls : liste tous les fichiers du systeme de fichiers\n");
            }
            else
            {
                file_iterator_t it = get_file_iterator();
                stat_t stat;
                char name [33];
                while (get_next_file(name, &it))
                {
                    get_stat(name, &stat);
                    printf("\t%s\t%d [Bytes]\n", name, stat.size);
                }
            }
            continue;
        }



        if(strcmp(tab_args[0], "cat"))
        {
            if(nb_args != 2)
            {
                puts("\tErreur d'arguments\n");
                puts("\tcat <file> : affiche le contenu du fichier file\n");
            }
            else
            {
                stat_t st;
                if(get_stat(tab_args[1], &st) == -1)
                {
                    printf("\tLe fichier %s n'existe pas\n", tab_args[1]);
                }
                else
                {
                    char data[st.size];
                    read_file(tab_args[1], data);
                    puts(data);
                    puts("\n");
                }
            }
            continue;
        }



        if(strcmp(tab_args[0], "rm"))
        {
            if(nb_args != 2)
            {
                puts("\tErreur d'arguments\n");
                puts("\trm <file> : efface le fichier file\n");
            }
            else
            {
                if(remove_file(tab_args[1]) == -1)
                {
                    printf("\tLe fichier %s n'existe pas\n", tab_args[1]);
                }
            }
            continue;
        }



        if(strcmp(tab_args[0], "run"))
        {
            if(nb_args != 2)
            {
                puts("\tErreur d'arguments\n");
                puts("\trun <file> : execute le fichier file\n");
            }
            else
            {
                exec(tab_args[1]);
            }
            continue;
        }



        if(strcmp(tab_args[0], "ticks"))
        {
            if(nb_args != 1)
            {
                puts("\tErreur d'arguments\n");
                puts("\tticks : affiche le nombre de ticks courant\n");
            }
            else
            {
                printf("\t%d\n", get_ticks());
            }
            continue;
        }



        if(strcmp(tab_args[0], "sleep"))
        {
            if(nb_args != 2)
            {
                puts("\tErreur d'arguments\n");
                puts("\tsleep <N> : attend pendant N milli-secondes\n");
            }
            else
            {
                sleep(atoi(tab_args[1]));
            }
            continue;
        }



        if(strcmp(tab_args[0], "exit"))
        {
            if(nb_args != 1)
            {
                puts("\tErreur d'arguments\n");
                puts("\texit : sort du shell (meme comportement que la commande exit de bash)\n");
            }
            else
            {
                exit();
            }
            continue;
        }



        if(strcmp(tab_args[0], "help"))
        {
            print_help();
            continue;
        }
    }
}



void print_help()
{
    puts("\tls : liste tous les fichiers du systeme de fichiers\n");
    puts("\tcat <file> : affiche le contenu du fichier file\n");
    puts("\trm <file> : efface le fichier file\n");
    puts("\trun <file> : execute le fichier file\n");
    puts("\tticks : affiche le nombre de ticks courant\n");
    puts("\tsleep <N> : attend pendant N milli-secondes\n");
    puts("\texit : sort du shell (meme comportement que la commande exit de bash)\n");
    puts("\thelp : affiche la liste des commandes disponibles\n");
}

int get_nb_args(char* args)
{
    int i = 0;
    int nb_args;
    if(args[0] == '\n')
    {
    	nb_args = 0;
    }
    else
    {
    	nb_args = 1;
    }
    while(args[i] != 0)
    {
        if(args[i] == ' ')
        {
            nb_args++;
        }
        i++;
    }
    return nb_args;
}