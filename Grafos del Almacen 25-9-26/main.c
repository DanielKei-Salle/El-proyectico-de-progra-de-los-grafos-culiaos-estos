#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    Ingegrantes:    Daniel Kei Márquez Okamoto,  Layla Trujillo Echevería,  Luis Ángel Salinas García
    Grupo:  301CIB      Fehca de entrega:   26/10/2026
    Lenguaje: C
    El programa consiste a partir de una simulacion de una fabrica en la que un robot tendra que llevar distintos paquetes a las distintas zona de manera eficiente
    Esta simulacion se hara mediante el uso de grafos como representacion de las salas y pasillos conectados
*/


//-------------------Apuntadores para los pasillos entre las distintas salas
struct conctWay
{
    struct conctWay *nextWay;
    struct roomPos *conctRoom;
    int weight;
    short int blocked;
};

//---------------------Estructura para los apuntadores de las salas
struct roomPos
{
    struct roomPos *nextRoom, *backRoom;
    struct conctWay *startWay;
    char nameInfo[30];
    int ID;

};


//-----------------------------------------LLenar de manera rapida el mapa y conexiones
void fillMap(struct roomPos **startRoom)
{
    short int cl1,cl2;
    struct roomPos *moveRoom;

    *startRoom = (struct roomPos*) malloc(sizeof (struct roomPos));         //Crear el primer espacio para el primer apuntador
    moveRoom = *startRoom;
    for(cl1=0;cl1<8;cl1++)          //Ciclo que repite 8 veces para llenar todos los datos
    {
        moveRoom->ID  = cl1;
        switch(cl1)         //Escoje los datos a llenar de cada uno de los apuntadoes
        {
        case 0:     strcpy(moveRoom->nameInfo, "Estanterias");     break;
        case 1:     strcpy(moveRoom->nameInfo, "Zona de carga");    break;
        case 2:     strcpy(moveRoom->nameInfo, "Control de calidad");   break;
        case 3:     strcpy(moveRoom->nameInfo, "Zona de descarga");    break;
        case 4:     strcpy(moveRoom->nameInfo, "Area de empaquetado");    break;
        case 5:     strcpy(moveRoom->nameInfo, "Almacen frio");    break;
        case 6:     strcpy(moveRoom->nameInfo, "Mantenimiento");    break;
        case 7:     strcpy(moveRoom->nameInfo, "Puertas de salida");    break;
        }
        if(cl1!=7)          //Crea un nuevo espacio vacio al siguiente apuntador si aun queda espacio del ciclo
        {
            moveRoom->nextRoom = (struct roomPos*) malloc(sizeof (struct roomPos));
            moveRoom->nextRoom->backRoom = moveRoom;
            moveRoom = moveRoom->nextRoom;
        }else   moveRoom->nextRoom = NULL;
    }

    struct conctWay *moveWay;
    moveRoom = *startRoom;
    for(cl1=0;cl1<8;cl1++)
    {
        moveRoom->startWay = (struct conctWay*) malloc(sizeof (struct conctWay));
        moveWay = moveRoom->startWay;
        switch(cl1)
        {
            //----Coinfiguracion de Estanteria
        case 0:
            moveWay->weight = 3;
            moveWay->conctRoom = moveRoom->nextRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 1;
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom;
            moveWay->nextWay = NULL;
            break;

            //----Coinfiguracion de Zona de carga
        case 1:
            moveWay->weight = 4;
            moveWay->conctRoom = moveRoom->nextRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 5;
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 2;
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom->nextRoom;
            moveWay->nextWay = NULL;
            break;

            //----Coinfiguracion de cONTROL de calidad
        case 2:
            moveWay->weight = 25;
            moveWay->conctRoom = moveRoom->nextRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 17;
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom->nextRoom;
            moveWay->nextWay = NULL;
            break;

            //----Configuracion de Zona de descarga
        case 3:
            moveWay->weight = 5;
            moveWay->conctRoom = moveRoom->backRoom->backRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 7;
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom->nextRoom;
            moveWay->nextWay = NULL;
            break;

            //----Configuracion de Area de Empaquetado
        case 4:
            moveWay->weight = 6;
            moveWay->conctRoom = moveRoom->nextRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 17;
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom->nextRoom;
            moveWay->nextWay = NULL;
            break;

        case 5:
            moveWay->weight = 7;
            moveWay->conctRoom = moveRoom->backRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 15;
            moveWay->conctRoom = moveRoom->nextRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 8;
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom;
            moveWay->nextWay = NULL;
            break;

        case 6:
            moveWay->weight = 8;
            moveWay->conctRoom = moveRoom->backRoom->backRoom->backRoom;
            moveWay->nextWay = NULL;
            break;

        case 7:
            moveRoom->startWay = NULL;
            break;
        }
        moveRoom = moveRoom->nextRoom;
    }


    moveRoom = *startRoom;
    while(moveRoom!=NULL)           //Los imprime para comprobar que si funciona
    {
        moveWay = moveRoom->startWay;
        printf("[%s][%d]   ",moveRoom->nameInfo, moveRoom->ID);
        while(moveWay!=NULL)
        {
            moveWay->blocked = 0;
            printf("(%s)(%d)[%hd]  ",moveWay->conctRoom->nameInfo, moveWay->weight, moveWay->blocked);
            moveWay = moveWay->nextWay;
            //scanf("%hd",&cl1);
        }
        printf("\n");
        moveRoom = moveRoom->nextRoom;
    }

}


