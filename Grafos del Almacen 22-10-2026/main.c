#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    LO QUE PRINCIPALMENTE FALTA DE ESTE TRABAJO SON:
    - Poder agregar caminos a la sala que desee el usuario
    - Poder permitir la opcion de cargar de un archivo el nuestro o uno nuevo,  ya sea por los propios metodos del programa o por archivo txt
    - Que la opcion de bloquear camino desbloque el camino si es que se activa a un camino que ya estaba bloqueado
    - Cambiar los valores de peso por algunos mas extremos
    - xdddddddd
    - Talvez (no se como lo quioera Gaby),  poner las funciones como una libreria propia en vez de poner todo en el main
*/

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
            moveWay->weight = 10;
            moveWay->conctRoom = moveRoom->nextRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 1;
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
            moveWay->weight = 3;
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom->nextRoom;
            moveWay->nextWay = NULL;
            break;

            //----Coinfiguracion de Zona de carga
        case 1:
            moveWay->weight = 9;
            moveWay->conctRoom = moveRoom->backRoom;
            moveWay->nextWay = (struct conctWay*) malloc(sizeof (struct conctWay));
            moveWay = moveWay->nextWay;
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
            moveWay->conctRoom = moveRoom->nextRoom->nextRoom;
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
            moveWay->weight = 1;
            moveWay->conctRoom = moveRoom->backRoom->backRoom->backRoom;
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


int blockRoom(struct roomPos **startRoom)
{
    /*
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
    */
}


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
    newRoom = (struct roomPos*) malloc(sizeof(struct roomPos));             //Crear una nueva sala vacia

    newRoom->nextRoom = NULL;           //Poner las conexiones de la nueva sala como vacios
    newRoom->startWay = NULL;
    printf("Ingrese el Nombre de la sala a agregar:  ");            //Ingresar el nombre de la sala
    scanf(" ");     gets(newRoom->nameInfo);

    printf("Desea agrgar conexiones a esta sala?  Si(1)  No(0)  ");   scanf("%hd",&addLoop);
    while(addLoop==1)               //Ciclo que repite cada vez que se pretende agregar conexiones
    {
        moveWay = (struct conctWay*) malloc(sizeof(struct conctWay));           //Crear un nuevo camino vacio
        do
        {
            printf("Ingrese la ID de la sala a conectar:  ");      scanf("%d",&idSearch);           //Buscar la sala con la funcion y repetir si no encuenta
            extRoom = searchRoom(*startRoom, idSearch);
            if(extRoom==NULL)   printf("Error,  no se encontro una sala con esa ID\n");
        }while(extRoom==NULL);

        printf("Sala Conectada [%s]\n",extRoom->nameInfo);
        moveWay->conctRoom = extRoom;               //Colocar la nueva conexion a la lista de conexiones de la nueva sala
        printf("Ingrese el peso de la conexion con esa sala:  ");    scanf("%d",&moveWay->weight);
        moveWay = moveWay->nextWay;
        printf("Desea agregar mas conexiones a esta sala?  Si(1)  No(0) ");   scanf("%hd",&addLoop);
    }
    moveWay = NULL;

    while(moveRoom->nextRoom!=NULL)     moveRoom = moveRoom->nextRoom;          //recorrer todas las salas
    newRoom->backRoom = moveRoom;           //Colcoar los apuntadores correctos a la nueva sala
    newRoom->ID = moveRoom->ID + 1;         //Añadirle la ID a la nueva sala considerando la sala mas alta + 1
    moveRoom->nextRoom = newRoom;
    printf("Se añadio la sala %s\n",newRoom->nameInfo);
}


void printRoom(struct roomPos *startRoom)
{
    struct roomPos *moveRoom = startRoom;           //Crear las variables de apuntadores movibles
    struct conctWay *moveWay;

    while(moveRoom!=NULL)           //Ciclo que recorre la lista de salas
    {
        printf("\n--------\nSala: %s   ID: %d\nConexiones:  ",moveRoom->nameInfo, moveRoom->ID);
        moveWay = moveRoom->startWay;           //Coloca el apuntador de camino en su lista de conexiones
        while(moveWay!=NULL)
        {
            printf("[Sala: %s  Peso: %d  ",moveWay->conctRoom->nameInfo, moveWay->weight);
            if(moveWay->blocked==0)     printf("(Libre)] | ");            //Imprime ya se alibre o bloqueado dependiendo de lo que contenga su variable
            else    printf("(Bloqueado)] | ");
            moveWay = moveWay->nextWay;
        }
        moveRoom = moveRoom->nextRoom;
    }
}



