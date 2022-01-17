# Assignment 0 Graphics Assignment 

## Execution Instructions 

- The given submission has the following directory structure : 

- include 
  - camera.h 
  - shader.h
  
  
- libraries
  - glad
  - glfw
  - glm
  
- src
  - fragment.shader
  - vertex.shader
  - main.cpp
- CMakeLists.txt
  
  To Generate the prism :

    ```cpp
    mkdir build // make a directory 
    cd build 
    cmake ..
    make 
    ./app < number_of_sides >
    ```
    To open an opengl window with the given polynomial rendered .

- This is a graphics pipeline to create and render 3-d objects , with `n` ( the number of sides ) passed as the command line input.
  
## Part A : Procedural Generation of the Prism

1. Created a prism using the parametric coordinates  , with each face having unique color .

## Part B : Bringing the scene to life with motion

1. `Flying Camera` : 
   
   - `W` ,  `A` ,  `S` ,`D` , `E` ,`Q` : for 6- D translation of camera around the object 
  
2. `Object Translation` :
   
   - `2` , `4` , `6`  , `8`  ,  `1`  , `3` : for the object translation 
  
3. `Prism fashion show` :
   
   - `0` : First of the 2 specified coordinates for the prism
   - `9` : Second of the 2 specified coordinates for the prism
   
4. `Prism, let’s go for a spin`
   
   - `R` : A toggle functionality switch . On pressing R , the prism starts rotating about the `Z-Axis`  , on again pressing `R` The prism stops the rotation .
  
  
5. `Oh! How the turntables?`

    - `T`  : Toggle switch .
    -  **Assumption** : The object re-orients itself to the origin , and then the camera starts revolving around the object ( which is re arranged to the origin )
  