//----------------------Determinar sala con menor numero de conexiones
void checkWays(struct roomPos *startRoom)
{
    struct roomPos *moveRoom = startRoom, *extRoom = startRoom;
    struct conctWay *moveWay;
    int maxWay = 0,countWay;

    while(moveRoom!=NULL)
    {
        countWay = 0;
        moveWay = moveRoom->startWay;
        while(moveWay!=NULL)
        {
            countWay++;
            moveWay = moveWay->nextWay;
        }
        if(countWay>=maxWay)
        {
            extRoom = moveRoom;
            maxWay = countWay;
        }
        moveRoom = moveRoom->nextRoom;
    }
    printf("Sala con mas conexiones [%s]",extRoom->nameInfo);
}


/*int blockRoom(struct roomPos **startRoom)
{
    int idSearch;
    struct roomPos *moveRoom = *startRoom;

    printf("Ingrese la ID de la sala a buscar:  ");
    scanf("%d",&idSearch);
    while(moveRoom!=NULL)           //Ciclo que recorre la lista de salas
    {
        if(idSearch == moveRoom->ID)        //Detecta si la ID ingresada coincide con la de la sala
        {
            moveRoom->blocked += 1;         //Suma 1 el estado de la sala para cambiar su estado
            if(moveRoom->blocked==1)    printf("Se ha bloqueado la sala [%s]",moveRoom->nameInfo);
            else            //Si el estado es mayor a 1,  lo devuelve a 0 osea que la desbloquea
            {
                moveRoom->blocked = 0;
                printf("Se ha desbloqueado la sala [%s]",moveRoom->nameInfo);
            }
            return 0;
        }
        moveRoom = moveRoom->nextRoom;
    }
    printf("No se encontro una lista con esa ID");          //Si termina el ciclo,  significa que no encontro una sala con la misma ID
    return 0;
}*/


struct roomPos *searchRoom(struct roomPos *startRoom, int idSearch)
{
    struct roomPos *moveRoom = startRoom;

    while(moveRoom!=NULL)           //Ciclo que recorre la lista de salas
    {
        if(idSearch == moveRoom->ID)    return moveRoom;        //Detecta si la ID ingresada coincide con la de la sala
        moveRoom = moveRoom->nextRoom;
    }
    return NULL;            //Si termina el ciclo,  significa que no encontro una sala con la misma ID
}


void addConct(struct roomPos **startRoom)
{
    struct roomPos *extRoom;
    int idSearch;

    printf("Ingrese la ID de la sala a agregar conexiones:  ");      scanf("%d",&idSearch);
    extRoom = searchRoom(*startRoom, idSearch);
    if(extRoom!=NULL)
    {

    }else   printf("No existe una sala con esa ID");
}


void makeRoom(struct roomPos **startRoom)
{
    short int addLoop;
    int idSearch;
    struct roomPos *moveRoom = *startRoom, *newRoom, *extRoom;
    struct conctWay *moveWay;
    newRoom = (struct roomPos*) malloc(sizeof(struct roomPos));

    newRoom->nextRoom = NULL;
    newRoom->startWay = NULL;
    printf("Ingrese el Nombre de la sala a agregar:  ");
    scanf(" ");     gets(newRoom->nameInfo);

    /*printf("Desea agrgar conexiones a esta sala?  Si(1)  No(0)  ");   scanf("%hd",&addLoop);
    while(addLoop==1)
    {
        moveWay = (struct conctWay*) malloc(sizeof(struct conctWay));
        do
        {
            printf("Ingrese la ID de la sala a conectar:  ");      scanf("%d",&idSearch);
            extRoom = searchRoom(*startRoom, idSearch);
            if(extRoom==NULL)   printf("Error,  no se encontro una sala con esa ID\n");
        }while(extRoom==NULL);

        printf("Sala Conectada [%s]\n",extRoom->nameInfo);
        moveWay->conctRoom = extRoom;
        printf("Ingrese el peso de la conexion con esa sala:  ");    scanf("%d",&moveWay->weight);
        moveWay = moveWay->nextWay;
        printf("Desea agregar mas conexiones a esta sala?  Si(1)  No(0) ");   scanf("%hd",&addLoop);
    }
    moveWay = NULL;*/

    while(moveRoom->nextRoom!=NULL)     moveRoom = moveRoom->nextRoom;
    newRoom->backRoom = moveRoom;
    newRoom->ID = moveRoom->ID + 1;
    moveRoom->nextRoom = newRoom;


    moveRoom = *startRoom;
    while(moveRoom!=NULL)           //Los imprime para comprobar que si funciona
    {
        moveWay = moveRoom->startWay;
        printf("[%s][%d]   ",moveRoom->nameInfo, moveRoom->ID);
        while(moveWay!=NULL)
        {
            moveWay->blocked = 0;
            printf("(%s)(%d)[%hd]  ",moveWay->conctRoom->nameInfo, moveWay->weight, moveWay->blocked);
            moveWay = moveWay->nextWay;
            //scanf("%hd",&cl1);
        }
        printf("\n");
        moveRoom = moveRoom->nextRoom;
    }
}



//----------------El mismisimo main
int main()
{
    struct roomPos *startRoom, *moveRoom, *extRoom;
    struct conctWay *moveWay;
    fillMap(&startRoom);

    short int menu;


    do
    {
        printf("\n\n)Realizar simulacion de viaje\n2)Bloquear ubicacion\n3)Identificar nodo de mayor conexiones\n4)Añadir nueva sala)");
        scanf("%hd",&menu);
        switch(menu)
        {

        case 2:
            //blockRoom(&startRoom);  break;
        case 3:
            checkWays(startRoom);   break;
        case 4:
            makeRoom(&startRoom);   break;
        }
    }while(menu!=-1);

    return 0;
}
