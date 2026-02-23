include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <stdint.h>

#define TAM_BLOQUE sizeof(bloque_t)

typedef struct bloque{
    size_t size;
    uint8_t estado; // Si esta en 1 esta libre si esta en 0 esta ocupado
    struct bloque *siguiente;
}bloque_t;

bloque_t *lista = NULL;

void *util_malloc(size_t size){
    bloque_t *bloque;
    bloque = sbrk(size + TAM_BLOQUE);

    if(bloque == (void*)-1){
        fprintf(stderr, "\nNo hay memoria suficiente para pedir");
        return NULL;
    }

    bloque->size = size;
    bloque->estado = 0;
    bloque->siguiente = lista;

    lista = bloque;

    return (void*)(bloque + 1);
}

void util_free(void *ptr){
    if(!ptr){
        fprintf(stderr, "\nError al liberar memoria");
        return;
    }

    bloque_t *bloque = (bloque_t*)ptr - 1;
    bloque->estado = 1;

}

void util_estado(void){
    bloque_t *actual = lista;
    uint8_t f = 0; // Si esta en 0 No se imprimio el estado de bloque
    uint8_t r = 0; // Si esta en 0 Imprime el no hay bloques
    printf("\n===== [ Estado de los bloques ] =====\n\n");
    while(actual){

        if(f == 1){
            printf("Bloque en %p | size: %zu | estado: %s\n", (void*)actual, actual->size, actual->estado ? "LIBRE" : "OCUPADO");
            actual = actual->siguiente;
            if(r == 0){
                r = 1;
            }
            continue;
        }

        if(f == 0){
            printf("Bloque en %p | size: %zu | estado: %s\n", (void*)actual, actual->size, actual->estado ? "LIBRE" : "OCUPADO");
            f = 1;
            actual = actual->siguiente;
            if(r == 0){
                r = 1;
            }
            continue;
        }

    }
    if(r == 0){
        printf("\nNo hay bloques actualmente\n");
    }
}

int main(){
    char *a = util_malloc(20);

    util_estado();

    util_free(a);
    return 0;
}
