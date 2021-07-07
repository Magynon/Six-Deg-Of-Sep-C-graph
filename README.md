# Ștefan MĂGIRESCU

## MovieGraph

</br>
In designing the graph, I first read the data in a special collection struct that has the following format:
</br></br>

- **collection** _struct_

  - no. of films
  - array of **film** _structs_

- **film** _struct_
  - name of film
  - no. of actors
  - array of actors' names

</br>
I then proceeded with the graph building procedure which remained the same all throughout the 3 tasks. The graph struct is as following:
</br></br>

- **graph** _struct_

  - no. of actors
  - array of pointers to **actor** _adj lists_'s heads (node)

- **actor** _node_
  - name of the actor
  - hash
  - degree of relation
  - pointer to neighbour</br></br>

### **Graph building procedure**

</br>

- First, go through one movie at a time in the collection list ( **addNode**() ), and add each actor to the graph, as long as it hasnt been already added before ( **alreadyInGraphList**() )
- Initialize each actor's hash with their index in the adjacency list
- Go through one actor at a time and build their adjacency list ( **adjList**() ). Add whole film cast where they belong ( **isInFilm**(), **addFilmCastToList**() ), provided each actor in the cast hasn't already been added to the list ( **alreadyInList**() ). Retreive each actor's hash from the nodes list ( **hashRetreiver**() ) and assign it to the actor in the adjacency list.

All this above is encapsulated into **BUILD_GRAPH**() method, as to minimise number of lines of code.</br></br>

### Printing functions

- **printData**() prints data from collection
- **printGraphNodes**() - prints graph

### Memory freeing functions

- **freeCollection**()
- **freeGraph**()

</br>

### General functions:

- **ProbOne** deals with exercise no. 1
- **ProbTwo** deals with exercise no. 2
- **ProbThree** deals with exercise no. 3
- **initQ** allocates memory for root node
- **isEmpty** checks if queue is empty
- **enqueue** adds element to queue
- **dequeue** deletes element from queue

</br>

## **Task no. 1**:

</br>

- Do Breadth First Traversal ( **bfsForest**() ) on graph and mark visited nodes on the way. When the algorithm ends, it's either because the whole graph has been traveresed, or because the graph is made out of several disconnected subgraphs (_remember the number of members and the starting node_)
- If it is the latter, pick a node which hasn't been visited and start the BFT on that node. Do this until no unvisited node is left
- Always compare the last visited production's number of members with the maximum number yet found
- Print the largest production ( **printLargestProduction**() ), by performing BFT starting from the node belonging to the biggest production. Memorize all names in an array and sort it lexicographically, then print it.

  </br>

## **Task no. 2**:

</br>

- Perform BFT ( **bfs**() ) on graph, starting from the first actor provided in the input
- Keep track of the degree of proximity by adding 1 to the parentDegree provided in the struct, for each of its children
- Stop the algorithm when it gets to the second actor provided in the input
- Print the degree.

</br>

## **Task no. 3**:

</br>

- Use Tarjan's algorithm to find the bridges between all the subgraphs.

</br>

_For more on how the functions work, check code comments._
