#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define THREAD_MAX_NUMBER 5 // to change

typedef struct thread_struct
{
    bool *tab;
    int index;
    int len;
    int iterator;
} thread_struct;

void *crible_range(void *arguments)
{
    struct thread_struct *args = arguments;
    for (int i = args->index; i < args->index + args->len; i += args->iterator)
    {
        args->tab[i] = false;
    }
}

bool *crible_parallele(int n)
{
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    for (int i = 0; i <= n; i++)
        is_prime[i] = true;

    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= n; i++)
    {
        if (is_prime[i])
        {
            pthread_t thread_tab[THREAD_MAX_NUMBER];
            int val = i * i;
            int step = i;
            int nb_elem = (n - val) / step + 1; // Nombre total d'éléments à marquer
            int len_per_thread = nb_elem / THREAD_MAX_NUMBER;
            int remainder = nb_elem % THREAD_MAX_NUMBER;

            // Allouer un tableau d'arguments pour chaque thread
            thread_struct *t_args = malloc(THREAD_MAX_NUMBER * sizeof(thread_struct));

            for (int thread_number = 0; thread_number < THREAD_MAX_NUMBER; thread_number++)
            {
                t_args[thread_number].tab = is_prime;
                t_args[thread_number].index = val + thread_number * len_per_thread * step;
                t_args[thread_number].iterator = step;
                t_args[thread_number].len = len_per_thread;
                if (thread_number < remainder)
                    t_args[thread_number].len++; // Distribuer le reste

                pthread_create(&thread_tab[thread_number], NULL, &crible_range, (void *)&t_args[thread_number]);
            }

            // Attendre la fin de tous les threads
            for (int thread_number = 0; thread_number < THREAD_MAX_NUMBER; thread_number++)
            {
                pthread_join(thread_tab[thread_number], NULL);
            }

            free(t_args); // Libérer la mémoire allouée pour les arguments
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
    bool *is_prime = crible_parallele(n);
    afficher_premiers(is_prime, n);
    free(is_prime);
    return 0;
}