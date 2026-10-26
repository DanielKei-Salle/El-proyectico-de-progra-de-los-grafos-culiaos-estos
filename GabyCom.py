#Examen 2
#25/10/2026
#Lenguaje en phyton
#Marquez Okamoto Daniel Kei
#Salinas García Luis Angel
#Trujillo Echeverria Layla
import networkx as nx                       #Libreria de grafos
import matplotlib.pyplot as plt             #Libreria de gráficos

#Funcion encargada de buscar la mayor cantidad de conexiones
def MaxConexiones(G:nx.DiGraph):
    if G.number_of_nodes()==0:              #Valida que si no hay nodos, devuelva un vacio
        raise ValueError("Grafo vacio")
    nodo=max(G.nodes(),key=lambda n:G.out_degree(n))    #Evalua cada nodo con la funcion max, para encontrarlo
    grado=G.out_degree(nodo)
    return nodo,grado                       #Devuelve el nodo y cantidad de conexiones

#Funcion encargada de mostrar el grafo
def VerGrafo(G:nx.DiGraph):
    pos=nx.spring_layout(G)              #Calcula la posicion de los nodos por medio de una distribucion de fuerzas
    fig,ax=plt.subplots(figsize=(8, 5))
    nx.draw(G, pos, with_labels=True, node_color='lightblue', edge_color='lightpink', node_size=200, arrowsize=5)   #Dibuja los nodos y aristas
    labels=nx.get_edge_attributes(G, 'weight')    #Extrae el peso
    nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)     #Coloca el peso en las aristas
    plt.show()            #imprime

#Funcion que permite cambiar el peso al modificar la arista
def CambiarPeso(G:nx.DiGraph):
    lugar=input("Nombre 1:\t")
    ragul=input("Nombre 2:\t")      #Solicita el nombre de dos nodos
    if (G.has_node(lugar) and G.has_node(ragul)):      #Valida que no sean iguales
        while True:
            pesopesado=int(input("Peso:\t"))
            if pesopesado>0 and pesopesado<50:         #un rango de peso de 1-49
                break
            else:
                print("Peso invalido")
        G.add_edge(lugar,ragul,weight=pesopesado)      #Actualiza peso
    else:
        print("Uno o ambos nodos no existen")

#Compurbe la conexion de nodos
def ValidarSentido(G:nx.DiGraph):
    origen=input("Origen:\t")
    final=input("Final:\t")               #Solicita 2 nodos
    if (G.has_node(origen) and G.has_node(final)):
        conexiondirecta=G.has_edge(origen,final)     #Busca si hay conexion de una direcion
        if(conexiondirecta):
            print("Si hay conexion directa")
        else:
            print("No hay conexion directa")
        conexionambossentidos=G.has_edge(origen,final) and G.has_edge(final,origen)
        if (conexionambossentidos):
            print("Si hay conexion bidireccional")   #Busca si hay conexion de ambas direcciones
        else:
            print("No hay conexion bidireccional")
    else:
        print("Uno o ambos nodos no existen en el grafo")

#Funcion para bloquear un nodo de manera temporal y visualizar que sucede
def BloquearNodo(G: nx.DiGraph):
    if G.number_of_nodes()==0:
        print("El grafo está vacío.")
        return
    ZonaBloquear=input("Ingrese el nombre de la ubicación a bloquear:\t")   #Solicita la zona
    if not G.has_node(ZonaBloquear):     #Busca la zona en elo grafo
        print("El nodo {} no existe en el grafo".format(ZonaBloquear))
        return
    G_temp=G.copy()      #Genera una copia temporal
    G_temp.remove_node(ZonaBloquear)     #Elimina esa zona del grafo temporal
    ZonasDebiles=list(nx.weakly_connected_components(G_temp))     #Busca cuantos nodos quedan desconectados
    num_zonas=len(ZonasDebiles)
    print("Como queda el grafo tras bloquear {}".format(ZonaBloquear))    #Actualizacion del grafo para verlo
    print("Número de zonas o subgrupos independientes resultantes: {}\n".format(num_zonas))
    for i, zona in enumerate(ZonasDebiles, 1):
        print("Zona/Grupo {} ({} ubicaciones): {}".format(i, len(zona), ', '.join(zona)))    #Va recorriendo la distancia e imprime las zonas desconectadas
    print()

