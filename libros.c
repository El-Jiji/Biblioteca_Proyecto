#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Libro {
    int id;
    char titulo[100];
    char autor[100];
    int disponible; // 1: Disponible, 0: No disponible
};

void registrarLibro() {
    struct Libro nuevoLibro;
    FILE *archivo;

    printf("\n|======= Registro de nuevo libro =======|\n");
    
    printf("Ingrese ID del libro: ");
    scanf("%d", &nuevoLibro.id);
    getchar(); // Limpiar buffer

    printf("Ingrese Titulo: ");
    scanf(" %[^\n]", nuevoLibro.titulo);

    printf("Ingrese Autor: ");
    scanf(" %[^\n]", nuevoLibro.autor);

    nuevoLibro.disponible = 1; // Por defecto disponible

    archivo = fopen("libros.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de libros.\n");
        return;
    }

    // Guardar en formato: ID;Titulo;Autor;Disponibilidad
    fprintf(archivo, "%d;%s;%s;%d\n", nuevoLibro.id, nuevoLibro.titulo, nuevoLibro.autor, nuevoLibro.disponible);
    fclose(archivo);

    printf("Libro registrado exitosamente.\n");
}

void mostrarCatalogo() {
    struct Libro libro;
    FILE *archivo;
    char linea[256];

    printf("\n|======= Mostrar catalogo de libros =======|\n");

    archivo = fopen("libros.txt", "r");
    if (archivo == NULL) {
        printf("No hay libros registrados o error al abrir archivo.\n");
        return;
    }

    printf("%-5s %-30s %-20s %-15s\n", "ID", "Titulo", "Autor", "Estado");
    printf("--------------------------------------------------------------------------\n");

    while (fgets(linea, sizeof(linea), archivo)) {
        // Parsear la linea: ID;Titulo;Autor;Disponibilidad
        // Usamos sscanf o strtok. Strtok es mas facil para delimitadores.
        char *token;
        
        token = strtok(linea, ";");
        if(token) libro.id = atoi(token);
        
        token = strtok(NULL, ";");
        if(token) strcpy(libro.titulo, token);
        
        token = strtok(NULL, ";");
        if(token) strcpy(libro.autor, token);
        
        token = strtok(NULL, ";");
        if(token) libro.disponible = atoi(token);

        printf("%-5d %-30s %-20s %-15s\n", 
               libro.id, 
               libro.titulo, 
               libro.autor, 
               libro.disponible ? "Disponible" : "Prestado");
    }

    fclose(archivo);
}

void cambiarDisponibilidad() {
    int idBuscar, encontrado = 0;
    struct Libro libro;
    FILE *archivo, *temp;
    char linea[256];

    printf("\n|======= Cambiar disponibilidad de libro =======|\n");
    printf("Ingrese ID del libro: ");
    scanf("%d", &idBuscar);

    archivo = fopen("libros.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("Error al crear archivo temporal.\n");
        fclose(archivo);
        return;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        char lineaCopia[256];
        strcpy(lineaCopia, linea); // Copia para no dañar original al parsear

        char *token = strtok(lineaCopia, ";");
        int idActual = atoi(token);

        // Reconstruimos los datos para guardarlos en struct si es necesario
        // Pero mas facil: si es el ID, cambiamos el ultimo campo.
        // Si no es el ID, copiamos la linea original.
        
        if (idActual == idBuscar) {
            // Parsear resto para mostrar info y cambiar estado
            token = strtok(NULL, ";"); strcpy(libro.titulo, token);
            token = strtok(NULL, ";"); strcpy(libro.autor, token);
            token = strtok(NULL, ";"); libro.disponible = atoi(token);

            libro.disponible = !libro.disponible; // Cambiar estado
            encontrado = 1;

            fprintf(temp, "%d;%s;%s;%d\n", idActual, libro.titulo, libro.autor, libro.disponible);
            printf("Estado del libro '%s' cambiado a: %s\n", libro.titulo, libro.disponible ? "Disponible" : "Prestado");
        } else {
            fprintf(temp, "%s", linea); // Escribir linea original
        }
    }

    fclose(archivo);
    fclose(temp);

    remove("libros.txt");
    rename("temp.txt", "libros.txt");

    if (!encontrado) {
        printf("Libro con ID %d no encontrado.\n", idBuscar);
    }
}
