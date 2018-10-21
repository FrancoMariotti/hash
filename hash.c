#define TAM_INICIAL	100
typedef enum estado { OCUPADO, VACIO, BORRADO } estado_t;

typedef struct hash {
	size_t cantidad;
	size_t capacidad;
	elemento_t* datos;
	hash_destruir_dato_t funcion_destruir;
}hash_t;

typedef struct elemento {
	void * dato;
	char * clave;
	estado_t estado;
}elemento_t;

unsigned long hash(unsigned char *str) {
	unsigned long hash = 5381;
	int c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

size_t hash_posicion(const hash_t *hash, const char *clave) {
	return (size_t) hash(clave) % hash->capacidad;
}

hash_t * hash_crear(hash_destruir_dato_t destruir_dato) {
	
	hash_t* hash = malloc(sizeof(hash_t));
	
	if(!hash) return NULL;
	
	hash->funcion_destruir = destruir_dato;
	
	hash->datos = malloc(sizeof(elemento_t) * TAM_INICIAL);
	
	if(!hash->datos) {
		free(hash);
		return NULL;
	}
	
	hash->capacidad = TAM_INICIAL;
	hash->cantidad = 0;
	
	return hash;	
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {

}

elemento_t * hash_obtener_elemento(const hash_t *hash, const char *clave) {
	if(!hash) return NULL;
	size_t posicion = hash_posicion(hash, clave);

	for(size_t i = 0; i < hash->capacidad; i++) {
		
		size_t nueva_posicion = (posicion + i) % hash->capacidad;

		if(!hash->datos[nueva_posicion])
			return NULL;
		if(strcmp(hash->datos[nueva_posicion]->clave, clave) == 0)
			return hash->datos[nueva_posicion];
	}
}

void *hash_borrar(hash_t *hash, const char *clave) {
	if(!hash || !hash_pertenece(hash, clave)) return NULL;
	size_t posicion = hash_posicion(hash, clave);
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	if(!hash) return NULL;

	elemento_t *elemento = hash_obtener_elemento(hash, clave);

	if(!elemento) return NULL; 
	return elemento->dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	if(!hash) return false;
	return hash_obtener(hash, clave) != NULL;
}

size_t hash_cantidad(const hash_t *hash) {

}

void hash_destruir(hash_t *hash) {
	if(!hash) return;

	for(size_t i = 0; i < hash->capacidad && hash->funcion_destruir; i++) {
		if(hash->datos[i]->estado == OCUPADO) 
			funcion_destruir(hash->datos[i].dato);
	}
	free(hash->datos);
	free(hash);
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {

}

bool hash_iter_avanzar(hash_iter_t *iter) {

}
const char *hash_iter_ver_actual(const hash_iter_t *iter) {

}
bool hash_iter_al_final(const hash_iter_t *iter) {

}
void hash_iter_destruir(hash_iter_t* iter) {

}