#Funcion para agregar un nodo y si lo desea lo conecta
def AgregarNodo(G:nx.DiGraph):
    nodo=input("Nombre Nodo:\t")
    if G.has_node(nodo):      #Valida que no exista el nodo
        print("El nodo",nodo,"ya existe")
    else:
        G.add_node(nodo)      #Agrega el nodo
        d=input("Destino:\t") #Solicita el destino
        if G.has_node(d):
            while True:
                p=int(input("Peso de la conexión(0-50):\t"))   #    Pide el peso
                if p>0 and p<50:
                    break
                else:
                    print("Peso invalido")
            G.add_edge(nodo,d,weight=p)     #Agrega el peso
            print(f"Conexión creada:{nodo}->{d} con peso {p}.")
        else:               #Si el nodo de destino no existe, pregunta si quiere crearlo tambien y repite proceso
            print(f"El nodo de destino '{d}' no existe en el grafo.")
            while True:
                pregunta=int(input("Desea crear el nodo de destino?\n1.Si\n2.No\n"))
                if pregunta==1:
                    while True:
                        p = int(input("Peso de la conexión(0-50):\t"))
                        if p > 0 and p < 50:
                            break
                        else:
                            print("Peso invalido\n")
                    G.add_node(d)
                    G.add_edge(nodo,d,weight=p)
                    break
                elif pregunta==2:
                    print("Nodo creado pero no conectado\n")
                    break
                else:
                    print("Opcion invalid\nIntente de nuevo")

#Realiza el recorrido DFS, sin recursion explicita
def RecorrerDFS(G:nx.DiGraph):
    if G.number_of_nodes()==0:
        print("Grafo vacio")
        return
    v=set()         #Conjunto de nodos visitados
    total=[]        #Lista de la secuencia final
    for nodoinicial in G.nodes():     #Recorre cada seccion del grafo
        if nodoinicial not in v:      #si aun no ha sido visitado, inicia la busqueda
            fila=[nodoinicial]        #Pila que guarda los nodos pendientes
            while len(fila)>0:         #Recoore hasta que no haya nodos en la pila
                nodoactual=fila.pop()    #Saca el ultimo nodo insertado en la pila
                if nodoactual not in v:  #Verifica que el nodo extraido no haya sido visitado
                    v.add(nodoactual)     #Guarda que fue visitado
                    total.append(nodoactual)  #Lo añade a la secuencia final
                    for a in G.successors(nodoactual):    #Obtiene todos los nodos a los que apunta
                        if a not in v:                    #Si aun no es visitado, lo agrega a la fila para explorarlo
                            fila.append(a)
    print("->".join(total))     #Imprime el recorrido

#Encuentra la ruta optima entre dos puntos por medio de la suma minina de pesos
def Dijkstra(G: nx.DiGraph):
    entrada = input("Lugar de Entrada:\n")
    salida = input("Lugar de Salida:\n")
    if G.has_node(entrada) and G.has_node(salida):    #Busca que si existan
        if nx.has_path(G, entrada, salida):    #Compuerba que sea alcanzable el destino
            ruta = nx.shortest_path(G, source=entrada, target=salida, weight='weight')   #Busca la ruta mas economica en peso
            peso_total = nx.shortest_path_length(G, source=entrada, target=salida, weight='weight')   #Calcula el peso total
            print("Ruta óptima basada en pesos (Dijkstra):\n")
            print("-->".join(ruta))
            print("Costo total del recorrido: {}\n" .format(peso_total))
        else:
            print("No existe ningún camino posible entre esas rutas\n")
    else:
        print("Uno o ambos lugares no existen en el grafo")

