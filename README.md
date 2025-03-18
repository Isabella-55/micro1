CLASE 05
Escribir en lenguaje C un programa que: Pida por teclado la duración en minutos (dato entero) de una llamada telefónica. Calcule el coste de la llamada telefónica. Muestre por pantalla el resultado (dato real) en pesos. Nota: 1 Minuto = 45.48 pesos.

#include <stdio.h>

int main() {
    int minutos;          // Variable para almacenar la duración de la llamada en minutos
    float coste;          // Variable para almacenar el coste de la llamada en pesos

    // Solicitar al usuario la duración de la llamada en minutos
    printf("Introduce la duración de la llamada en minutos: ");
    scanf("%d", &minutos);

    // Calcular el coste de la llamada (1 minuto = 45.48 pesos)
    coste = minutos * 45.48;

    // Mostrar el coste de la llamada en pesos
    printf("El coste de la llamada es: %.2f pesos\n", coste);

    return 0;
}

Escribir en lenguaje C un programa que: Pida por teclado la nota (dato real) de una asignatura. En el caso de que la nota sea incorrecta, muestre por pantalla el mensaje: "ERROR: Nota incorrecta, debe ser >= 0,0 y <= 5,0". Repita los pasos 1º y 2º, mientras que, la nota introducida sea incorrecta. Muestre por pantalla: "APROBADO", en el caso de que la nota sea mayor o igual que 3,0. "SUSPENDIDO", en el caso de que la nota sea menor que 3,0. Nota: Utilizar un bucle while
#include <stdio.h>

int main() {
    float nota;  // Variable para almacenar la nota de la asignatura

    // Solicitar la nota al usuario y verificar que sea correcta
    do {
        printf("Introduce la nota de la asignatura (0.0 - 5.0): ");
        scanf("%f", &nota);

        // Verificar si la nota está dentro del rango permitido
        if (nota < 0.0 || nota > 5.0) {
            printf("ERROR: Nota incorrecta, debe ser >= 0.0 y <= 5.0\n");
        }
    } while (nota < 0.0 || nota > 5.0);  // Repetir mientras la nota sea incorrecta

    // Mostrar el resultado según la nota
    if (nota >= 3.0) {
        printf("APROBADO\n");
    } else {
        printf("SUSPENDIDO\n");
    }

    return 0;
}
CLASE 06
1.	Hacer un programa informático que realice las operaciones matemáticas de suma, resta, división y multiplicación entre dos números.
#include <stdio.h>

int main() {
    float num1, num2;  // Definimos las variables para los dos números
    int opcion;         // Variable para la opción de operación

    // Solicitar al usuario los dos números
    printf("Introduce el primer número: ");
    scanf("%f", &num1);

    printf("Introduce el segundo número: ");
    scanf("%f", &num2);

    // Mostrar el menú de operaciones
    printf("\nSeleccione la operación que desea realizar:\n");
    printf("1. Suma\n");
    printf("2. Resta\n");
    printf("3. Multiplicación\n");
    printf("4. División\n");
    printf("Ingrese su opción (1/2/3/4): ");
    scanf("%d", &opcion);

    // Realizar la operación correspondiente
    switch (opcion) {
        case 1:
            printf("\nEl resultado de %.2f + %.2f es: %.2f\n", num1, num2, num1 + num2);
            break;
        case 2:
            printf("\nEl resultado de %.2f - %.2f es: %.2f\n", num1, num2, num1 - num2);
            break;
        case 3:
            printf("\nEl resultado de %.2f * %.2f es: %.2f\n", num1, num2, num1 * num2);
            break;
        case 4:
            // Verificar si el segundo número no es 0 antes de realizar la división
            if (num2 != 0) {
                printf("\nEl resultado de %.2f / %.2f es: %.2f\n", num1, num2, num1 / num2);
            } else {
                printf("\nERROR: No se puede dividir por cero.\n");
            }
            break;
        default:
            printf("\nOpción no válida.\n");
    }

    return 0;
}

2.	Mostrar la suma de los 50 primeros números.
#include <stdio.h>

int main() {
    int suma = 0;

    // Calcular la suma de los 50 primeros números
    for (int i = 1; i <= 50; i++) {
        suma += i;  // Sumar el valor de i a la variable suma
    }

    // Mostrar el resultado
    printf("La suma de los primeros 50 números es: %d\n", suma);

    return 0;
}
3.	Encuentre la serie de Fibonacci hasta el término ≤1000
#include <stdio.h>

