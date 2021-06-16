#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef struct film
{
    char name[100];
    int actorNumber;
    char **actor;
} film;

typedef struct collection
{
    int filmNumber;
    struct film *filmArray;
} Collection;

typedef struct nod
{
    char *name;
    int hash, degree;
    struct nod *next;
} Nod;

typedef struct graph
{
    int n;
    Nod **actor;
} Graph;

Graph graph;
char actor1[100];
char actor2[100];
char pairs[173][100];
int indx;

typedef struct queue
{
    Nod *node;
    struct queue *next;
} queue;

queue *front, *rear;

// common queue functions

void initQ()
{
    front = NULL;
    rear = NULL;
}

int isEmpty()
{
    return front == NULL;
}

void enqueue(Nod *node)
{
    queue *tmp = (queue *)malloc(sizeof(queue));
    tmp->next = NULL;
    tmp->node = node;
    if (front == NULL && rear == NULL)
    {
        front = tmp;
        rear = tmp;
    }
    rear->next = tmp;
    rear = tmp;
}

Nod *dequeue()
{
    queue *tmp = front;
    Nod *node;
    if (tmp == NULL)
    {
        return NULL;
    }
    node = tmp->node;
    if (front == rear)
    {
        front = NULL;
        rear = NULL;
    }
    else
    {
        front = front->next;
    }
    free(tmp);
    return node;
}

// frees input read data
void freeCollection(Collection *c)
{
    int i, j;
    for (i = 0; i < c->filmNumber; i++)
    {
        for (j = 0; j < c->filmArray[i].actorNumber; j++)
        {
            free(c->filmArray[i].actor[j]);
        }
        free(c->filmArray[i].actor);
    }

    free(c->filmArray);
    free(c);
}

// frees graph
void freeGraph()
{
    int i;
    Nod *tmp, *garb;
    for (i = 0; i < graph.n; i++)
    {
        tmp = graph.actor[i];
        while (tmp != NULL)
        {
            garb = tmp;
            tmp = tmp->next;
            free(garb->name);
            free(garb);
        }
    }
    free(graph.actor);
}

// reads data from input file
Collection *readData(char *input, int pb2)
{
    FILE *f = fopen(input, "r");
    Collection *c = (Collection *)malloc(sizeof(Collection));
    int filmNumber, i, j;
    char name[100];

    // reads number of films
    fscanf(f, "%d", &filmNumber);
    fgetc(f);

    film *filmArray = (film *)malloc(filmNumber * sizeof(film));

    // reads data from input file
    for (i = 0; i < filmNumber; i++)
    {
        // reads name of current film
        fgets(filmArray[i].name, 100, f);
        filmArray[i].name[strlen(filmArray[i].name) - 1] = '\0';

        // reads number of actors in current film
        fscanf(f, "%d", &filmArray[i].actorNumber);
        fgetc(f);

        //allocates actorNumber places in actor array
        filmArray[i].actor = (char **)malloc(filmArray[i].actorNumber * sizeof(char *));

        for (j = 0; j < filmArray[i].actorNumber; j++)
        {
            // reads name of current actor
            fgets(name, 100, f);
            name[strlen(name) - 1] = '\0';

            // allocates memory for the name in actor[j]
            filmArray[i].actor[j] = (char *)malloc((strlen(name) + 1) * sizeof(char));

            // adds name to the array
            strcpy(filmArray[i].actor[j], name);
        }
    }

    if (pb2 == 1)
    {
        fgets(actor1, 50, f);
        fgets(actor2, 50, f);
        actor1[strlen(actor1) - 1] = '\0';
        actor2[strlen(actor2) - 1] = '\0';
    }

    fclose(f);
    c->filmArray = filmArray;
    c->filmNumber = filmNumber;
    return c;
}

// prints data read from input file
void printData(int filmNumber, film *filmArray)
{
    int i, j;
    for (i = 0; i < filmNumber; i++)
    {
        printf("Film %d: %s\n", i + 1, filmArray[i].name);
        for (j = 0; j < filmArray[i].actorNumber; j++)
        {
            printf("\tActor %d: %s\n", j + 1, filmArray[i].actor[j]);
        }
    }
}

// prints whole graph
void printGraphNodes(char *output)
{
    int i;
    Nod *tmp;
    FILE *f = fopen(output, "w");
    for (i = 0; i < graph.n; i++)
    {
        fprintf(f, "%s (HASH: %d)\n", graph.actor[i]->name, graph.actor[i]->hash);
        tmp = graph.actor[i];
        while (tmp)
        {
            if (tmp != graph.actor[i])
            {
                fprintf(f, "\t%s (HASH: %d)\n", tmp->name, tmp->hash);
            }
            tmp = tmp->next;
        }
    }
    fclose(f);
}

