#ifndef IBERIA_GRAFO_H
#define IBERIA_GRAFO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct roomPos *nextRoom, *backRoom,*followRoute;
    struct conctWay *startWay;
    char nameInfo[30];
    int ID,sumWgh;

};
int ExisteCamino(struct roomPos *origen, struct roomPos *destino); //revisar si ya existe un camino de una sala a otra, regresa 1 si existe y 0 sino
void AgregarCamino(struct roomPos *origen, struct roomPos *destino, int peso,char *nombrearch);//Agregar un camino al final de la lista de conexiones de una sala
void AnadirCamino(struct roomPos **inicio,char *nombrearch);//Añade un camino entre dos salas existentes
void BloquearoDesbloquear(struct roomPos **inicio);//Bloquear o desbloquear alguna ruta  de camino
int buscarsala(struct roomPos *startRoom,char *nombrebuscado);//Busca la sala por nombre, si la encuentra regresa el ID, sino, regresa -1-->Grafo nuevo
int checkInt(int valMin, int valMax);//Validar el ingreso de enteros
short int checkRoot(struct roomPos *pastRoom, struct roomPos *selcRoom);//Checar si no se encuentra repetido en la lista de espera
void fillRoute(struct roomPos **checkRoom, struct roomPos *moveRoom);//LLenar la lista de ruta tomada de cada cuenta
short int checkShInt(short int valMin, short int valMax);//Validar el ingreso de enteros chicos
int checkJump(struct roomPos *finalRoad, struct roomPos *pastRoom);//Checar por cantidad de saltos, regresa 0 o 1 dependiendo de que ruta tiene más saltos
void checkWays(struct roomPos *startRoom);//Determinar sala con menor numero de conexiones
void fillMap(struct roomPos **startRoom,char *nombrearch);//LLenar de manera rapida el mapa y conexiones
int findLoop(struct roomPos *startRoom, struct roomPos *pastRoom, int findID, struct roomPos **finalRoad, short int roadType, int *lowWeight, int  newWeight);//Funcion que recorre todos los datos de una sala
void makeRoom(struct roomPos **startRoom,char *nombrearch);//Añadir una sala al grafo
void printRoom(struct roomPos *startRoom);//Immprimir todas las salas del grafo
struct roomPos *searchRoom(struct roomPos *startRoom, int idSearch);//Buscar la sala de un grafo a partir e su ID, retorna el apuntador al nodo si la encuentra, sino, regresa un apuntador nulo
void startPath(struct roomPos *startRoom);//Inicializacion de datos para la busqueda de rutas
short int bfsLoop(struct roomPos *startRoom, int findID,  short int roadType);//Funcion recursiva que recorre en sistem BFS
void guardarcamino(struct roomPos *origen, struct roomPos *destino, int peso);//Da de alta origen,destino y peso (si no existen)en el archivo creado por el usuario
//---------------------Validar el ingreso de enteros
int checkInt(int valMin, int valMax)
{
    int check,input;
    do
    {
        check = scanf("%d",&input);             //Ingresa los datos dentro de otro entero que guarda si es un entero
        if(check!=1)
        {
            scanf("%*[^\n]");           //Detecta si pudo guardar el valor,  sino limpia el buffer
            printf("Error,  Ingrese un dato valido      ");
        }
    }while(check!=1 || input<valMin || input>valMax);           //Reinicia el ciclo si acepta el valor
    return input;
}
//-----------------Validar el ingreso de numeros enteros chicos
short int checkShInt(short int valMin, short int valMax)
{
    short int check,input;
    do
    {
        check = scanf("%d",&input);             //Ingresa los datos dentro de otro entero que guarda si es un entero
        if(check!=1 || input<valMin || input>valMax)
        {
            scanf("%*[^\n]");           //Detecta si pudo guardar el valor,  sino limpia el buffer
            printf("Error,  Ingrese un dato valido      ");
        }
    }while(check!=1 || input<valMin || input>valMax);           //Reinicia el ciclo si acepta el valor
    return input;
}
//Busca la sala por nombre, si la encuentra regresa el ID, sino, regresa -1-->Grafo nuevo
int buscarsala(struct roomPos *startRoom,char *nombrebuscado)
{
    struct roomPos *moveRoom;
    moveRoom=startRoom;
    while(moveRoom!=NULL)
    {
        if(strcmp(moveRoom->nameInfo,nombrebuscado)==0)
        {
            return moveRoom->ID;
        }
        moveRoom=moveRoom->nextRoom;
    }
    return -1;
}
//--------------------Checar si no se encuentra repetido en la lista de espera
short int checkRoot(struct roomPos *pastRoom, struct roomPos *selcRoom)
{
    struct roomPos *moveRoom = pastRoom;

    if(moveRoom == NULL)  return 0;
    while(moveRoom!=NULL)
    {
        if(selcRoom->ID==moveRoom->ID)  return 0;
        moveRoom = moveRoom->nextRoom;
    }
    return 1;
}
//-------------------------------------LLenar la lista de ruta tomada de cada cuenta
void fillRoute(struct roomPos **checkRoom, struct roomPos *moveRoom)
{
    struct roomPos *extRoom;

    extRoom = moveRoom->followRoute;
    *checkRoom = (*checkRoom)->followRoute;
    while(extRoom!=NULL)            //Ciclo que recorre las salas por las que paso y la pasa a la siguiente
    {
        **checkRoom = *extRoom;             //Coloca los datos al nuevo apuntador
        (*checkRoom)->nextRoom = (struct roomPos*) malloc(sizeof(struct roomPos));      //crea el siguiente apuntador
        *checkRoom = (*checkRoom)->nextRoom;            //mueve los dos apuntadores
        extRoom = extRoom->nextRoom;
    }
    **checkRoom = *moveRoom;         //Le agrega la sala actual a la lista por las que paso para usarlo la siguiente vez
    (*checkRoom)->nextRoom = NULL;
}