int main() {
    int a = 0, b = 1, c;

    printf("Serie de Fibonacci hasta el término <= 1000:\n");

    // Imprimir los primeros dos términos de la secuencia
    printf("%d ", a);
    printf("%d ", b);

    // Calcular y mostrar los siguientes términos
    c = a + b;
    while (c <= 1000) {
        printf("%d ", c);
        a = b;
        b = c;
        c = a + b;
    }

    printf("\n");

    return 0;
}
4.	Encuentra todas las raíces de una ecuación cuadrática ax2+bx+c=0
#include <stdio.h>
#include <math.h>  // Para usar la función sqrt()

int main() {
    float a, b, c, discriminante, raiz1, raiz2;

    // Solicitar al usuario los coeficientes de la ecuación
    printf("Introduce el coeficiente a: ");
    scanf("%f", &a);

    printf("Introduce el coeficiente b: ");
    scanf("%f", &b);

    printf("Introduce el coeficiente c: ");
    scanf("%f", &c);

    // Calcular el discriminante
    discriminante = b * b - 4 * a * c;

    // Evaluar según el valor del discriminante
    if (discriminante > 0) {
        // Dos raíces reales y distintas
        raiz1 = (-b + sqrt(discriminante)) / (2 * a);
        raiz2 = (-b - sqrt(discriminante)) / (2 * a);
        printf("Las raíces son: %.2f y %.2f\n", raiz1, raiz2);
    }
    else if (discriminante == 0) {
        // Una raíz real doble
        raiz1 = -b / (2 * a);
        printf("La raíz doble es: %.2f\n", raiz1);
    }
    else {
        // Raíces complejas
        float parte_real = -b / (2 * a);
        float parte_imaginaria = sqrt(-discriminante) / (2 * a);
        printf("Las raíces son complejas: %.2f + %.2fi y %.2f - %.2fi\n", parte_real, parte_imaginaria, parte_real, parte_imaginaria);
    }

    return 0;
}

5.	Encuentra el mayor entre tres números diferentes ingresados por el usuario.
#include <stdio.h>

int main() {
    float num1, num2, num3;

    // Solicitar al usuario los tres números
    printf("Introduce el primer número: ");
    scanf("%f", &num1);

    printf("Introduce el segundo número: ");
    scanf("%f", &num2);

    printf("Introduce el tercer número: ");
    scanf("%f", &num3);

    // Comparar los tres números para encontrar el mayor
    if (num1 > num2 && num1 > num3) {
        printf("El mayor número es: %.2f\n", num1);
    }
    else if (num2 > num1 && num2 > num3) {
        printf("El mayor número es: %.2f\n", num2);
    }
    else {
        printf("El mayor número es: %.2f\n", num3);
    }

    return 0;
}

6.	Escribir un programa que visualice un led verde cuando el valor de una variable sea mayor de 127, un led amarillo cuando el valor de variable este entre 64 y 127 y un led rojo cuando el valor de la variable sea menor a 64.

#include <stdio.h>

int main() {
    int valor;  // Variable que representa el valor de entrada

    // Solicitar al usuario que ingrese un valor
    printf("Introduce el valor de la variable: ");
    scanf("%d", &valor);

    // Comprobar el valor de la variable y encender el LED correspondiente
    if (valor > 127) {
        // LED verde
        printf("Encender LED verde\n");
    }
    else if (valor >= 64 && valor <= 127) {
        // LED amarillo
        printf("Encender LED amarillo\n");
    }
    else {
        // LED rojo
        printf("Encender LED rojo\n");
    }

    return 0;
}

CLASE 07
Se necesita diseñar una estructura de datos para el buzón de entrada de un teléfono móvil. Los mensajes en ese buzón son de dos tipos, o SMS, o MMS. Los mensajes SMS tiene un remitente (un número de teléfono), una fecha/hora (que es un número entero), un texto de no más de 140 letras y un tipo de mensaje que puede ser SMC o normal. Los mensajes MMS tienen los mismos campos excepto que en lugar del tipo de mensaje tienen una cadena de texto de hasta 200 letras con la ruta a la imagen que contiene. Escribir las definiciones de las estructuras de datos necesarias para tener una tabla de 100 mensajes como buzón de entrada.

#include <stdio.h>
#include <string.h>

#define MAX_MENSAJES 100
#define MAX_TEXTO_SMS 140
#define MAX_TEXTO_MMS 200
#define MAX_RUTA_IMAGEN 255

// Estructura para los mensajes SMS
struct SMS {
    char remitente[15];        // Número de teléfono (remitente)
    int fechaHora;             // Fecha/hora del mensaje
    char texto[MAX_TEXTO_SMS]; // Texto del mensaje
    char tipo[4];              // Tipo de mensaje: "SMC" o "normal"
};

// Estructura para los mensajes MMS
struct MMS {
    char remitente[15];        // Número de teléfono (remitente)
    int fechaHora;             // Fecha/hora del mensaje
    char texto[MAX_TEXTO_MMS]; // Texto del mensaje
    char rutaImagen[MAX_RUTA_IMAGEN]; // Ruta de la imagen contenida
};

