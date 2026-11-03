#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

typedef struct station {
    int qtd_pessoas_fila;
    int qtd_assentos_disponiveis;
    int qtd_assentos_ocupados;
    int passageiros_embarcando;
    pthread_mutex_t mutex;
    pthread_cond_t cond_passageiros;
    pthread_cond_t cond_vagao;
} station;

void station_init(struct station *station);
void station_load_car(struct station *station, int count);
void station_wait_for_car(struct station *station);
void station_on_board(struct station *station);

#endif