//-----------------------------------------LLenar de manera rapida el mapa y conexiones
void fillMap(struct roomPos **startRoom,char *nombrearch)
{
    short int cl1,cl2,i,j,k;
    int peso,idor,idest;
    struct roomPos *moveRoom;
    struct roomPos *or;
    struct roomPos *dest;
    struct roomPos *nueva;
    char linea[100],origen[30],destino[30],cpeso[10];
    if(strcmp(nombrearch,"Grafo_precargado.txt")==0)
    {
        *startRoom=(struct roomPos*)malloc(sizeof (struct roomPos));         //Crear el primer espacio para el primer apuntador
        (*startRoom)->startWay=NULL;
        (*startRoom)->nextRoom=NULL;
        (*startRoom)->backRoom=NULL;
        moveRoom = *startRoom;
        for(cl1=0;cl1<8;cl1++)          //Ciclo que repite 8 veces para llenar todos los datos
        {
            moveRoom->ID  = cl1;
            switch(cl1)         //Escoje los datos a llenar de cada uno de los apuntadoes
            {
            case 0:     strcpy(moveRoom->nameInfo, "Estanteria");     break;
            case 1:     strcpy(moveRoom->nameInfo, "Zona de carga");    break;
            case 2:     strcpy(moveRoom->nameInfo, "Control de calidad");   break;
            case 3:     strcpy(moveRoom->nameInfo, "Zona de descarga");    break;
            case 4:     strcpy(moveRoom->nameInfo, "Area de empaquetado");    break;
            case 5:     strcpy(moveRoom->nameInfo, "Almacen frio");    break;
            case 6:     strcpy(moveRoom->nameInfo, "Mantenimiento");    break;
            case 7:     strcpy(moveRoom->nameInfo, "Puerta de salida");    break;
            }
            if(cl1!=7)          //Crea un nuevo espacio vacio al siguiente apuntador si aun queda espacio del ciclo
            {
                moveRoom->nextRoom = (struct roomPos*) malloc(sizeof (struct roomPos));
                moveRoom->nextRoom->startWay=NULL;
                moveRoom->nextRoom->nextRoom=NULL;
                moveRoom->nextRoom->backRoom = moveRoom;
                moveRoom = moveRoom->nextRoom;
            }
            else
            {
                moveRoom->nextRoom = NULL;
            }
        }
    }
    else
    {
        *startRoom=NULL;//-->Apuntador a nulo
    }
    FILE *arch;
    arch=fopen(nombrearch,"r");//-->Escribe en el archivo sin importar el nombre del mismo
    if (arch==NULL)
    {
        printf("Error al abrir el archivo");
    }
    else
    {
        while(fgets(linea,sizeof(linea),arch)!=NULL)
        {
            i=0;
            while(linea[i]!='\0')
            {
                if(linea[i]=='\n')
                {
                    linea[i]='\0';
                }
                else
                {
                    i+=1;
                }
            }
            j=0;
            k=0;
            while(linea[k]!='\0'&&linea[k]!=';')
            {
                origen[j]=linea[k];
                j+=1;
                k+=1;
            }
            origen[j]='\0';//-->Obtiene el nodo de origen
            if (linea[k]==';')
            {
                k+=1;
            }
            j=0;
            while(linea[k]!='\0'&&linea[k]!=';')
            {
                destino[j]=linea[k];
                j+=1;
                k+=1;
            }
            destino[j]='\0';//-->Obtiene el nodo de destino
            if (linea[k]==';')
            {
                k+=1;
            }
            j=0;
            while(linea[k]!='\0'&&linea[k]!='\n')
            {
                cpeso[j]=linea[k];
                j+=1;
                k+=1;
            }
            cpeso[j]='\0';//-->Obtiene el peso en forma de string
            peso=0;
            i=0;
            while(cpeso[i]!='\0')//-->Convierte el string a int
            {
                if(cpeso[i]>='0'&&cpeso[i]<='9')
                {
                    peso=(peso*10)+(cpeso[i]-'0');
                }
                i+=1;
            }
            if (strcmp(nombrearch,"Grafo_precargado.txt")!=0)//-->Si el archivo es "Grafo_nuevo.txt"
            {                                                                                                   //-->Se encarga de volver a acomodar los pauntadores y el espacio en la memoria del grafo que ya se habia creado
                if(buscarsala(*startRoom,origen)==-1)//-->Si es la primera sala, la carga con el ID 0
                {
                    nueva=(struct roomPos*)malloc(sizeof(struct roomPos));
                    strcpy(nueva->nameInfo, origen);
                    nueva->startWay=NULL;
                    nueva->nextRoom=NULL;
                    if(*startRoom==NULL)
                    {
                        nueva->backRoom=NULL;
                        nueva->ID=0;
                        *startRoom=nueva;
                    }
                    else//-->No es la primera sala, le agrega el ID correspondiente
                    {
                        moveRoom=*startRoom;
                        while(moveRoom->nextRoom!=NULL)
                        {
                            moveRoom=moveRoom->nextRoom;
                        }
                        nueva->backRoom=moveRoom;
                        nueva->ID=moveRoom->ID+1;
                        moveRoom->nextRoom=nueva;
                    }
                }
                if(buscarsala(*startRoom,destino)==-1) //-->Asigna el ID de destino
                {
                    nueva=(struct roomPos*)malloc(sizeof(struct roomPos));
                    strcpy(nueva->nameInfo,destino);
                    nueva->startWay=NULL;
                    nueva->nextRoom=NULL;
                    moveRoom=*startRoom;
                    while (moveRoom->nextRoom!=NULL)
                    {
                        moveRoom=moveRoom->nextRoom;
                    }
                    nueva->backRoom=moveRoom;
                    nueva->ID=moveRoom->ID+1;
                    moveRoom->nextRoom=nueva;
                }
            }
            //-->Las salas ya estan dadas de alta con el ID
            idor=buscarsala(*startRoom,origen);//-->Regresa el ID de origen
            idest=buscarsala(*startRoom,destino);//-->Regresa el ID de destino
            if(idor!=-1&&idest!=-1)
            {
                or=searchRoom(*startRoom,idor);//-->Regresa el apuntador basado en sus ID
                dest=searchRoom(*startRoom,idest);
                if(or!=NULL && dest!=NULL && ExisteCamino(or,dest)==0)//-->Si no hay conexion entre ellos
                {
                    AgregarCamino(or,dest,peso,"");//-->Crea la conexion
                }
            }
        }
        fclose(arch);
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
    printf("Sala con mas conexiones [%s]\nNumero de conexiones: %d",extRoom->nameInfo, maxWay);
}



//------------------------------Buscar la sala de un grafo a partir e su ID, retorna el apuntador al nodo si la encuentra, sino, regresa un apuntador nulo
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

//Agregar un camino al final de la lista de conexiones de una sala
void AgregarCamino(struct roomPos *origen, struct roomPos *destino, int peso,char *nombrearch)
{
    struct conctWay *nuevo = (struct conctWay*) malloc(sizeof(struct conctWay));
    struct conctWay *aux;
    short int band=0,i,j,k;
    nuevo->conctRoom = destino;
    nuevo->weight = peso;
    nuevo->blocked = 0;            //Siempre inicia desbloqueado
    nuevo->nextWay = NULL;
    if(origen->startWay == NULL)
    {
        origen->startWay = nuevo;        //Si la sala no tenia caminos, este es el primero
    }
    else
    {
        aux = origen->startWay;
        while(aux->nextWay != NULL)
        {
            aux = aux->nextWay;     //Ir hasta el ultimo camino para conectar
        }
        aux->nextWay = nuevo;
    }
    if (strcmp(nombrearch,"Grafo_nuevo.txt")==0)
    {
        guardarcamino(origen,destino,peso);//Solamente es utilizada si el archivo es el creado por el usuario, de lo contrario se mezclan archivos
    }
}
//Da de alta origen,destino y peso (si no existen)en el archivo creado por el usuario
void guardarcamino(struct roomPos *origen, struct roomPos *destino, int peso)
{
    char linea[100],o[30],d[30];
    short int i,j,k,band=0;
    FILE *arch;
    arch=fopen("Grafo_nuevo.txt","r");//-->Abre archivo nuevo
    if(arch!=NULL)
    {
        char linea[100],o[30],d[30];
        while(fgets(linea,sizeof(linea),arch)!=NULL)
        {
            i=0;
            while(linea[i]!='\0')
            {
                if(linea[i]=='\n')
                {
                    linea[i]='\0';
                }
                else
                {
                    i+=1;
                }
            }
            j=0;
            k=0;
            while(linea[k]!='\0'&&linea[k]!=';')
            {
                o[j]=linea[k];
                j+=1;
                k+=1;
            }
            o[j]='\0';//-->Obtiene origen
            if (linea[k]==';')
            {
                k+=1;
            }
            j=0;
            while(linea[k]!='\0'&&linea[k]!=';')
            {
                d[j]=linea[k];
                j+=1;
                k+=1;
            }
            d[j]='\0';//-->Obtiene destino
            if(strcmp(o,origen->nameInfo)==0 && strcmp(d,destino->nameInfo)==0)//Si el origen y destino obtenidos son los buscados ya estan en el archivo nuevo
            {
                band=1; //-->Sale de la funcion
                break;
            }
        }
        fclose(arch);
    }
    if(band!=1)//Si aun no estan= union nueva
    {
        arch=fopen("Grafo_nuevo.txt","a");//-->Agrega la conexion al archivo por medio del formato (origen;destino;peso)
        if(arch!=NULL)
        {
            fprintf(arch,"%s;%s;%d\n",origen->nameInfo,destino->nameInfo,peso);
            fclose(arch);
        }
    }
}

//revisar si ya existe un camino de una sala a otra
int ExisteCamino(struct roomPos *origen, struct roomPos *destino)
{
    struct conctWay *aux =origen->startWay;
    while(aux != NULL)
    {
        if(aux->conctRoom == destino)
        {
            return 1;
        }
        aux = aux->nextWay;
    }
    return 0;
}

//Añade un camino entre dos salas existentes
void AnadirCamino(struct roomPos **inicio,char *nombrearch)
{
    struct roomPos *origen, *destino;
    int idOrigen, idDestino, peso, bid;
    printf("ID de la sala de origen: ");
    idOrigen = checkInt(0, 9999);
    origen = searchRoom(*inicio, idOrigen);
    if(origen == NULL)                            //Busca la sala de origen con el ID
    {
        printf("No se encontro ninguna sala con esa ID\n");
        return;
    }
    printf("ID de la sala destino: ");
    idDestino = checkInt(0, 9999);
    destino = searchRoom(*inicio, idDestino);
    if(destino == NULL)                           //Busca la sala de destino con el ID
    {
        printf("No se encontro ninguna sala con esa ID\n");
        return;
    }

    if(origen == destino)                         //Valida que no se apunte a si misma
    {
        printf("Una sala no puede conectarse consigo misma\n");
        return;
    }
    if(ExisteCamino(origen, destino))             //Valida si ya existe el camino
    {
        printf("Ya existe un camino [%s] -> [%s]\n", origen->nameInfo, destino->nameInfo);
        return;
    }
    printf("Peso del camino: ");                  //Si llega hasta aqui, solicita peso y si es de una direccion o bidireccional
    peso = checkInt(1, 1000);
    printf("Camino de ida y vuelta? Si(1)  No(0)  ");
    bid = checkInt(0, 1);
    AgregarCamino(origen, destino, peso,nombrearch);         //Agrega el camino
    printf("Se agrego el camino [%s] -> [%s]\n", origen->nameInfo, destino->nameInfo);
    if(bid == 1 && !ExisteCamino(destino, origen))  //Si es bidireccional tambien agrega el camino
    {
        AgregarCamino(destino, origen, peso,nombrearch);
        printf("Se agrego el camino [%s] -> [%s]\n", destino->nameInfo, origen->nameInfo);
    }
}

//-----------------------------------Añadir una sala al grafo
void makeRoom(struct roomPos **startRoom,char *nombrearch)
{
    if(*startRoom==NULL)//-->Si es un archivo nuevo
    {
        struct roomPos *or;
        struct roomPos *dest;
        char origen[30],destino[30];
        int peso;
        printf("Origen:\t");
        scanf("%s",&origen);
        printf("Destino:\t");
        scanf("%s",&destino);
        printf("Peso:\t");
        peso=checkInt(1,100);
        or=(struct roomPos*)malloc(sizeof(struct roomPos));
        strcpy(or->nameInfo,origen);
        or->startWay=NULL;
        or->nextRoom=NULL;
        or->backRoom=NULL;
        or->ID=0; //-->Origen es el primer nodo
        *startRoom=or;
        dest=(struct roomPos*)malloc(sizeof(struct roomPos));
        strcpy(dest->nameInfo,destino);
        dest->startWay=NULL;
        dest->nextRoom=NULL;
        dest->backRoom=or;
        dest->ID=1;//Destino es el segundo nodo
        or->nextRoom=dest;
        AgregarCamino(or,dest,peso,nombrearch); //-->Une ambos nodos
        FILE *arch;
        arch=fopen("Grafo_nuevo.txt","w");//-->Abre el archivo y los escribe en el formato de (origen;destino;peso)
        if (arch!=NULL)
        {
            fprintf(arch,"%s;%s;%d\n",origen,destino,peso);
            fclose(arch);
        }
    }
    else
    {
        short int addLoop;
        int idSearch, peso;
        struct roomPos *moveRoom = *startRoom, *newRoom, *extRoom;
        newRoom = (struct roomPos*) malloc(sizeof(struct roomPos));             //Crear una nueva sala vacia

        newRoom->nextRoom = NULL;           //Poner las conexiones de la nueva sala como vacios
        newRoom->startWay = NULL;
        do
        {
            printf("Ingrese el Nombre de la sala a agregar:  ");            //Ingresar el nombre de la sala
            scanf(" ");     gets(newRoom->nameInfo);
            if(buscarsala(*startRoom,newRoom->nameInfo)!=-1)
            {
                printf("Ya existe una sala con ese nombre\n");
            }
        }while(buscarsala(*startRoom,newRoom->nameInfo)!=-1);
        printf("Desea agrgar conexiones a esta sala?  Si(1)  No(0)  ");
        addLoop=checkInt(0,1);
        while(addLoop==1)               //Ciclo que repite cada vez que se pretende agregar conexiones
        {
            do
            {
                printf("Ingrese la ID de la sala a conectar:  ");
                idSearch=checkInt(0,9999);          //Buscar la sala con la funcion y repetir si no encuenta
                extRoom = searchRoom(*startRoom, idSearch);
                if(extRoom==NULL)   printf("Error,  no se encontro una sala con esa ID\n");
            }while(extRoom==NULL);

            printf("Sala Conectada [%s]\n",extRoom->nameInfo);
            printf("Ingrese el peso de la conexion con esa sala:  ");
            peso=checkInt(1, 1000);
            AgregarCamino(newRoom, extRoom, peso,nombrearch);              //Utiliza la funcion para crear la conexion
            guardarcamino(newRoom,extRoom,peso);
            printf("Desea agregar mas conexiones a esta sala?  Si(1)  No(0) ");   scanf("%hd",&addLoop);
        }

        while(moveRoom->nextRoom!=NULL)     moveRoom = moveRoom->nextRoom;          //recorrer todas las salas
        newRoom->backRoom = moveRoom;           //Colcoar los apuntadores correctos a la nueva sala
        newRoom->ID = moveRoom->ID + 1;         //Añadirle la ID a la nueva sala considerando la sala mas alta + 1
        moveRoom->nextRoom = newRoom;
        printf("Se añadio la sala %s\n",newRoom->nameInfo);
    }
}


//--------------------------------------------Immprimir todas las salas del grafo
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


//Bloquear o desbloquear alguna ruta  de camino
void BloquearoDesbloquear(struct roomPos **inicio)
{
    struct roomPos *origen;
    struct conctWay *aux;
    int idOrigen, opcion, i, total=0;
    if(*inicio==NULL)                           //Valida que existan salas
    {
        printf("El grafo esta vacio\n");
        return;
    }
    printf("ID de la sala de origen: ");         //Solicita el ID de la sala donde se va a bloquear o desbloquear
    idOrigen=checkInt(0, 9999);
    origen=searchRoom(*inicio, idOrigen);
    if(origen==NULL)                            //Busca la sala con el ID
    {
        printf("No se encontro ninguna sala con esa ID\n");
        return;
    }
    if(origen->startWay==NULL)                  //Valida que la sala tenga caminos para poder bloquear o desbloquear, si no retorna
    {
        printf("La sala [%s] no tiene caminos\n", origen->nameInfo);
        return;
    }
    printf("[%s]  Elija la conexion a bloquear o desbloquear:\n", origen->nameInfo);
    aux=origen->startWay;
    while(aux!=NULL)                            //Imprime los caminos con su estado actual
    {
        printf("%d) %s  (Peso: %d)  - ", total, aux->conctRoom->nameInfo, aux->weight);
        if(aux->blocked==0)    //Imprime el estado
        {
            printf("Libre\n");
        }
        else
        {
            printf("Bloqueado\n");
        }
        aux=aux->nextWay;
        total++;
    }
    opcion=checkInt(0, total-1);              //Solo acepta opciones que existen
    aux = origen->startWay;
    for(i=0; i<opcion; i++)                   //Avanza hasta el camino elegido
    {
        aux=aux->nextWay;
    }
    if(aux->blocked == 0)                         //Si estaba libre, lo bloquea con la bandera
    {
        aux->blocked = 1;
        printf("Se bloqueo la conexion [%s] -> [%s]\n", origen->nameInfo, aux->conctRoom->nameInfo);
    }
    else                                          //Si estaba bloqueado, lo desbloquea con la bandera
    {
        aux->blocked=0;
        printf("Se desbloqueo la conexion [%s] -> [%s]\n", origen->nameInfo, aux->conctRoom->nameInfo);
    }
}

//-------------Checar por cantidad de saltos, regresa 0 o 1 dependiendo de que ruta tiene más saltos
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

//--------------Funcion recursiva que recorre en sistem BFS
short int bfsLoop(struct roomPos *startRoom, int findID,  short int roadType)
{
    struct roomPos *moveRoom, *listRoom, *checkRoom, *extRoom;          //Inicializa las variables a usar
    struct conctWay *moveWay;
    int stop, extWgh, jumpAmo = 0;
    startRoom->followRoute = NULL;          //Pone la ruta usada de la sala inicial como vacia
    startRoom->sumWgh = 0;           //Pone la suma de pesosç inicial como 0

    listRoom = (struct roomPos*) malloc(sizeof(struct roomPos));            //coloca el primer elemento de la lista de espera
    *listRoom = *startRoom;
    listRoom->nextRoom = NULL;

    moveRoom = listRoom;            //Inicializa los datos
    while(moveRoom!=NULL)           //Ciclo que recorre la listra de espera
    {
        if(moveRoom->ID==findID && roadType==1)          //Funcion que detecta  si encontro la sala,  solo lo hace en la busqueda por saltos
        {
            printf("---------------Ruta de menor numero de saltos------------------------------\n",moveRoom->nameInfo);
            checkRoom = moveRoom->followRoute;
            while(checkRoom!=NULL)          //Ciclo que imprime la ruta de guardados que tiene de salas
            {
                jumpAmo++;          //Suma la cantidad de saltos hechos para llegar a la sala
                printf("o %s\n",checkRoom->nameInfo);
                checkRoom = checkRoom->nextRoom;
            }
            printf("o %s\n",moveRoom->nameInfo);
            printf("(  Saltos hechos:  [%d]  )\n\n",jumpAmo);
            return 1;           //Termina la funcion como 1 (osea sala encontrada)

        }else
        {
            moveWay = moveRoom->startWay;
            while(moveWay!=NULL)            //Ciclo que recorre los caminos de la sala actual en la lista
            {
                moveWay->conctRoom->sumWgh = 0;         //Reinicia la el peso de la sala a conectar a 0
                if(checkRoot(listRoom, moveWay->conctRoom) == 1 && moveWay->blocked==0)         //Manda a llamar a la funcion a detectar si ya se paso por esa sala,  o si esta bloqueada
                {
                    checkRoom = moveRoom;               //Llenar los datos de el siguiente en los de sala de espera con la sala que conecta
                    while(checkRoom->nextRoom!=NULL)    checkRoom = checkRoom->nextRoom;
                    checkRoom->nextRoom = (struct roomPos*) malloc(sizeof(struct roomPos));
                    *(checkRoom->nextRoom) = *(moveWay->conctRoom);
                    checkRoom->nextRoom->nextRoom = NULL;

                    checkRoom = searchRoom(listRoom, moveWay->conctRoom->ID);           //Coloca el apuntador en la direccion de la sala conectada
                    checkRoom->followRoute = (struct roomPos*) malloc(sizeof(struct roomPos));
                    if(moveRoom->followRoute==NULL)         //Detecta si ya tiene una ruta de llegada inicial para crear una o seguir la siguiente
                    {
                        *(checkRoom->followRoute) = *moveRoom;
                        checkRoom->followRoute->nextRoom = NULL;
                    }else   fillRoute(&checkRoom, moveRoom);        //funcion para llenar la nueva ruta a tomar
                }

                if(roadType==2 && moveWay->blocked==0)         //Funcion para el calculo de peso
                {
                    checkRoom = searchRoom(listRoom, moveWay->conctRoom->ID);           //coloca el apuntador en la sala apuntada del pasullo
                    extWgh = moveRoom->sumWgh + moveWay->weight;
                    if(extWgh < checkRoom->sumWgh || checkRoom->sumWgh==0)      //Compara si el peso actual es mayor que el peso que seria por la nueva ruta
                    {
                        checkRoom->sumWgh = extWgh;         //Funcion que cambia la ruta conectada a la sala actual por la nueva
                        if(checkRoom->sumWgh!=0)
                        {
                            checkRoom = searchRoom(listRoom, moveWay->conctRoom->ID);           //Coloca el apuntador en la direccion de la sala conectada
                            checkRoom->followRoute = (struct roomPos*) malloc(sizeof(struct roomPos));
                            fillRoute(&checkRoom, moveRoom);
                        }
                    }
                }
                moveWay = moveWay->nextWay;         //Avanzar al siguiente pasillo conectados a la sala
            }
        }
        free(moveRoom->followRoute);
        moveRoom = moveRoom->nextRoom;          //avanzar a la siguiente sala en la lista de espera
    }

    moveRoom = searchRoom(listRoom, findID);
    if(roadType==2 && moveRoom!=NULL)         //funcion que realiza al terminar si esta calculando por peso,  solo imprime el dato con el que finalizo el dato
    {
        moveRoom = searchRoom(listRoom, findID);
        printf("---------------Ruta de menor peso------------------------------\n",moveRoom->nameInfo);
        checkRoom = moveRoom->followRoute;
        while(checkRoom!=NULL)
        {
            printf("o %s\n",checkRoom->nameInfo);
            checkRoom = checkRoom->nextRoom;
        }
        printf("o %s\n",moveRoom->nameInfo);
        printf("( Peso de conexion: [%d] )\n\n", moveRoom->sumWgh);
        return 1;
    }
    return 0;
}

//------------------Inicializacion de datos para la busqueda de rutas
void startPath(struct roomPos *startRoom)
{
    struct roomPos *moveRoom, *extRoom;          //inicializart las variables de uso
    int findID, baseID, newWeight, lowWeight = 0;
    short int roadType, truRoute = 0;

    printf("Tipo de busqueda:\n1)Por saltos\n2)Por peso\n");    roadType = checkShInt(1,2);         //Pedir los datos de tipo de buscqueda y salas a buscar
    printf("ID de la sala inicial:  ");     baseID = checkInt(0,99);
    printf("ID de la sala a buscar:  ");     findID = checkInt(0,99);
    moveRoom = searchRoom(startRoom,baseID);         //Manda a llamar la funcion que devuelve la sala a buscar
    extRoom=searchRoom(startRoom,findID);
    if(moveRoom!=NULL && extRoom!=NULL)
    {
        truRoute = bfsLoop(moveRoom, findID, roadType);          //Manzar la funcion de buscar con la sala que inicia a buscar
        if(truRoute == 0)    printf("No encontro alguna conexion entro las salas\n");           //Buscar si el camino encontrado existe
    }else       printf("Alguna de las salas ingresadas no existe\n");
}

#endif
