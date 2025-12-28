#include <stdio.h>
#include <stdlib.h>

void mostrarCatalogo();
void registrarUsuario();
void registrarLibro();
void realizarPrestamo();
void devolverLibro();
void mostrarDatosBiblioteca();

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
        #endif

        printf("\n|======= MENU BIBLIOTECA =======|\n");
        printf("1. Mostrar catalogo\n");
        printf("2. Registrar usuario\n");
        printf("3. Registrar libro\n");
        printf("4. Prestamo\n");
        printf("5. Devolucion\n");
        printf("6. Datos biblioteca\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        getchar();

        switch (opcion) {
            case 1: mostrarCatalogo(); break;
            case 2: registrarUsuario(); break;
            case 3: registrarLibro(); break;
            case 4: realizarPrestamo(); break;
            case 5: devolverLibro(); break;
            case 6: mostrarDatosBiblioteca(); break;
            case 7: printf("Saliendo...\n"); break;
            default: printf("Opcion invalida\n");
        }
        if (opcion != 7) {
            pausar();
        }
    } while (opcion != 7);

    system("pause");
    return 0;
}
