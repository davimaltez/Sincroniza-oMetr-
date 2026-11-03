#include <pthread.h>
#include <stdio.h>
#include "sync_car.h"

void station_init(station *station){
    station->qtd_pessoas_fila = 0;
    station->qtd_assentos_disponiveis = 0;
    station->passageiros_embarcando = 0;

    pthread_mutex_init(&station->mutex, NULL);

    pthread_cond_init(&station->cond_passageiros,NULL);
    pthread_cond_init(&station->cond_vagao,NULL);

}

//Passageiro esperando

void station_wait_for_car(station *station){

    pthread_mutex_lock(&station->mutex);

    station->qtd_pessoas_fila++;

    
    while(station->qtd_assentos_disponiveis == 0){

        pthread_cond_wait(&station->cond_passageiros, &station->mutex);

    }

    station->qtd_pessoas_fila --;
    station->qtd_assentos_disponiveis--;
    station->passageiros_embarcando++;

    pthread_mutex_unlock(&station->mutex);

    return;
}

void station_on_board(station *station){

    pthread_mutex_lock(&station->mutex);

    station->passageiros_embarcando--;

    
    
    // Condição de partida é quando está sem vagas OU a fila está vazia) + ninguém no corredor
    if((station->qtd_assentos_disponiveis == 0 || station->qtd_pessoas_fila == 0) && station->passageiros_embarcando == 0){
        
        pthread_cond_signal(&station->cond_vagao);
        
    }

    pthread_mutex_unlock(&station->mutex);
}


//count = quantidade de assentos disponíveis no carro
void station_load_car(station *station, int count){

    pthread_mutex_lock(&station->mutex);

    station->qtd_assentos_disponiveis = count;

    pthread_cond_broadcast(&station->cond_passageiros);
    
    while((station->qtd_assentos_disponiveis > 0 && station->qtd_pessoas_fila > 0) || station->passageiros_embarcando > 0){

        pthread_cond_wait(&station->cond_vagao, &station->mutex);

    }

    station->qtd_assentos_disponiveis = 0;
    
    pthread_mutex_unlock(&station->mutex);  

    return;

}