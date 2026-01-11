#include <stdio.h>
#include <stdlib.h>

void mostrarDatosBiblioteca() {
    FILE *archivo;
    char linea[256];

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    archivo = fopen("datos/biblioteca.txt", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo de datos de la biblioteca.\n");
        return;
    }

    printf("|======= Datos de la biblioteca =======|\n");

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%s", linea);
    }

    printf("\n=============================================\n");

    fclose(archivo);
}
