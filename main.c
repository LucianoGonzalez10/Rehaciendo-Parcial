#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    int id;
    char nombre[20];
    char apellido[20];
    char dni[10];
    int edad;
    char genero;
} stPersona;

typedef struct nodo
{
    stPersona dato;
    struct nodo * siguiente;
    struct nodo * anterior;
} nodo;

typedef struct
{
    int * arreglo;
    int posTope;
} Pila;

nodo * iniclista();
nodo * crearNodo(stPersona dato);
nodo * agregarOrdenado(nodo * lista, nodo * nuevo);
nodo * cargarLista(char nombreAr[], nodo * lista);
nodo * agregarPrincipio(nodo * lista, nodo * nuevo);
nodo * agregarFinal(nodo * lista, nodo * nuevo);
nodo * buscarUltimo(nodo * lista);
nodo * pasarJoves(nodo * lista, nodo ** menores);
void inicpila(Pila * p);
void apilar(Pila * p, int valor);

int main()
{
    stPersona dato;
    char nombreAr[] = "archivoPersonas.bin";
    nodo * lista = iniclista();
    lista = cargarLista(nombreAr, lista);
    // mostrarReves(lista);

    nodo * menores = iniclista();
    lista = pasarJoves(lista, &menores);
    mostrarNodo(menores, dato);

    Pila p;
    inicpila(&p);
    apilarMenores(menores, &p);
    mostrarPila(p);
    return 0;
}

nodo * iniclista()
{
    return NULL;
}

nodo * crearNodo(stPersona dato)
{
    nodo * nuevo = (nodo*) malloc(sizeof(nodo));
    nuevo->dato = dato;
    nuevo->siguiente = NULL;
    nuevo->anterior = NULL;

    return nuevo;
}

nodo * agregarOrdenado(nodo * lista, nodo * nuevo)
{
    if(lista == NULL) /// Primero verifico si la lista esta vacia.
    {
        lista = nuevo; /// Si esta vacia, hago que lista sea igual al nuevo dato.
    }
    else /// Si la lista NO esta vacia.
    {
        if(nuevo->dato.edad < lista->dato.edad) /// Si el nuevo dato es menor al primer dato de la lista, agrego al principio.
        {
            lista = agregarPrincipio(lista, nuevo); /// Igualo la lista a la funcion de agregar al principio.
        }
        else
        {
            nodo * seg = lista->siguiente; /// Creo una lista seguidora y  la igualo a lista siguiente.
            nodo * ante = lista; /// Creo una lista ante y la igualo a la lista.
            while(seg != NULL && nuevo->dato.edad > seg->dato.edad) /// Mientras seguidora tenga algo y el nuevo dato sea MAYOR al de la seguidora.
            {
                ante = seg; /// Igualo ante a seg.
                seg = seg->siguiente; /// Y avanzo seguidora
            }
            ante->siguiente = nuevo; /// Luego vinculo los nodos.
            nuevo->anterior = ante; /// vinculo
            nuevo->siguiente = seg; /// vinculo
            if(seg != NULL) /// Si seguidora es distinto de NULL.
            {
                seg->anterior = nuevo; /// Seguidora anterior igual a nuevo.
            }
        }
    }
    return lista;/// Retorno la lista
}
nodo * cargarLista(char nombreAr[], nodo * lista)
{
    FILE * buffer = fopen(nombreAr, "rb");
    stPersona aux;
    if(buffer)
    {
        while(fread(&aux, sizeof(stPersona),1, buffer) > 0)
        {
            nodo * nuevo = crearNodo(aux);
            lista = agregarOrdenado(lista, nuevo);
        }
    }
    fclose(buffer);
    return lista;
}

void mostrarNodo(nodo * lista, stPersona dato)
{
    while(lista != NULL)
    {
        printf("\nID...................................: %d", lista->dato.id);
        printf("\nNombre y Apellido..........: %s %s", lista->dato.nombre, lista->dato.apellido);
        printf("\nDNI.................................: %s",lista->dato.dni);
        printf("\nEdad...............................: %d",lista->dato.edad);
        printf("\nGenero............................: %s \n",(lista->dato.genero=='f')?"Femenino":"Masculino");
        lista = lista->siguiente;
    }
}

nodo * agregarPrincipio(nodo * lista, nodo * nuevo)
{
    if(lista == NULL)
    {
        lista = nuevo;
    }
    else
    {
        lista->anterior = nuevo;
        nuevo->siguiente = lista;
    }
    lista = nuevo;
    return lista;
}

nodo * agregarFinal(nodo * lista, nodo * nuevo)
{
    if(lista == NULL)
    {
        lista = nuevo;
    }
    else
    {
        nodo * ultimo = buscarUltimo(lista);
        ultimo->siguiente = nuevo;
        nuevo->anterior = ultimo;
    }
    return lista;
}

nodo * buscarUltimo(nodo * lista)
{
    nodo * seg = lista;
    if(seg->siguiente != NULL)
    {
        seg = buscarUltimo(seg->siguiente);
    }
    return seg;
}

void mostrarReves (nodo * lista)
{
    nodo * ultimo = buscarUltimo(lista);
    while(ultimo != NULL)
    {
        printf("\nID...................................: %d", ultimo->dato.id);
        printf("\nNombre y Apellido..........: %s %s", ultimo->dato.nombre, ultimo->dato.apellido);
        printf("\nDNI.................................: %s",ultimo->dato.dni);
        printf("\nEdad...............................: %d",ultimo->dato.edad);
        printf("\nGenero............................: %s \n",(ultimo->dato.genero=='f')?"Femenino":"Masculino");
        ultimo = ultimo->anterior;
    }
}

nodo * pasarJoves(nodo * lista, nodo ** menores)
{
    nodo * pasar;
    if(lista != NULL && lista->dato.edad < 30)
    {
        pasar = lista;
        lista = lista->siguiente;
        pasar->siguiente = NULL;
        lista->anterior = NULL;
        *menores = agregarFinal(*menores, pasar);
    }

    nodo * seg = lista->siguiente;
    while(seg != NULL)
    {
        nodo * ante = seg;
        if(seg->dato.edad < 30)
        {
            pasar = seg;
            seg = seg->siguiente;
            seg->anterior = NULL;
            pasar->siguiente = NULL;
            pasar->anterior = NULL;
            *menores = agregarFinal(*menores, pasar);
        }
        else
        {
            seg = seg->siguiente;
        }
    }
    return lista;
}

void inicpila(Pila * p)
{
    p->arreglo =(int*) malloc(sizeof(int)*10);
    p->posTope = 0;
}

void apilar(Pila * p, int valor)
{
    if(p->posTope < 10)
    {
        p->arreglo[p->posTope] = valor;
        (p->posTope)++;
    }
    else
    {
        printf("La pila esta llena. \n");
    }
}

void apilarMenores(nodo * menores, Pila * p)
{
    int i = 0;
    while(menores != NULL &&  i<10)
    {
        apilar(p, (menores->dato.id));
        i++;
        menores = menores->siguiente;
    }
}

void mostrarPila(Pila p)
{
    int i = 0;
    printf("\nBASE....................................TOPE \n");
    while(i < p.posTope)
    {
        printf("%d   ", p.arreglo[i]);
        i++;
    }
    printf("\nBASE....................................TOPE \n");
}
