## **GraphFacesFinder**
---
This program is able to list all of the faces of a planar graph(with straight edges), which is inputted by the user through its number of vertices, number of edges and its vertices(which must contain 2D coordinates, degree and adjacency list).
---
### **Tools** 
---
- Language: [C++](https://cplusplus.com/doc/)
- Compiler: [GCC](https://gcc.gnu.org/)
---
### How to use it
In order to compile the program, you must use the following command: 

    g++ faces.cpp -o faces
To run the program, use:

    ./faces

- The format of the inputs must be the following:

  N(number of vertices) M(number of edges)
  <p>
  X Y degree adjacentvertex1 adjacentvertex2 adjacentvertex3... (This will be vertex 1)
  <p>
  X Y degree adjacentvertex1 adjacentvertex2 adjacentvertex3... (This will be vertex 2)
  <p>
  X Y degree adjacentvertex1 adjacentvertex2 adjacentvertex3... (This will be vertex 3)
  <p>
  <p>

- The output will have the following format:
  <p>
  Number of Faces
  <p>
  Size Face1
  <p>
  Size Face2
  <p>
  Size Face3
  <p>
  ...
  <p>
  <p>

- Example:
    <p>
![02Fzi15](https://github.com/Lorenzovagliano/GraphFacesFinder/assets/111889654/4232a8af-8f8d-4b99-97b0-b6972161d0d5)