//-------------Checar por cantidad de saltos
int checkJump(struct roomPos *finalRoad, struct roomPos *pastRoom)
{
    struct roomPos *finalRoom = finalRoad,  *moveRoom = pastRoom;           //Inicializar los apuntadores de movimiento
    int cl1 = 0, cl2 = 0;

    while(moveRoom!=NULL)           //Checar la cantidad de elementos hay  en la lista de pasados actual
    {
        cl1++;
        moveRoom = moveRoom->nextRoom;
    }
    while(finalRoom!=NULL)          //Checar la cantidad de elementos hay en la lista de posible ruta actual
    {
        cl2++;
        finalRoom = finalRoom->nextRoom;
    }
    if(cl1<cl2) return 1;
    else    return 0;
}


//-------------Bloquear alguna ruta  de camino
void blockWay(struct roomPos **startRoom)
{
    struct roomPos *moveRoom = *startRoom;           //Crear las variables de apuntadores movibles
    struct conctWay *moveWay;
    int idSearch, cl1=0;

    printf("ID de la sala a buscar: ");     scanf("%d",&idSearch);          //Preguntar por los datos datos
    moveRoom = searchRoom(*startRoom,idSearch);         //Manda a llamar la funcion que devuelve la sala a buscar
    if(moveRoom!=NULL)          //Detecta si siquiera la sala existe
    {
        printf("[%s]  Ingrese que  conexion desea bloquear:\n",moveRoom->nameInfo);
        moveWay = moveRoom->startWay;
        while(moveWay!=NULL)            //Ciclo que recorre la lista de conexiones
        {
            printf("%d) %s\n",cl1,  moveWay->conctRoom->nameInfo);          //Imprimir las opciones de conexiones a bloquear
            moveWay = moveWay->nextWay;
            cl1++;
        }
        scanf("%d",&idSearch);
        moveWay = moveRoom->startWay;
        for(cl1=0; cl1<idSearch; cl1++)     moveWay = moveWay->nextWay;         //ciclo que recorre los caminos hasta que alcanza el que se quiso bloquear
        moveWay->blocked = 1;           //Activar El bloqueo de la casilla
        printf("Se bloqueo la conexion [%s] -> [%s]",moveRoom->nameInfo, moveWay->conctRoom->nameInfo);
    }else   printf("No se encontro ninguna sala con esa ID\n");
}



//---------------Funcion que recorre todos los datos de una sala
int findLoop(struct roomPos *startRoom, struct roomPos *pastRoom, int findID, struct roomPos **finalRoad, short int roadType, int *lowWeight, int  newWeight)
{
    int stop, truFlag = 0, ogWeight;
    struct roomPos *moveRoom, *idk;

    if(startRoom->ID == findID)             //deteccion si la sala actual coincide ID con la sala  buscada
    {
        if(roadType==1)     truFlag = checkJump(*finalRoad, pastRoom);
        else    if(newWeight < *lowWeight || *finalRoad == NULL)
            {
                truFlag = 1;
                *lowWeight = newWeight;
            }
        if(truFlag==1 || *finalRoad == NULL)
        {
            if(*finalRoad == NULL)      *finalRoad = (struct roomPos*) malloc(sizeof(struct roomPos));
            idk = *finalRoad;           //Inicializar variables iniciales a recorrer
            moveRoom = pastRoom;
            while(moveRoom!=NULL)
            {
                strcpy(idk->nameInfo, moveRoom->nameInfo);          //Pasar lows datos de la lista de pasados a la lista definitiva
                idk->nextRoom = (struct roomPos*) malloc(sizeof(struct roomPos));           //Crear el siguiente apuntador vacios de la Lista definitiva
                moveRoom = moveRoom->nextRoom;
                idk = idk->nextRoom;
            }
            strcpy(idk->nameInfo, startRoom->nameInfo);
            idk->nextRoom = NULL;
            moveRoom = NULL;          //Borrar el ultimo elemento de los pasados,  esto porque termino esa ruta.
            return 1;
        }else   return 0;
    }

    if(pastRoom == NULL)            //detecta si es que ya xiste una lista de salas pasadas
    {
        pastRoom = (struct roomPos*) malloc(sizeof(struct roomPos));
        pastRoom->ID = startRoom->ID;           //Poner como sala pasada la sala actual
        strcpy(pastRoom->nameInfo, startRoom->nameInfo);
        pastRoom->nextRoom = NULL;          //Llenar la siguiente sala pasada como inexistente
        moveRoom = startRoom;           //devolver el movimiento a la posicion inicial
    }else
    {
        moveRoom = pastRoom;
        while(moveRoom!=NULL)          //Iniciar un ciclo en que detecta si el siguiente apuntador es vacio
        {
            if(moveRoom->ID == startRoom->ID)   return 0;           //Terminar la funcion si es que detecta que la sala actual es una dentro de la lista de pasados
            moveRoom = moveRoom->nextRoom;
        }

        moveRoom = pastRoom;
        do              //Ciclo que recoree la lista de pasados hasta que lleue al final
        {
            if(moveRoom->nextRoom!=NULL) moveRoom = moveRoom->nextRoom;
        }while(moveRoom->nextRoom!=NULL);
        moveRoom->nextRoom = (struct roomPos*) malloc(sizeof(struct roomPos));      //Crea la nueva posicion de el apuntador pasado y le pone sus datos basicos
        moveRoom->nextRoom->ID = startRoom->ID;
        strcpy(moveRoom->nextRoom->nameInfo, startRoom->nameInfo);
        moveRoom->nextRoom->nextRoom = NULL;
    }

    struct conctWay *moveWay = startRoom->startWay;         //Inicializa variable para recorrer la lista de conexiones
    ogWeight = newWeight;
    while(moveWay!=NULL)            //Iniciar un cilo que recorrera las salas conectadas a la sala inicial
    {
        if(moveWay->blocked==0)         //comprobar si se encuentra libre la conexion
        {
            newWeight= ogWeight;
            newWeight += moveWay->weight;
            findLoop(moveWay->conctRoom, pastRoom, findID, &*finalRoad, roadType, &*lowWeight, newWeight);            //Mandar la misma funcion pero ahora poniendo de sala inicial como la primera conexion de la sala  actual
        }
        moveWay = moveWay->nextWay;
    }
    if(moveRoom->nextRoom != NULL && moveRoom!=startRoom)      moveRoom->nextRoom = NULL;          //Borrar el ultimo elemento de los pasados,  esto porque termino esa ruta.
}