// checks if node already exists in graph
int alreadyInGraphList(char *name)
{
    int i;
    // printf("Compares name: \"%s\"\n", name);
    for (i = 0; i < graph.n; i++)
    {

        if (strcmp(graph.actor[i]->name, name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// adds actor to graph
void addNode(int filmNumber, film *filmArray)
{
    int i, j;
    for (i = 0; i < filmNumber; i++)
    {
        //printf("Film %d: %s\n", i + 1, filmArray[i].name);

        for (j = 0; j < filmArray[i].actorNumber; j++)
        {

            if (alreadyInGraphList(filmArray[i].actor[j]))
            {
                //printf("\t%s is already in graph\n\n", filmArray[i].actor[j]);
            }
            else
            {
                graph.n++;

                // allocs
                graph.actor = (Nod **)realloc(graph.actor, graph.n * sizeof(Nod *));
                graph.actor[graph.n - 1] = (Nod *)malloc(sizeof(Nod));
                graph.actor[graph.n - 1]->name = (char *)malloc(strlen(filmArray[i].actor[j]) + 1);

                graph.actor[graph.n - 1]->next = NULL;
                graph.actor[graph.n - 1]->hash = graph.n - 1;
                graph.actor[graph.n - 1]->degree = -1;
                strcpy(graph.actor[graph.n - 1]->name, filmArray[i].actor[j]);

                //printf("%s added to graph\n\n", graph.actor[graph.n - 1]->name);
            }
        }
    }
}

// checks if 'name' stars in 'currentFilm'
int isInFilm(char *name, film currentFilm)
{
    int i;
    for (i = 0; i < currentFilm.actorNumber; i++)
    {
        if (strcmp(name, currentFilm.actor[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}

// checks if 'name' belongs to 'adjList'
int alreadyInList(Nod *adjList, char *name)
{
    Nod *tmp = adjList;
    while (tmp)
    {
        if (strcmp(tmp->name, name) == 0)
        {
            return 1;
        }
        tmp = tmp->next;
    }
    return 0;
}

// retreives hash from nodes list
int hashRetreiver(char *name)
{
    int i;
    for (i = 0; i < graph.n; i++)
    {
        if (strcmp(name, graph.actor[i]->name) == 0)
        {
            return graph.actor[i]->hash;
        }
    }
    return -1;
}

// adds whole 'currentFilm' cast to 'adjList'
void addFilmCastToList(Nod *adjList, film currentFilm)
{
    int i;
    for (i = 0; i < currentFilm.actorNumber; i++)
    {
        if (!alreadyInList(adjList, currentFilm.actor[i]))
        {
            Nod *tmp = (Nod *)malloc(sizeof(Nod));
            tmp->name = (char *)malloc(strlen(currentFilm.actor[i]) + 1);

            strcpy(tmp->name, currentFilm.actor[i]);
            tmp->hash = hashRetreiver(tmp->name);
            tmp->degree = 0;
            tmp->next = adjList->next;

            if (tmp->hash == -1)
            {
                printf("PROBLEM WITH HASH!");
            }

            adjList->next = tmp;
        }
    }
}

// builds every actor's adjList
void adjList(Collection *c)
{
    int i, j;

    // go thru every actor
    for (i = 0; i < graph.n; i++)
    {
        // look for actor in every film
        for (j = 0; j < c->filmNumber; j++)
        {
            if (isInFilm(graph.actor[i]->name, c->filmArray[j]))
            {
                addFilmCastToList(graph.actor[i], c->filmArray[j]);
            }
        }
    }
}

// spares me of copy-pasting same shit three times
void BUILD_GRAPH(char *input, int pb2)
{
    Collection *c = readData(input, pb2);
    graph.n = 0;
    graph.actor = (Nod **)malloc(0);
    addNode(c->filmNumber, c->filmArray);
    adjList(c);
    freeCollection(c);
}

//  -------------------------
// | FUNCTIONS FOR PROBLEM 1 |
//  -------------------------

// locates first unvisited node
int locateUnvisitedNode(int *visited)
{
    int i;
    for (i = 0; i < graph.n; i++)
    {
        if (visited[i] == 0)
        {
            return i;
        }
    }
    return -1;
}

// bfs
int bfsForest()
{
    Nod *tmp, *currentNode, *maxNode;
    int *visited = (int *)calloc((graph.n + 1), sizeof(int));
    int unvisitedHash, prodMembers = 0, max_prodMembers = 0;
    initQ();

    // go thru all productions
    while (1)
    {
        prodMembers = 0;
        // find first unvisited node
        unvisitedHash = locateUnvisitedNode(visited);

        // if no unvisited node left, exit algorithm
        if (unvisitedHash == -1)
        {
            free(visited);
            return maxNode->hash;
        }

        // keep hold of head of forest
        currentNode = graph.actor[unvisitedHash];

        enqueue(graph.actor[unvisitedHash]);

        // go thru one production
        while (!isEmpty())
        {
            tmp = dequeue();

            // if parent hasn't been visited
            if (!visited[tmp->hash])
            {
                prodMembers++;
                // mark parent as visited
                visited[tmp->hash] = 1;

                // traverse adj list
                tmp = tmp->next;
                while (tmp)
                {
                    enqueue(graph.actor[tmp->hash]);
                    tmp = tmp->next;
                }
            }
        }

        // if current tree's degree is bigger than the max tree degree
        printf("%d\n", prodMembers);
        if (prodMembers > max_prodMembers)
        {
            max_prodMembers = prodMembers;
            maxNode = currentNode;
        }
    }
}

// swaps to strings in an array
void swap(char *a, char *b)
{
    char *c = (char *)malloc(strlen(a) + 1);
    strcpy(c, a);
    strcpy(a, b);
    strcpy(b, c);
    free(c);
}

// finds biggest production, orders it and prints it
void printLargestProduction(int hash, char *output)
{
    Nod *tmp;
    int *visited = (int *)calloc((graph.n + 1), sizeof(int));
    int number = 0, i, j;
    char array[3300][100];

    for (i = 0; i < 3299; i++)
    {
        array[i][0] = '\0';
    }
    i = 0;

    initQ();
    enqueue(graph.actor[hash]);

    while (!isEmpty())
    {
        tmp = dequeue();

        // if parent hasn't been visited
        if (!visited[tmp->hash])
        {
            number++;

            // mark parent as visited
            visited[tmp->hash] = 1;
            strcpy(array[i++], tmp->name);
            //printf("%s\n", array[i - 1]);

            // traverse adj list
            tmp = tmp->next;
            while (tmp)
            {
                enqueue(graph.actor[tmp->hash]);
                tmp = tmp->next;
            }
        }
    }
    free(visited);

    // lexicographical order
    for (i = 0; i < number - 1; i++)
    {
        for (j = i + 1; j < number; j++)
        {
            if (strcmp(array[i], array[j]) > 0)
            {
                swap(array[i], array[j]);
            }
        }
    }

    // print's
    FILE *f = fopen(output, "w");

    fprintf(f, "%d\n", number);
    for (i = 0; i < number; i++)
    {
        fprintf(f, "%s\n", array[i]);
    }
    fclose(f);
}

//  -------------------------
// | FUNCTIONS FOR PROBLEM 2 |
//  -------------------------

int bfs(int hash1, int hash2, char *actor1, char *actor2)
{
    Nod *tmp;
    int *visited = (int *)calloc((graph.n + 1), sizeof(int));
    int parent_degree, degree, found = 0;

    initQ();
    graph.actor[hash1]->degree = 0;
    enqueue(graph.actor[hash1]);

    while (!isEmpty())
    {
        tmp = dequeue();
        parent_degree = tmp->degree;

        // if parent hasn't been visited
        if (!visited[tmp->hash])
        {

            // mark parent as visited
            visited[tmp->hash] = 1;

            // traverse adj list
            tmp = tmp->next;
            while (tmp)
            {
                if (graph.actor[tmp->hash]->degree == -1)
                {
                    graph.actor[tmp->hash]->degree = parent_degree + 1;
                }

                if (strcmp(tmp->name, actor2) == 0)
                {
                    // FOUND ACTOR2! RETURN
                    found = 1;
                    degree = parent_degree + 1;
                    goto free;
                }

                enqueue(graph.actor[tmp->hash]);
                tmp = tmp->next;
            }
        }
    }
free:
    // free's
    free(visited);
    while (isEmpty() == 0)
    {
        dequeue();
    }

    return found == 1 ? degree : -1;
}

//  -------------------------
// | FUNCTIONS FOR PROBLEM 3 |
//  -------------------------

void dfsB(int v, int timp, int *idx, int *low, int *pi)
{
    idx[v] = timp;
    low[v] = timp;
    timp++;
    int u;
    Nod *tmp = graph.actor[v];

    while (tmp)
    {
        u = tmp->hash;
        if (u != pi[v])
        {
            if (idx[u] == -1)
            {
                pi[u] = v;
                dfsB(u, timp, idx, low, pi);
                low[v] = low[v] < low[u] ? low[v] : low[u];
                if (low[u] > idx[v])
                {
                    strcpy(pairs[indx++], graph.actor[v]->name);
                    strcpy(pairs[indx++], tmp->name);
                    //printf("PUNTE intre %s si %s!\n", graph.actor[v]->name, tmp->name);
                }
            }
            else
            {
                low[v] = low[v] < idx[u] ? low[v] : idx[u];
            }
        }
        tmp = tmp->next;
    }
}

void punti(char *output)
{
    int timp = 0, i;
    indx = 0;
    int *idx = (int *)malloc(graph.n * sizeof(int));
    int *low = (int *)malloc(graph.n * sizeof(int));
    int *pi = (int *)malloc(graph.n * sizeof(int));

    for (i = 0; i < 173; i++)
    {
        pairs[i][0] = '\0';
    }

    for (i = 0; i < graph.n; i++)
    {
        idx[graph.actor[i]->hash] = -1;
        low[graph.actor[i]->hash] = INT_MAX;
        pi[graph.actor[i]->hash] = 0;
    }

    for (i = 0; i < graph.n; i++)
    {
        if (idx[graph.actor[i]->hash] == -1)
        {
            dfsB(graph.actor[i]->hash, timp, idx, low, pi);
        }
    }
    free(idx);
    free(low);
    free(pi);

    int j;
    // lexicographical order

    for (i = 0; i < indx - 1; i = i + 2)
    {
        if (strcmp(pairs[i], pairs[i + 1]) > 0)
        {
            swap(pairs[i], pairs[i + 1]);
        }
    }
    for (i = 0; i < indx - 2; i = i + 2)
    {
        for (j = i + 2; j < indx; j = j + 2)
        {
            if (strcmp(pairs[i], pairs[j]) > 0)
            {
                swap(pairs[i], pairs[j]);
                swap(pairs[i + 1], pairs[j + 1]);
            }
        }
    }

    i = 0;
    char name[100];
    name[0] = '\0';
    int count = 0, k, m;
    while (i < indx)
    {
        strcpy(name, pairs[i]);
        j = i;
        k = i;
        count = 0;
        while (strcmp(pairs[j], name) == 0 && i < indx)
        {
            count++;
            j = j + 2;
        }
        m = j;
        for (i = k; i < k + (count - 1) * 2; i = i + 2)
        {
            for (j = i + 2; j <= k + (count - 1) * 2; j = j + 2)
            {
                if (strcmp(pairs[i + 1], pairs[j + 1]) > 0)
                {
                    swap(pairs[i + 1], pairs[j + 1]);
                }
            }
        }
        i = m;
    }

    // print's
    FILE *f = fopen(output, "w");

    fprintf(f, "%d\n", indx / 2);
    for (i = 0; i < indx; i = i + 2)
    {
        fprintf(f, "%s %s\n", pairs[i], pairs[i + 1]);
    }
    fclose(f);
}

// ----------------------------------------------------------------------------

void ProbOne(char *input, char *output)
{
    BUILD_GRAPH(input, 0);
    //printGraphNodes(output);
    int hash = bfsForest();
    //printf("hash: %d\n", hash);
    printLargestProduction(hash, output);

    freeGraph();
}

void ProbTwo(char *input, char *output)
{
    BUILD_GRAPH(input, 1);
    printf("actor1: %s\nactor2: %s\n", actor1, actor2);

    int hash1 = hashRetreiver(actor1);
    int hash2 = hashRetreiver(actor2);
    int degree = bfs(hash1, hash2, actor1, actor2);

    FILE *f = fopen(output, "w");
    fprintf(f, "%d\n", degree);
    fclose(f);
    freeGraph();
}

void ProbThree(char *input, char *output)
{
    BUILD_GRAPH(input, 0);
    punti(output);
    freeGraph();
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Insufficient arguments! (%d)\n", argc);
        return 0;
    }

    // select problem
    {
        if (strcmp(argv[1], "-c1") == 0)
        {
            ProbOne(argv[2], argv[3]);
        }
        if (strcmp(argv[1], "-c2") == 0)
        {
            ProbTwo(argv[2], argv[3]);
        }
        if (strcmp(argv[1], "-c3") == 0)
        {
            ProbThree(argv[2], argv[3]);
        }
    }

    return 0;
}