#Encuentra la ruta con menor numero de saltos(aristas) entre dos nodos ignorando el peso
def CaminoCorto (G:nx.DiGraph):
    while True:
        entrada=input("Lugar de Entrada:\t")
        salida=input("Lugar de Salida:\t")
        if G.has_node(entrada) and G.has_node(salida):   #Busca los valores en el grafo
            c=[entrada]     #Se va a encargar de guardar los nodos que aun  no se han explorado
            visitados={entrada}   #Guarda los nodos ya visitados, para no repetir
            padre={entrada:None}     #Diccionario de rastreo que guarda de donde vienen los nodos para armar la ruta
            encontrado=False
            while len(c)>0:
                nodo_actual=c.pop(0)      #Extrae el primer nodo
                if nodo_actual==salida:
                    encontrado=True
                    break
                for i in G.successors(nodo_actual):    #Revisa a los nodos que apunta
                    if i not in visitados:
                        visitados.add(i)
                        padre[i]=nodo_actual    #Guarda que a "i" se llega desde el nodo actual
                        c.append(i)       #Añade al nodo
            if encontrado:
                ruta=[]
                paso=salida
                while paso is not None:   #Construye desde la salida a la entrada
                    ruta.append(paso)
                    paso=padre[paso]
                ruta.reverse()  #Invierte el esentido
                saltos=len(ruta)-1
                print(f"\nRuta con menor número de saltos ({saltos} conexiones):")
                print("->".join(ruta))    #Imprime la ruta
            else:
                print(f"\nNo existe ningún camino posible entre '{entrada}' y '{salida}'.")
            break
        else:
            print("Intente nuevamente")
cont=0
band=0
#Menu de archivos
while True:
    if cont!=0:
        print("ADVERTENCIA!\nSi decides crear un grafo nuevo y ya habias creado uno anteriormente, este se borrara\n")
    opc1=int(input("--BIENVENIDO--\nSeleccione un metodo:\n1.Crear grafo nuevo\n2.Usar grafo precargado\n3.Seguir editando mi grafo\n4.Salir\n"))
    cont=1
    match(opc1):
        case 1:
            G=nx.DiGraph()
            nx.write_weighted_edgelist(G,"archivo_cero.txt",delimiter=";")  #Genera el archivo con un grafo vacio
            print("Archivo creado\n")
        case 2:    #Carga un grafo desde un archivo precargado y castea los pesos a enteros, y leyendo las aristas separadas por medio de ";"
            G=nx.read_weighted_edgelist("Grafo_precargado.txt", delimiter=";", create_using=nx.DiGraph, nodetype=str)
            for u,v,d in G.edges(data=True):
                d['weight']=int(d['weight'])
        case 3:
            try:   #Vuelves al archivo que estabas moficiando leyendolo
                G=nx.read_weighted_edgelist("archivo_cero.txt", delimiter=";", create_using=nx.DiGraph, nodetype=str)
                if G.number_of_edges()==0:
                    print("Aun no existe un grafo")
                    continue
                else:
                    for u,v,d in G.edges(data=True):
                        d['weight']=int(d['weight'])
            except FileNotFoundError:
                print("Crea primero un grafo")
                continue
        case 4: #Cierra
            print("Hasta luego")
            break
        case _:
            print("Error")
            continue
#Menu del grafo
    while True:
        opc=int(input("1.Ver grafo\n2.Cambiar peso\n3.Agregar nodo\n4.Nodo con mas conexiones\n5.Que pasa si bloquea?\n6.Ruta mas cortas entre 2 puntos\n7.Validacion de conexion entre dos puntos\n8.Recorrer por DFS\n9.Ruta optima con pesos (Dijkstra)\n10.Menu principal\n"))
        match(opc):
            case 1:
                if G.number_of_edges()==0:
                    print("Grafo vacio")
                else:
                    VerGrafo(G)
            case 2:
                CambiarPeso(G)
                if opc1==1 or opc1==3:
                    nx.write_weighted_edgelist(G,"archivo_cero.txt", delimiter=";") #Reescibe en el archivo
            case 3:
                AgregarNodo(G)
                if opc1==1 or opc==3:
                    nx.write_weighted_edgelist(G,"archivo_cero.txt", delimiter=";") #Reescibe en el archivo
            case 4:
                if G.number_of_nodes()==0:
                    print("El grafo está vacío")
                else:
                    nodo,conexiones=MaxConexiones(G)
                    print("El nodo",nodo,"con",conexiones,"conexiones es el que tiene mas conexiones")
            case 5:
                BloquearNodo(G)
            case 6:
                CaminoCorto(G)
            case 7:
                ValidarSentido(G)
            case 8:
                RecorrerDFS(G)
            case 9:
                Dijkstra(G)
            case 10:
                break
            case _:
                print("Error\n")
                continue