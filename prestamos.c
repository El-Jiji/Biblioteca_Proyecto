#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Prestamo {
    int numeroPrestamo;
    int idUsuario;
    int idLibro;
    char fechaPrestamo[20];
    int diasSolicitados;
    char fechaDevolucion[20];
    int devuelto;
    char fechaDevolucionReal[20];
};

// Obtiene la fecha actual
void obtenerFechaActual(char *buffer) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d",
            tm_info->tm_mday,
            tm_info->tm_mon + 1,
            tm_info->tm_year + 1900);
}

// Pide al usuario que introduzca una fecha (como texto)
void obtenerFechaEntrega(char *fechaTexto) {
    printf("Introduce la fecha de devolucion (DD/MM/AAAA): ");
    fgets(fechaTexto, 20, stdin);
    fechaTexto[strcspn(fechaTexto, "\n")] = '\0';
}

// Calcula una fecha futura sumando dias
void calcularFechaDevolucion(int dias, char *buffer) {
    time_t t = time(NULL);
    t += dias * 24 * 60 * 60;
    struct tm *tm_info = localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d",
            tm_info->tm_mday,
            tm_info->tm_mon + 1,
            tm_info->tm_year + 1900);
}

// Genera el siguiente numero de prestamo
int obtenerSiguienteNumeroPrestamo() {
    FILE *archivo = fopen("prestamos.txt", "r");
    int maxNumero = 0;
    char linea[256];

    if (archivo != NULL) {
        while (fgets(linea, sizeof(linea), archivo)) {
            int num;
            sscanf(linea, "%d;", &num);
            if (num > maxNumero)
                maxNumero = num;
        }
        fclose(archivo);
    }

    return maxNumero + 1;
}

// Verifica si el libro existe y está disponible
int verificarLibroDisponible(int idLibro, char *tituloLibro) {
    FILE *archivo = fopen("libros.txt", "r");
    char linea[256];
    int encontrado = 0;

    if (archivo == NULL) {
        return 0;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        int id, disponible;
        char titulo[100], autor[100];

        sscanf(linea, "%d;%[^;];%[^;];%d", &id, titulo, autor, &disponible);

        if (id == idLibro) {
            strcpy(tituloLibro, titulo);
            encontrado = disponible;
            break;
        }
    }

    fclose(archivo);
    return encontrado;
}

// Verifica si el usuario existe
int verificarUsuarioExiste(int idUsuario, char *nombreUsuario) {
    FILE *archivo = fopen("usuarios.txt", "r");
    char linea[256];
    int encontrado = 0;

    if (archivo == NULL) {
        return 0;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        int id;
        char nombre[100], tipo[20];
        sscanf(linea, "%d;%[^;];%[^\n]", &id, nombre, tipo);

        if (id == idUsuario) {
            strcpy(nombreUsuario, nombre);
            encontrado = 1;
            break;
        }
    }

    fclose(archivo);
    return encontrado;
}

