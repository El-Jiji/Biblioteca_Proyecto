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

