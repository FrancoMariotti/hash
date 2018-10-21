#define _POSIX_C_SOURCE 200809L 
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#define TAM_INICIAL 101
#define FACTOR_CARGA 0.5

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

		if(hash->tabla[nueva_posicion].estado == BORRADO)
			continue;
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

bool hash_redimensionar(hash_t *hash, size_t nueva_capacidad){
	
	elemento_t* vieja_tabla = hash->tabla;
	
	elemento_t* nueva_tabla = malloc(sizeof(elemento_t)*nueva_capacidad);
	
	if(!nueva_tabla)return false;
	
	for(size_t i = 0; i < hash->capacidad; i++) {
		
		if(vieja_tabla[i].estado == OCUPADO){
			size_t nueva_posicion = (size_t) hashing(vieja_tabla[i].clave) % nueva_capacidad;
			nueva_tabla[nueva_posicion] = vieja_tabla[i];
		}
	}
	
	hash->tabla = nueva_tabla;
	
	free(vieja_tabla);
	
	hash->cantidad = hash->cantidad-hash->borrados;
	hash->capacidad = nueva_capacidad;
	
	return true;
}

hash_t * hash_crear(hash_destruir_dato_t destruir_dato) {
	
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash) return NULL;
	
	hash->funcion_destruir = destruir_dato;
	hash->tabla = malloc(TAM_INICIAL * sizeof(elemento_t));
	
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
	
	if((hash->cantidad / hash->capacidad)  >= FACTOR_CARGA){
		if(!hash_redimensionar(hash, hash->capacidad * 2 ))return false;
	}
	
	elemento_t * elemento = hash_obtener_elemento(hash, clave);
	size_t posicion = hash_posicion(hash, clave);

	if(elemento){
		if(hash->funcion_destruir)
			hash->funcion_destruir(hash->tabla[posicion].dato);
		hash->tabla[posicion].dato = dato;
		return true;
	}
	
	for(size_t i = 0; i < hash->capacidad; i++) {
		size_t nueva_posicion = (i + posicion) % hash->capacidad;
		if(hash->tabla[nueva_posicion].estado == VACIO) {
			hash->tabla[nueva_posicion].clave =  strdup(clave);
			hash->tabla[nueva_posicion].dato = dato;
			hash->tabla[nueva_posicion].estado = OCUPADO;

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
	free(elemento->clave);
	elemento->clave = NULL;
	hash->borrados++;
	return elemento->dato;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	elemento_t * elemento = hash_obtener_elemento(hash, clave);
	if(!elemento) return NULL; 
	return elemento->dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	return hash_obtener_elemento(hash, clave) != NULL;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad - hash->borrados;
}

void hash_destruir(hash_t *hash) {
	for(size_t i = 0; i < hash->capacidad; i++) {
		if(hash->tabla[i].estado == VACIO)
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
	iter->posicion = -1;
	hash_iter_avanzar(iter);
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
	if(hash_iter_al_final(iter)) return false;
	for(size_t i = iter->posicion + 1; i < iter->hash->cantidad; i++) {
		if(iter->hash->tabla[i].estado == OCUPADO) {
			iter->posicion = i;
			break;
		}
	}
	return true;
}
const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	return iter->hash->tabla[iter->posicion].clave;
}
bool hash_iter_al_final(const hash_iter_t *iter) {
	return iter->posicion == iter->hash->cantidad - 1;
}
void hash_iter_destruir(hash_iter_t* iter) {
	free(iter);	
}
