#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int size;  // aktualny rozmiar wektora
    int capacity;  // pojemność wektora
    void** data;  // wskaźnik na tablicę przechowującą dane wektora
} myVector;

// Tworzenie nowego wektora
myVector* new_vector() {
    myVector* vector = (myVector*)malloc(sizeof(myVector));
    vector->size = 0;
    vector->capacity = 2;
    vector->data = (void**)malloc(sizeof(void*) * vector->capacity);
    return vector;
}

// Dodawanie nowego elementu do wektora
void vector_push(myVector* vector, void* value) {
    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        vector->data = (void**)realloc(vector->data, sizeof(void*) * vector->capacity);
    }
    vector->data[vector->size] = value;
    vector->size++;
}

// Usuwanie wektora i zwalnianie pamięci
void delete_vector(myVector* vector) {
    free(vector->data);
    free(vector);
}

void* vector_get(myVector* vector, int index) {
    if (index >= vector->size || index < 0) {
        printf("Index out of bounds!\n");
        return NULL;
    }
    return vector->data[index];
}