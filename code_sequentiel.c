#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Create a new is_prime[n] table containing bools
// Initially all values are true except 0 and 1
// Force each value, set to false their multiple
// Return the table is_prime[n]
bool *crible_sequentiel(int n)
{
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    for (int i = 0; i <= n; i++)
        is_prime[i] = true;

    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= n; i++)
    {
        if (is_prime[i])
        {
            for (int j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    return is_prime;
}

// Display all values set to true in is_prime[length] until n
void afficher_premiers(bool *is_prime, int n)
{
    printf("Nombres premiers jusqu'à %d :\n", n);
    for (int i = 2; i <= n; i++)
    {
        if (is_prime[i])
            printf("%d ", i);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    long long n;
    // Argc/argv handling
    if (argc != 2)
    {
        n = 20;
    }
    else
    {
        n = atoll(argv[1]);
    }

    bool *is_prime = crible_sequentiel(n);

    // Don't display if too much values
    if (n <= 1000)
    {
        afficher_premiers(is_prime, n);
    }
    else
    {
        printf("Calcul terminé avec succès pour n = %lld.\n", n);
    }

    // free is_prime[] allocated in crible_sequentiel
    free(is_prime);
    return 0;
}