// Unión para almacenar un mensaje SMS o MMS
union Mensaje {
    struct SMS sms; // Mensaje SMS
    struct MMS mms; // Mensaje MMS
};

// Estructura para el buzón de entrada que contiene hasta 100 mensajes
struct Buzon {
    union Mensaje mensajes[MAX_MENSAJES]; // Tabla de 100 mensajes
    int totalMensajes; // Número total de mensajes en el buzón
};

int main() {
    // Ejemplo de uso de la estructura

    // Crear un buzón de entrada
    struct Buzon buzon;
    buzon.totalMensajes = 0;

    // Crear un mensaje SMS
    struct SMS nuevoSMS = {
        "123456789",      // remitente
        202303141230,     // fecha/hora
        "Hola, ¿cómo estás?", // texto
        "normal"          // tipo
    };

    // Almacenar el mensaje en el buzón
    buzon.mensajes[buzon.totalMensajes].sms = nuevoSMS;
    buzon.totalMensajes++;

    // Crear un mensaje MMS
    struct MMS nuevoMMS = {
        "987654321",          // remitente
        202303141400,         // fecha/hora
        "Mira esta imagen",   // texto
        "/ruta/a/imagen.jpg"  // ruta de la imagen
    };

    // Almacenar el mensaje en el buzón
    buzon.mensajes[buzon.totalMensajes].mms = nuevoMMS;
    buzon.totalMensajes++;

    // Mostrar los mensajes almacenados
    for (int i = 0; i < buzon.totalMensajes; i++) {
        if (strlen(buzon.mensajes[i].sms.texto) > 0) {
            printf("Mensaje SMS de %s:\n", buzon.mensajes[i].sms.remitente);
            printf("Fecha/Hora: %d\n", buzon.mensajes[i].sms.fechaHora);
            printf("Texto: %s\n", buzon.mensajes[i].sms.texto);
            printf("Tipo: %s\n\n", buzon.mensajes[i].sms.tipo);
        } else {
            printf("Mensaje MMS de %s:\n", buzon.mensajes[i].mms.remitente);
            printf("Fecha/Hora: %d\n", buzon.mensajes[i].mms.fechaHora);
            printf("Texto: %s\n", buzon.mensajes[i].mms.texto);
            printf("Ruta Imagen: %s\n\n", buzon.mensajes[i].mms.rutaImagen);
        }
    }

    return 0;
}


#include <stdio.h>
#include <string.h>

#define MAX_MENSAJES 100
#define MAX_TEXTO_SMS 140
#define MAX_TEXTO_MMS 200
#define MAX_RUTA_IMAGEN 255

// Estructura para los mensajes SMS
struct SMS {
    char remitente[15];        // Número de teléfono (remitente)
    int fechaHora;             // Fecha/hora del mensaje
    char texto[MAX_TEXTO_SMS]; // Texto del mensaje
    char tipo[4];              // Tipo de mensaje: "SMC" o "normal"
};

// Estructura para los mensajes MMS
struct MMS {
    char remitente[15];        // Número de teléfono (remitente)
    int fechaHora;             // Fecha/hora del mensaje
    char texto[MAX_TEXTO_MMS]; // Texto del mensaje
    char rutaImagen[MAX_RUTA_IMAGEN]; // Ruta de la imagen contenida
};

// Unión para almacenar un mensaje SMS o MMS
union Mensaje {
    struct SMS sms; // Mensaje SMS
    struct MMS mms; // Mensaje MMS
};

// Estructura para el buzón de entrada que contiene hasta 100 mensajes
struct Buzon {
    union Mensaje mensajes[MAX_MENSAJES]; // Tabla de 100 mensajes
    int totalMensajes; // Número total de mensajes en el buzón
};

