typedef enum estado { OCUPADO, VACIO, BORRADO } estado_t;

typedef struct hash {
	size_t cantidad;
	size_t capacidad;
	elemento_t * datos;
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

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {

}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {

}

void *hash_borrar(hash_t *hash, const char *clave) {

}

void *hash_obtener(const hash_t *hash, const char *clave) {

}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	if(!hash) return false;
	unsigned long posicion = hash(clave) % hash->capacidad;

	for(int i = 0; i < hash->capacidad; i++) {
		if(hash->datos[posicion + i]->estado == VACIO)
			return false;
		if(strcmp(hash->datos[posicion + i]->clave, clave) == 0)
			return true;
	}
}

size_t hash_cantidad(const hash_t *hash) {

}

void hash_destruir(hash_t *hash) {

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
