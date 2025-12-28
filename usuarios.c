#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Usuario {
    int id;
    char nombre[100];
    char tipo[20]; // "Lector" o "Empleado"
};

void registrarUsuario() {
    struct Usuario nuevoUsuario;
    FILE *archivo;
    int opcionTipo;

    printf("\n|======= Registro de usuario =======|\n");
    
    printf("Ingrese ID de usuario: ");
    scanf("%d", &nuevoUsuario.id);
    getchar(); // Limpiar buffer

    printf("Ingrese Nombre: ");
    scanf(" %[^\n]", nuevoUsuario.nombre);

    printf("Seleccione Tipo de Usuario:\n");
    printf("1. Lector\n");
    printf("2. Empleado\n");
    printf("Opcion: ");
    scanf("%d", &opcionTipo);

    if (opcionTipo == 1) {
        strcpy(nuevoUsuario.tipo, "Lector");
    } else if (opcionTipo == 2) {
        strcpy(nuevoUsuario.tipo, "Empleado");
    } else {
        strcpy(nuevoUsuario.tipo, "Desconocido");
    }

    archivo = fopen("usuarios.txt", "a");
    if (archivo == NULL) {
        printf("Error al abrir el archivo de usuarios.\n");
        return;
    }

    // Guardar en formato: ID;Nombre;Tipo
    fprintf(archivo, "%d;%s;%s\n", nuevoUsuario.id, nuevoUsuario.nombre, nuevoUsuario.tipo);
    fclose(archivo);

    printf("Usuario registrado exitosamente como %s.\n", nuevoUsuario.tipo);
}

void mostrarUsuarios() {
    struct Usuario usuario;
    FILE *archivo;
    char linea[256];

    printf("\n|======= Lista de Usuarios =======|\n");

    archivo = fopen("usuarios.txt", "r");
    if (archivo == NULL) {
        printf("No hay usuarios registrados.\n");
        return;
    }

    printf("%-5s %-30s %-15s\n", "ID", "Nombre", "Tipo");
    printf("----------------------------------------------------\n");

    while (fgets(linea, sizeof(linea), archivo)) {
        char *token;
        
        token = strtok(linea, ";");
        if(token) usuario.id = atoi(token);
        
        token = strtok(NULL, ";");
        if(token) strcpy(usuario.nombre, token);
        
        token = strtok(NULL, ";");
        if(token) {
            // Eliminar posible salto de linea al final
            token[strcspn(token, "\n")] = 0;
            strcpy(usuario.tipo, token);
        }

        printf("%-5d %-30s %-15s\n", usuario.id, usuario.nombre, usuario.tipo);
    }

    fclose(archivo);
}
