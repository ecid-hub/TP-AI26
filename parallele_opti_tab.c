#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define THREAD_MAX_NUMBER 7

// Struct used in thread instanctiation
// Contain everything to explore a part of a tab concurrently
typedef struct thread_struct
{
    bool *tab;
    long long start_index;
    long long max_n;
    long long step;
} thread_struct;

pthread_t thread_tab[THREAD_MAX_NUMBER];
thread_struct t_args[THREAD_MAX_NUMBER];

// Thread function
// In the table, check from start_index to max_n with steps.
// Set false any corresponding value
void *crible_range(void *arguments)
{
    thread_struct *args = (thread_struct *)arguments;

    for (long long i = args->start_index; i <= args->max_n; i += args->step)
    {
        args->tab[i] = false;
    }
    return NULL;
}

// Create the is_prime[n] concurrently
// Allocate is_prime, set 0, 1 and even (except 2) numbers to false
// Then go over each value and concurrently mark as false the multiples
bool *crible_parallele(long long n)
{
    if (n < 2)
        return NULL; // Handling edge case

    // Size to store all odds number >= 3
    long long max_index = (n - 3) / 2;
    if (max_index < 0)
        max_index = 0;

    bool *is_prime = (bool *)malloc((max_index + 1) * sizeof(bool));
    if (is_prime == NULL)
    {
        perror("Erreur allocation mémoire");
        exit(EXIT_FAILURE);
    }

    for (long long i = 0; i <= max_index; i++)
        is_prime[i] = true;

    // i is the memory index, but the true number is k=2*i+3
    // That's why we have a weird stop point
    for (long long i = 0; (2 * i + 3) * (2 * i + 3) <= n; i++)
    {
        if (is_prime[i])
        {
            long long k = 2 * i + 3;

            long long first_index = k * k;

            long long first_index_number = (first_index - 3) / 2;

            long long local_step = k;
            long long global_step = local_step * THREAD_MAX_NUMBER;

            for (int thread_number = 0; thread_number < THREAD_MAX_NUMBER; thread_number++)
            {
                t_args[thread_number].tab = is_prime;
                t_args[thread_number].max_n = max_index;

                t_args[thread_number].start_index = first_index_number + (thread_number * local_step);
                t_args[thread_number].step = global_step;

                if (t_args[thread_number].start_index <= max_index)
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

// Display until n the elements of prime[length]
void afficher_premiers(bool *is_prime, long long n)
{
    if (n >= 2)
        printf("2 ");

    long long max_index = (n - 3) / 2;
    for (long long i = 0; i <= max_index; i++)
    {
        if (is_prime[i])
        {
            printf("%lld ", 2 * i + 3);
        }
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    long long n;

    // Argc/argv treatement
    if (argc != 2)
    {
        n = 20;
    }
    else
    {
        n = atoll(argv[1]);
    }

    bool *is_prime = crible_parallele(n);

    // Don't display if n is too high
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