int main() {
    // Crear un buzón de entrada
    struct Buzon buzon;
    buzon.totalMensajes = 0;

    // Crear un mensaje SMS
    struct SMS nuevoSMS = {
        "123456789",      // remitente
        202303141230,     // fecha/hora
        "Hola, ¿cómo estás?", // texto
        "normal"          // tipo
    };

    // Almacenar el mensaje en el buzón
    buzon.mensajes[buzon.totalMensajes].sms = nuevoSMS;
    buzon.totalMensajes++;

    // Crear un mensaje MMS
    struct MMS nuevoMMS = {
        "987654321",          // remitente
        202303141400,         // fecha/hora
        "Mira esta imagen",   // texto
        "/ruta/a/imagen.jpg"  // ruta de la imagen
    };

    // Almacenar el mensaje en el buzón
    buzon.mensajes[buzon.totalMensajes].mms = nuevoMMS;
    buzon.totalMensajes++;

    // Mostrar los mensajes almacenados
    for (int i = 0; i < buzon.totalMensajes; i++) {
        if (buzon.mensajes[i].sms.texto[0] != '\0') {  // Verificamos si el SMS tiene texto
            printf("Mensaje SMS de %s:\n", buzon.mensajes[i].sms.remitente);
            printf("Fecha/Hora: %d\n", buzon.mensajes[i].sms.fechaHora);
            printf("Texto: %s\n", buzon.mensajes[i].sms.texto);
            printf("Tipo: %s\n\n", buzon.mensajes[i].sms.tipo);
        } else {
            printf("Mensaje MMS de %s:\n", buzon.mensajes[i].mms.remitente);
            printf("Fecha/Hora: %d\n", buzon.mensajes[i].mms.fechaHora);
            printf("Texto: %s\n", buzon.mensajes[i].mms.texto);
            printf("Ruta Imagen: %s\n\n", buzon.mensajes[i].mms.rutaImagen);
        }
    }

    return 0;
}
Construir un programa dotado de un menú de Archivo, otro de Edición y otro de Texto. El menú Archivo ofrecerá las opciones Nuevo, Abrir, Guardar, Cerrar y Salir. El menú Edición ofrecerá las opciones Deshacer, Cortar, Copiar y Pegar. El menú Texto ofrecerá las opciones Menor, Medio y Mayor. El programa debe mostrar una barra con los menús Archivo, Edición y Texto; cuando el usuario seleccione uno de ellos, aparecerán las opciones correspondientes a ese menú y el usuario podrá seleccionar una de ellas. Hecho esto, el programa mostrará el menú y la opción seleccionados, en la forma siguiente: Se ha seleccionado la opción Cortar del menú Edición.

#include <stdio.h>

void menuArchivo() {
    int opcion;
    printf("\nMenu Archivo:\n");
    printf("1. Nuevo\n");
    printf("2. Abrir\n");
    printf("3. Guardar\n");
    printf("4. Cerrar\n");
    printf("5. Salir\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            printf("Se ha seleccionado la opcion Nuevo del menu Archivo.\n");
            break;
        case 2:
            printf("Se ha seleccionado la opcion Abrir del menu Archivo.\n");
            break;
        case 3:
            printf("Se ha seleccionado la opcion Guardar del menu Archivo.\n");
            break;
        case 4:
            printf("Se ha seleccionado la opcion Cerrar del menu Archivo.\n");
            break;
        case 5:
            printf("Se ha seleccionado la opcion Salir del menu Archivo.\n");
            break;
        default:
            printf("Opcion invalida.\n");
            break;
    }
}

void menuEdicion() {
    int opcion;
    printf("\nMenu Edicion:\n");
    printf("1. Deshacer\n");
    printf("2. Cortar\n");
    printf("3. Copiar\n");
    printf("4. Pegar\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            printf("Se ha seleccionado la opcion Deshacer del menu Edicion.\n");
            break;
        case 2:
            printf("Se ha seleccionado la opcion Cortar del menu Edicion.\n");
            break;
        case 3:
            printf("Se ha seleccionado la opcion Copiar del menu Edicion.\n");
            break;
        case 4:
            printf("Se ha seleccionado la opcion Pegar del menu Edicion.\n");
            break;
        default:
            printf("Opcion invalida.\n");
            break;
    }
}

void menuTexto() {
    int opcion;
    printf("\nMenu Texto:\n");
    printf("1. Menor\n");
    printf("2. Medio\n");
    printf("3. Mayor\n");
    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            printf("Se ha seleccionado la opcion Menor del menu Texto.\n");
            break;
        case 2:
            printf("Se ha seleccionado la opcion Medio del menu Texto.\n");
            break;
        case 3:
            printf("Se ha seleccionado la opcion Mayor del menu Texto.\n");
            break;
        default:
            printf("Opcion invalida.\n");
            break;
    }
}

int main() {
    int menuSeleccionado, continuar = 1;

    while (continuar) {
        printf("\nMenu Principal:\n");
        printf("1. Archivo\n");
        printf("2. Edicion\n");
        printf("3. Texto\n");
        printf("4. Salir\n");
        printf("Seleccione un menu: ");
        scanf("%d", &menuSeleccionado);

        switch (menuSeleccionado) {
            case 1:
                menuArchivo();
                break;
            case 2:
                menuEdicion();
                break;
            case 3:
                menuTexto();
                break;
            case 4:
                printf("Saliendo del programa...\n");
                continuar = 0;
                break;
            default:
                printf("Opcion invalida. Por favor, seleccione una opcion valida.\n");
                break;
        }
    }

    return 0;
}









