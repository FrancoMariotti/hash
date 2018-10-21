#define FACTOR_CARGA 0.5
#define FACTOR_CARGA_BORRADO FACTOR_CARGA * 0.5
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
	
	hash->datos = calloc(TAM_INICIAL,sizeof(elemento_t));
	
	if(!hash->datos) {
		free(hash);
		return NULL;
	}
	
	hash->capacidad = TAM_INICIAL;
	hash->cantidad = 0;
	
	return hash;	
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if(hash_obtener_elemento(hash,clave)){
		hash->datos[posicion].clave = strdup(clave);
		hash->datos[posicion].dato = dato;
	}
	else{	
		size_t posicion = hash_posicion(hash,clave);
		
		for(size_t i = posicion; i < hash->capacidad; i++) {
			
			if(hash->datos[posicion] == NULL){
				elemento_t* elemento = malloc(sizeof(elemento_t));
			
				if(!elemento) return NULL;
				
				elemento->clave = clave;
				elemento->dato = dato;
				elemento->estado = OCUPADO;

				hash->datos[posicion]=elemento;
			}`
		}
	}
 
	hash->cantidad++;
	
	return true;
}

elemento_t* hash_obtener_elemento(const hash_t *hash, const char *clave) {
	size_t posicion = hash_posicion(hash, clave);

	for(size_t i = 0; i < hash->capacidad; i++) {
		
		size_t nueva_posicion = (posicion + i) % hash->capacidad;

		if(!hash->datos[nueva_posicion])
			return NULL;
		if(strcmp(hash->datos[nueva_posicion].clave, clave) == 0)
			return hash->datos[nueva_posicion];
	}
}

void *hash_borrar(hash_t *hash, const char *clave) {
	elemento_t * elemento = hash_obtener_elemento(hash, clave);

	if(!elemento) return NULL;
	elemento->estado = BORRADO;
	elemento->clave = NULL;
	if(hash->funcion_destruir)
		funcion_destruir(elemento->dato);
	hash->cantidad--;

	if((float) hash->cantidad / hash->capacidad <= FACTOR_CARGA)
		hash_redimensionar(hash, capacidad->capacidad * FACTOR_CARGA_BORRADO);

	free(elemento->clave);

}

void *hash_obtener(const hash_t *hash, const char *clave) {
	elemento_t elemento = hash_obtener_elemento(hash, clave);
	if(!elemento) return NULL; 
	return elemento.dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	return hash_obtener(hash, clave) != NULL;
}

size_t hash_cantidad(const hash_t *hash) {

}

void hash_destruir(hash_t *hash) {
	for(size_t i = 0; i < hash->capacidad; i++) {
		if(hash->datos[i]->estado != OCUPADO)
			continue;
		if(hash->funcion_destruir) 
			funcion_destruir(hash->datos[i].dato);
		free(hash->datos[i].clave);
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
