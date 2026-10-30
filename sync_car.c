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
    s->passageiros_embarcando = 0;

    pthread_mutex_init(&s->mutex, NULL);

    pthread_cond_init(&s->cond_passageiros,NULL);
    pthread_cond_init(&s->cond_vagao,NULL);

}

//Passageiro esperando

void station_wait_for_car(station *s){

    pthread_mutex_lock(&s->mutex);

    s->qtd_pessoas_fila++;

    
    while(s->qtd_assentos_disponiveis == 0){

        pthread_cond_wait(&s->cond_passageiros, &s->mutex);

    }

    s->qtd_pessoas_fila --;
    s->qtd_assentos_disponiveis--;
    s->passageiros_embarcando++;

    pthread_mutex_unlock(&s->mutex);

    return;
}

void station_on_board(station *s){

    pthread_mutex_lock(&s->mutex);

    s->passageiros_embarcando--;

    pthread_mutex_unlock(&s->mutex);


    //Sinaliza que o vagão pode seguirr
    if(s->qtd_pessoas_fila == 0 && s->passageiros_embarcando == 0){

        pthread_cond_signal(&s->cond_vagao);
    }
}
