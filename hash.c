#define _POSIX_C_SOURCE 200809L 
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#define TAM_INICIAL 101
#define FACTOR_CARGA 0.5
#define FACTOR_CARGA_BORRADO FACTOR_CARGA * 0.5

typedef enum estado { OCUPADO, VACIO, BORRADO } estado_t;

typedef struct elemento {
	void * dato;
	char * clave;
	estado_t estado;
}elemento_t;

typedef struct hash {
	size_t cantidad;
	size_t capacidad;
	size_t borrados;
	elemento_t* tabla;
	hash_destruir_dato_t funcion_destruir;
}hash_t;


typedef struct hash_iter {
	const hash_t * hash;
	elemento_t * actual;
	size_t posicion;
}hash_iter_t;

unsigned long hashing(const char *str) {
	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

size_t hash_posicion(const hash_t *hash, const char *clave) {
	return (size_t) hashing(clave) % hash->capacidad;
}

elemento_t* hash_obtener_elemento(const hash_t *hash, const char *clave) {
	size_t posicion = hash_posicion(hash, clave);

	for(size_t i = 0; i < hash->capacidad; i++) {
		size_t nueva_posicion = (posicion + i) % hash->capacidad;

		if(hash->tabla[nueva_posicion].estado == VACIO)
			return NULL;
		if(strcmp(hash->tabla[nueva_posicion].clave, clave) == 0)
			return &hash->tabla[nueva_posicion];
	}
	return NULL;
}

void hash_valores_iniciales(hash_t * hash) {
	for(size_t i = 0; i < hash->capacidad; i++) {
		hash->tabla[i].clave = NULL;
		hash->tabla[i].dato = NULL;
		hash->tabla[i].estado = VACIO;
	}
}

hash_t * hash_crear(hash_destruir_dato_t destruir_dato) {
	
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash) return NULL;
	
	hash->funcion_destruir = destruir_dato;
	hash->tabla = calloc(TAM_INICIAL,sizeof(elemento_t));
	
	if(!hash->tabla) {
		free(hash);
		return NULL;
	}
	
	hash->capacidad = TAM_INICIAL;
	hash->cantidad = 0;
	hash->borrados = 0;
	hash_valores_iniciales(hash);
	
	return hash;	
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	elemento_t* elemento = hash_obtener_elemento(hash,clave);
	size_t posicion = hash_posicion(hash, clave);

	if(elemento){
		if(hash->funcion_destruir)
			hash->funcion_destruir(dato);
		hash->tabla[posicion].dato = dato;
		return true;
	}
	
	for(size_t i = 0; i < hash->capacidad; i++) {
		size_t nueva_posicion = (i + posicion) % hash->capacidad;
		if(hash->tabla[nueva_posicion].estado == VACIO){
			elemento_t* elemento = malloc(sizeof(elemento_t));
			if(!elemento) return false;
			
			elemento->clave = strdup(clave);
			elemento->dato = dato;
			elemento->estado = OCUPADO;

			hash->tabla[nueva_posicion] = *elemento;
			hash->cantidad++;
			return true;
		}
	}
	return false;
}



void *hash_borrar(hash_t *hash, const char *clave) {
	elemento_t * elemento = hash_obtener_elemento(hash, clave);

	if(!elemento) return NULL;
	elemento->estado = BORRADO;
	elemento->clave = NULL;
	void * dato = elemento->dato;
	if(hash->funcion_destruir)
		hash->funcion_destruir(elemento->dato);
	free(elemento->clave);
	hash->borrados++;
	return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	elemento_t * elemento = hash_obtener_elemento(hash, clave);
	if(!elemento) return NULL; 
	return elemento->dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	return hash_obtener(hash, clave) != NULL;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad - hash->borrados;
}

void hash_destruir(hash_t *hash) {
	for(size_t i = 0; i < hash->capacidad; i++) {
		if(hash->tabla[i].estado != OCUPADO)
			continue;
		if(hash->funcion_destruir) 
			hash->funcion_destruir(hash->tabla[i].dato);
		free(hash->tabla[i].clave);
	}
	free(hash->tabla);
	free(hash);
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {
	hash_iter_t * iter = malloc(sizeof(hash_iter_t));
	if(!iter) return NULL;
	iter->hash = hash;
	iter->posicion = 0;
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
	if(hash_iter_al_final(iter)) return false;
	iter->posicion++;
	return true;
}
const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	if(iter->posicion >= iter->hash->cantidad) return NULL;
	return iter->hash->tabla[iter->posicion].clave;
}
bool hash_iter_al_final(const hash_iter_t *iter) {
	return iter->posicion >= iter->hash->cantidad;
}
void hash_iter_destruir(hash_iter_t* iter) {
	free(iter);	
}