// Cambia disponibilidad del libro
void actualizarDisponibilidadLibro(int idLibro, int nuevoEstado) {
    FILE *archivo = fopen("libros.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char linea[256];

    if (archivo == NULL || temp == NULL) {
        printf("Error al actualizar disponibilidad.\n");
        return;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        int id, disponible;
        char titulo[100], autor[100];
        sscanf(linea, "%d;%[^;];%[^;];%d", &id, titulo, autor, &disponible);

        if (id == idLibro)
            fprintf(temp, "%d;%s;%s;%d\n", id, titulo, autor, nuevoEstado);
        else
            fprintf(temp, "%s", linea);
    }

    fclose(archivo);
    fclose(temp);
    remove("libros.txt");
    rename("temp.txt", "libros.txt");
}

// Registrar un prestamo
void realizarPrestamo() {
    printf("|======= Realizar prestamo de libro =======|\n\n");

    struct Prestamo nuevoPrestamo;
    char tituloLibro[100], nombreUsuario[100], domicilio[200];

    printf("Ingrese el ID del usuario: ");
    scanf("%d", &nuevoPrestamo.idUsuario);
    getchar();

    if (!verificarUsuarioExiste(nuevoPrestamo.idUsuario, nombreUsuario)) {
        printf("\n[ERROR] Usuario no encontrado.\n");
        return;
    }

    printf("Usuario: %s\n", nombreUsuario);
    printf("Ingrese domicilio: ");
    scanf(" %[^\n]", domicilio);

    printf("\nIngrese el ID del libro: ");
    scanf("%d", &nuevoPrestamo.idLibro);
    getchar();

    if (!verificarLibroDisponible(nuevoPrestamo.idLibro, tituloLibro)) {
        printf("\n[ERROR] Libro no disponible.\n");
        return;
    }

    printf("Libro encontrado: %s\n", tituloLibro);
    printf("Ingrese dias solicitados: ");
    scanf("%d", &nuevoPrestamo.diasSolicitados);
    getchar();

    nuevoPrestamo.numeroPrestamo = obtenerSiguienteNumeroPrestamo();
    obtenerFechaActual(nuevoPrestamo.fechaPrestamo);
    calcularFechaDevolucion(nuevoPrestamo.diasSolicitados, nuevoPrestamo.fechaDevolucion);
    nuevoPrestamo.devuelto = 0;
    strcpy(nuevoPrestamo.fechaDevolucionReal, "N/A");

    FILE *archivo = fopen("prestamos.txt", "a");
    if (archivo == NULL) {
        printf("[ERROR] No se pudo guardar el prestamo.\n");
        return;
    }

    fprintf(archivo, "%d;%d;%d;%s;%d;%s;%d;%s\n",
            nuevoPrestamo.numeroPrestamo,
            nuevoPrestamo.idUsuario,
            nuevoPrestamo.idLibro,
            nuevoPrestamo.fechaPrestamo,
            nuevoPrestamo.diasSolicitados,
            nuevoPrestamo.fechaDevolucion,
            nuevoPrestamo.devuelto,
            nuevoPrestamo.fechaDevolucionReal);

    fclose(archivo);
    actualizarDisponibilidadLibro(nuevoPrestamo.idLibro, 0);

    printf("\n========== RECIBO DE PRESTAMO ==========\n");
    printf(" Numero de Prestamo: %d\n", nuevoPrestamo.numeroPrestamo);
    printf(" Usuario: %s (ID %d)\n", nombreUsuario, nuevoPrestamo.idUsuario);
    printf(" Libro: %s (ID %d)\n", tituloLibro, nuevoPrestamo.idLibro);
    printf(" Fecha de Prestamo: %s\n", nuevoPrestamo.fechaPrestamo);
    printf(" Fecha Limite: %s\n", nuevoPrestamo.fechaDevolucion);
    printf("========================================\n\n");
}

// Devolver un libro
void devolverLibro() {
    printf("|======= Devolucion de libro =======|\n\n");

    int numeroPrestamo, encontrado = 0;
    struct Prestamo prestamo;
    char linea[256];

    printf("Ingrese el numero de prestamo: ");
    scanf("%d", &numeroPrestamo);
    getchar();

    FILE *archivo = fopen("prestamos.txt", "r");
    if (archivo == NULL) {
        printf("[ERROR] No hay prestamos registrados.\n");
        return;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        int num;
        sscanf(linea, "%d;", &num);

        if (num == numeroPrestamo) {
            sscanf(linea, "%d;%d;%d;%[^;];%d;%[^;];%d;%[^;\n]",
                   &prestamo.numeroPrestamo,
                   &prestamo.idUsuario,
                   &prestamo.idLibro,
                   prestamo.fechaPrestamo,
                   &prestamo.diasSolicitados,
                   prestamo.fechaDevolucion,
                   &prestamo.devuelto,
                   prestamo.fechaDevolucionReal);
            encontrado = 1;
            break;
        }
    }
    fclose(archivo);

    if (!encontrado) {
        printf("\n[ERROR] No se encontro el prestamo.\n");
        return;
    }

    if (prestamo.devuelto == 1) {
        printf("\n[AVISO] Este prestamo ya fue devuelto.\n");
        return;
    }

    char miFecha[20];
    obtenerFechaEntrega(miFecha);
    strcpy(prestamo.fechaDevolucionReal, miFecha);

    char hayDemora;
    int diasDemora = 0;
    float cuotaDemora = 0.0;

    printf("\n¿El libro se devuelve con demora? (s/n): ");
    scanf(" %c", &hayDemora);
    getchar();

    if (hayDemora == 's' || hayDemora == 'S') {
        printf("Ingrese los dias de demora: ");
        scanf("%d", &diasDemora);
        getchar();
        cuotaDemora = diasDemora * 10.0;
    }

    archivo = fopen("prestamos.txt", "r");
    FILE *temp = fopen("temp_prestamos.txt", "w");

    while (fgets(linea, sizeof(linea), archivo)) {
        int num;
        sscanf(linea, "%d;", &num);

        if (num == numeroPrestamo) {
            fprintf(temp, "%d;%d;%d;%s;%d;%s;%d;%s\n",
                    prestamo.numeroPrestamo,
                    prestamo.idUsuario,
                    prestamo.idLibro,
                    prestamo.fechaPrestamo,
                    prestamo.diasSolicitados,
                    prestamo.fechaDevolucion,
                    1,
                    prestamo.fechaDevolucionReal);
        } else {
            fprintf(temp, "%s", linea);
        }
    }

    fclose(archivo);
    fclose(temp);
    remove("prestamos.txt");
    rename("temp_prestamos.txt", "prestamos.txt");

    actualizarDisponibilidadLibro(prestamo.idLibro, 1);

    printf("\n========== COMPROBANTE DE DEVOLUCION ==========\n");
    printf(" Numero de Prestamo: %d\n", prestamo.numeroPrestamo);
    printf(" ID del Libro: %d\n", prestamo.idLibro);
    printf(" Fecha de Prestamo: %s\n", prestamo.fechaPrestamo);
    printf(" Fecha Limite: %s\n", prestamo.fechaDevolucion);
    printf(" Fecha de Devolucion Real: %s\n", prestamo.fechaDevolucionReal);
    printf("----------------------------------------------\n");
    if (cuotaDemora > 0)
        printf(" Demora: %d dia(s)\n Cuota: $%.2f\n TOTAL: $%.2f\n", diasDemora, cuotaDemora, cuotaDemora);
    else
        printf(" Devolucion a tiempo. Sin cargos.\n TOTAL: $0.00\n");
    printf("==============================================\n\n");
}
