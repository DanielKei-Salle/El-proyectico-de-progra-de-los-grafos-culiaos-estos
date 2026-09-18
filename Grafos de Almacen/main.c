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

struct roomPos
{
    struct roomPos *nextRoom;
    struct roomPos *joinRoom;
    char nameInfo[30];
};

struct conctWay
{

};


int main()
{
    struct roomPos *startRoom, *moveRoom;
    int cl1;
    startRoom = (struct roomPos*) malloc(sizeof (struct roomPos));
    moveRoom = startRoom;

    for(cl1=0;cl1<4;cl1++)
    {
        switch(cl1)
        {
        case 0:     strcpy(moveRoom->nameInfo, "xddd");     break;
        case 1:     strcpy(moveRoom->nameInfo, "Zona de carga");    break;
        case 2:     strcpy(moveRoom->nameInfo, "Control de calidad");   break;
        case 3:     strcpy(moveRoom->nameInfo, "Puertas de salida");    break;
        }
        if(cl1!=3)
        {
            moveRoom->nextRoom = (struct roomPos*) malloc(sizeof (struct roomPos));
            moveRoom = moveRoom->nextRoom;
        }else   moveRoom->nextRoom = NULL;
    }

    moveRoom = startRoom;
    while(moveRoom!=NULL)
    {
        printf("[%s]",moveRoom->nameInfo);
        moveRoom = moveRoom->nextRoom;
    }

    return 0;
}
