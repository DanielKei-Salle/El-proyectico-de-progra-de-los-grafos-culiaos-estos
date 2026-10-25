#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ibreria_grafo.h"
/*
    Ingegrantes:    Daniel Kei Márquez Okamoto,  Layla Trujillo Echeverría,  Luis Ángel Salinas García
    Grupo:  301CIB      Fehca de entrega:   26/10/2026
    Lenguaje: C
    El programa consiste a partir de una simulacion de una fabrica en la que un robot tendra que llevar distintos paquetes a las distintas zona de manera eficiente
    Esta simulacion se hara mediante el uso de grafos como representacion de las salas y pasillos conectados
*/
//main
int main()
{
    struct roomPos *startRoom;
    struct conctWay *moveWay;
    short int menu,opc,band;
    char nombrearch[50];
    band=0;
    do
    {
        printf("--MENU PRINCIPAL--\n1.Grafo precargado\n2.Grafo nuevo\n3.Seguir editando grafo\n4.Salir\n");
        scanf("%hd",&opc);
        if (band!=0)
        {
            printf("ADVERTENCIA:\nSi ya creaste un grafo y decides hacer un grafo nuevo, el anterior será eliminado");
        }
        switch (opc)
        {
        case 1:
            strcpy(nombrearch,"Grafo_precargado.txt");
            fillMap(&startRoom,nombrearch);
            break;
        case 2:
            startRoom=NULL;
            FILE *arch;
            arch=fopen("Grafo_nuevo.txt","w");
            if(arch!=NULL)
            {
                fclose(arch);
            }
            break;
        case 3:
            strcpy(nombrearch,"Grafo_nuevo.txt");
            fillMap(&startRoom,nombrearch);
            break;
        case 4:
            printf("Hasta pronto\n");
            break;
        default:
            printf("Opcion invalida\n");
            break;
        }
        do
        {
            printf("\n--MENU SECUNDARIO--\n1)Realizar simulacion de viaje\n2)Bloquear o desbloquear conexion\n3)Identificar nodo de mayor conexiones\n4)Aniadir nueva sala\n5)Agregar Camino\n6)Imprimir salas y conexiones\n7.Menu principal\n");
            scanf("%hd",&menu);
            switch(menu)
            {
            case 1:
                if(startRoom==NULL)
                {
                    printf("Primero crea una sala");
                }
                else
                {
                    startPath(startRoom);
                }
                break;
            case 2:
                if(startRoom==NULL)
                {
                    printf("Primero crea una sala");
                }
                else
                {
                    BloquearoDesbloquear(&startRoom);
                }
                break;
            case 3:
                if(startRoom==NULL)
                {
                    printf("Primero crea una sala");
                }
                else
                {
                    checkWays(startRoom);
                }
                break;
            case 4:
                makeRoom(&startRoom,nombrearch);
                break;
            case 5:
                if(startRoom==NULL)
                {
                    printf("Primero crea una sala");
                }
                else
                {
                    AnadirCamino(&startRoom,nombrearch);
                }
                break;
            case 6:
                if(startRoom==NULL)
                {
                    printf("Primero crea una sala");
                }
                else
                {
                    printRoom(startRoom);
                }
                break;
            case 7:
                break;
            default:
                printf("Opcion invalida");
            }
        }while(menu!=7);
    }while (opc!=4);

    return 0;
}
