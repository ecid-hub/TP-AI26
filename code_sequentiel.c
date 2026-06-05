#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

int main()
{
    int n = 20;
    bool *is_prime = crible_sequentiel(n);
    afficher_premiers(is_prime, n);
    free(is_prime);
    return 0;
}