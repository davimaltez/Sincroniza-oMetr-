#include <pthread.h>
#include <stdio.h>

typedef struct station{

    int qtd_pessoas_fila;
    int qtd_assentos_disponiveis;
    int qtd_assentos_ocupados;
    pthread_mutex_t mutex;
    pthread_cond_t cond_passageiros;
    pthread_cond_t cond_vagao;

} station;


void station_init(station *s){
    s->qtd_pessoas_fila = 0;
    s->qtd_assentos_disponiveis = 0;
    s->qtd_assentos_ocupados = 0;

    pthread_mutex_init(&s->mutex, NULL);

    pthread_cond_init(&s->cond_passageiros,NULL);
    pthread_cond_init(&s->cond_vagao,NULL);

}