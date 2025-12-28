#include <stdio.h>
#include <stdlib.h>

// Incluimos los archivos .c directamente para que funcione todo junto sin .h
// Esto permite compartir las structs y funciones definidas en ellos.
#include "libros.c"
#include "usuarios.c"
#include "prestamos.c"
#include "biblioteca.c"

void pausar() {
    printf("\n|======= ENTER para volver al menu =======|");
    getchar();
    getchar();
}

int main() {
    int opcion;

    do {
        #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

        printf("\n|======= MENU BIBLIOTECA =======|\n");
        printf("1. Mostrar catalogo\n");
        printf("2. Registrar usuario\n");
        printf("3. Registrar libro\n");
        printf("4. Prestamo\n");
        printf("5. Devolucion\n");
        printf("6. Datos biblioteca\n");
        printf("7. Cambiar disponibilidad libro (Manual)\n");
        printf("8. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        getchar(); // Consumir newline despues de scanf

        switch (opcion) {
            case 1: mostrarCatalogo(); break;
            case 2: registrarUsuario(); break;
            case 3: registrarLibro(); break;
            case 4: realizarPrestamo(); break;
            case 5: devolverLibro(); break;
            case 6: mostrarDatosBiblioteca(); break;
            case 7: cambiarDisponibilidad(); break;
            case 8: printf("Saliendo...\n"); break;
            default: printf("Opcion invalida\n");
        }
        if (opcion != 8) {
            pausar();
        }
    } while (opcion != 8);

    system("pause");
    return 0;
}
