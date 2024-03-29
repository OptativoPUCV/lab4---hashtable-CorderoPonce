#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {

  unsigned short indice = hash(key, map->capacity);
  if (map->buckets[indice] == NULL){
    map->buckets[indice] = (Pair *) malloc (sizeof(Pair *));
    map->buckets[indice]->key = key;
    map->buckets[indice]->value = value;
  }
  else if (map->buckets[indice]->key == NULL){
    map->buckets[indice]->key = key;
    map->buckets[indice]->value = value;
    return;
  }
  else {
    while (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL)
    {
      indice = (indice + 1) % map->capacity;
    }
    map->buckets[indice] = (Pair *) malloc (sizeof(Pair *));
    map->buckets[indice]->key = key;
    map->buckets[indice]->value = value;
  }
  map->size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
  Pair **old_buckets = map->buckets;
  int capacidad = map->capacity;
  map->capacity *= 2;
  map->size = 0;
  map->buckets = calloc(map->capacity, sizeof(Pair *));
  for (int i = 0; i < capacidad; i++) {
    if (old_buckets[i] != NULL) {
      insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
    }
  }
}


HashMap * createMap(long capacity) {
  
  HashMap * mapa = (HashMap *) malloc (sizeof(HashMap));
  mapa->buckets = (Pair **) calloc (capacity,sizeof(Pair *));
  
  mapa->capacity = capacity;
  mapa->size = 0;
  mapa->current = -1;

  return mapa;
}

void eraseMap(HashMap * map,  char * key) {  
  Pair *casilla = searchMap(map, key);

  if (casilla != NULL){
    casilla->key = NULL;
    map->size--;
  }
}

Pair * searchMap(HashMap * map,  char * key) {
  int indice = hash(key, map->capacity);
  map->current = indice;

  if (map->buckets[indice] == NULL) return NULL;
  else if (strcmp(map->buckets[indice]->key, key) != 0){
    while (strcmp(map->buckets[indice]->key, key) != 0){
      indice = (indice + 1) % map->capacity;
      map->current = indice;
      if (map->buckets[indice] == NULL) return NULL;
    }
  }
  return map->buckets[indice];
}

Pair * firstMap(HashMap * map) {
  for (int i = 0 ; i < map->capacity ; i++){
    map->current = i;
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      return map->buckets[i];
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  if (map->current == map->capacity) return NULL;
  
  for (int i = map->current + 1 ; i < map->capacity ; i++){
    map->current = i;
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
      return map->buckets[i];
    }
  }
  return NULL;
}
