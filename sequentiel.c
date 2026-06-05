#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define DefaultInd 20

int main(int argc, char *argv[])
{
    int ind;

    if (argc == 2)
    {
        ind = atoi(argv[1]);
    }
    else
    {
        ind = DefaultInd;
    }

    if (ind < 2)
    {
        printf("Veuillez entrer un nombre supérieur ou égal à 2.\n");
        return 0;
    }

    int tab[ind];

    for (int i = 0; i < ind; i++)
    {
        tab[i] = 1;
    }

    tab[0] = 0;
    tab[1] = 0;

    double limite = sqrt(ind);
    for (int i = 2; i <= limite; i++)
    {
        if (tab[i] == 1)
        {
            for (int j = i * i; j < ind; j += i)
            {
                tab[j] = 0;
            }
        }
    }

    printf("Sur les %i premiers nombres, ceux-là sont premiers :\n", ind);
    for (int i = 0; i < ind; i++)
    {
        if (tab[i] == 1)
        {
            printf("%i ", i);
        }
    }
    printf("\n");
    fflush(stdout);

    return 0;
}