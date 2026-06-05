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
            // Thread initialization
            pthread_t thread_tab[THREAD_MAX_NUMBER];
            int val = i * i;
            int step = i;
            int nb_elem = (n - i * i + 1) / i;
            int modulo = (nb_elem % (THREAD_MAX_NUMBER - 1));
            for (int thread_number = 0; thread_number < THREAD_MAX_NUMBER; thread_number++)
            {
                thread_struct t_args;
                t_args.tab = is_prime;
                t_args.index = val;
                t_args.iterator = i;
                if (thread_number != THREAD_MAX_NUMBER - 1)
                {
                    t_args.len = (nb_elem / (THREAD_MAX_NUMBER - 1));
                    int is = pthread_create(&thread_tab[i], NULL, &crible_range, (void *)&t_args);
                }
                else
                {
                    if (modulo != 0)
                    {
                        t_args.len = modulo;
                        int is = pthread_create(&thread_tab[i], NULL, &crible_range, (void *)&t_args);
                    }
                }
                val += step * i;
            }

            // Synchro fin
            for (int thread_number = 0; thread_number < THREAD_MAX_NUMBER; thread_number++)
            {
                void *ret;
                int is = pthread_join(thread_tab[thread_number], ret);

                printf("IsS : %i", is);
            }
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