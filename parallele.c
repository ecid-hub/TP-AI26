#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define THREAD_MAX_NUMBER 7

typedef struct thread_struct
{
    bool *tab;
    long long start_index;
    long long max_n;
    long long step;
} thread_struct;

// Thread function
void *crible_range(void *arguments)
{
    thread_struct *args = (thread_struct *)arguments;

    for (long long i = args->start_index; i <= args->max_n; i += args->step)
    {
        args->tab[i] = false;
    }
    return NULL;
}

bool *crible_parallele(long long n)
{
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    if (is_prime == NULL)
    {
        perror("Erreur allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Optimization of loop to not treat the even numbers
    for (long long i = 0; i <= n; i++)
        if (n % 2 == 0 && n != 0)
        {
            is_prime[i] = false;
        }
        else
        {
            is_prime[i] = true;
        }

    if (n >= 0)
        is_prime[0] = false;
    if (n >= 1)
        is_prime[1] = false;

    for (long long i = 3; i * i <= n; i += 2)
    {
        if (is_prime[i])
        {
            pthread_t thread_tab[THREAD_MAX_NUMBER];
            thread_struct t_args[THREAD_MAX_NUMBER];

            long long first_multiple = i * i;
            long long global_step = i * THREAD_MAX_NUMBER;

            for (int thread_number = 0; thread_number < THREAD_MAX_NUMBER; thread_number++)
            {
                t_args[thread_number].tab = is_prime;
                t_args[thread_number].max_n = n;
                t_args[thread_number].start_index = first_multiple + (thread_number * i);
                t_args[thread_number].step = global_step;

                if (t_args[thread_number].start_index <= n)
                {
                    pthread_create(&thread_tab[thread_number], NULL, &crible_range, (void *)&t_args[thread_number]);
                }
                else
                {
                    thread_tab[thread_number] = 0;
                }
            }

            for (int thread_number = 0; thread_number < THREAD_MAX_NUMBER; thread_number++)
            {
                if (thread_tab[thread_number] != 0)
                {
                    pthread_join(thread_tab[thread_number], NULL);
                }
            }
        }
    }

    return is_prime;
}

void afficher_premiers(bool *is_prime, long long n)
{
    printf("Nombres premiers jusqu'à %lld :\n", n);
    for (long long i = 2; i <= n; i++)
    {
        if (is_prime[i])
            printf("%lld ", i);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    long long n;
    if (argc != 2)
    {
        n = 20;
    }
    else
    {
        n = atoll(argv[1]);
    }

    bool *is_prime = crible_parallele(n);

    if (n <= 1000)
    {
        afficher_premiers(is_prime, n);
    }
    else
    {
        printf("Calcul terminé avec succès pour n = %lld.\n", n);
    }

    free(is_prime);
    return 0;
}