//------------------Inicializacion de datos para la busqueda de rutas
void startPath(struct roomPos **startRoom)
{
    struct roomPos *moveRoom, *pastRoom = NULL, *finalRoad = NULL;          //inicializart las variables de uso
    int findID, baseID, newWeight, lowWeight = 0;
    short int roadType;

    printf("Tipo de busqueda:\n1)Por saltos\n2)Por peso\n");    scanf("%hd",&roadType);         //Pedir los datos de tipo de buscqueda y salas a buscar
    printf("ID de la sala inicial:  ");     scanf("%d",&baseID);
    printf("ID de la sala a buscar:  ");     scanf("%d",&findID);
    moveRoom = searchRoom(*startRoom,baseID);         //Manda a llamar la funcion que devuelve la sala a buscar
    /*moveRoom = startRoom;
    while(moveRoom->ID != baseID)    moveRoom = moveRoom->nextRoom;         //Ciclo que  busca la sala inicial basada en la ID*/
    findLoop(moveRoom, pastRoom, findID, &finalRoad, roadType, &lowWeight, 0);          //Manzar la funcion de buscar con la sala que inicia a buscar

    if(finalRoad == NULL)    printf("No encontro alguna conexion entro las salas\n");           //Buscar si el camino encontrado existe
    else
    {
        if(roadType==1)     printf("-------------\nLista de el camino mas corto:\n");           //IMprimir dependiendo de lo que se pidio
        else    printf("-------------\nLista de el camino de menor peso:\n");
        moveRoom = finalRoad;
        while(moveRoom!=NULL)               //Ciclo que recoree el camino final
        {
            printf(" o %s\n", moveRoom->nameInfo);
            moveRoom = moveRoom->nextRoom;
        }
    }
}



//----------------El mismisimo main
int main()
{
    struct roomPos *startRoom;
    struct conctWay *moveWay;
    short int menu;
    fillMap(&startRoom);

    do
    {
        printf("\n\n1)Realizar simulacion de viaje\n2)Bloquear conexion\n3)Identificar nodo de mayor conexiones\n4)Añadir nueva sala\n5)Imprimir salas y conexiones\n");
        scanf("%hd",&menu);
        switch(menu)
        {
        case 1:
            startPath(&startRoom);  break;
        case 2:
            blockWay(&startRoom);  break;
        case 3:
            checkWays(startRoom);   break;
        case 4:
            makeRoom(&startRoom);   break;
        case  5:
            printRoom(startRoom);   break;
        }
    }while(menu!=-1);

    return 0;
}
