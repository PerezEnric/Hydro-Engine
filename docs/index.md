# Introduction

This is a basic Game Engine made by two students of the 3rd year of Design and Development of Videogames in [CITM](https://www.citm.upc.edu/). It is based in Unity Engine so the GameObject sub-system, camera controls are similar and we have a game mode too. One of the features our engine has is a physics engine, implemented using Bullet Library and Opengl.

# Team

## Guillem

<img src="https://github.com/PerezEnric/Hydro-Engine/blob/master/docs/Guillem.jpg?raw=true">

* GameObjects
* Own File
* Serialization
* Resource Manager
* Model Physicalized
* Shoot spheres

[Github Profile](https://github.com/GuillemSanchez)

## Enric

<img src="https://github.com/PerezEnric/Hydro-Engine/blob/master/docs/Enric.jpg?raw=true">

* UI
* Frustum Culling
* Mouse Picking
* Time Manager
* Vehicle
* Constraint

[Github Profile](https://github.com/PerezEnric)

# Core sub-systems

## GameObjects sub-system

We use GameObjects like containers where they have the different components, such as transformation, mesh, textures, etc.

## Own file format

We create our own way to read and load files.

## Serialization

All the information of the scene that is necessarily to be replicated is converted to characters in order to be used in the future to replicate the scene.

## Frustum Culling

In order to save resources, everything's bounding box that is not colliding with the camera planes is not drawn. Also, there is a panel where the diferent features like the near and far plane can be modified as well as the FOV.

## Mouse picking

A Raycast is used in order to select a GameObject from the scene. It intersects with all the GameObjects' bounding boxes so the ray pass through them and calculate all the triangles to pick the nearest one. Once picked, in the inspector is shown the diferent components the GameObject has.

## Time Management

When the _play_ button is pressed another clock is activated so the game is running. In this case, the physics of the gameobjects work. It can be paused and if _stop_ button is pressed, everything returns to the previous state.

## Resource Manager

Only one GameObject is loaded and the others are references.

# Physics sub-system

In order to implement physics in our Game Engine, we used [Bullet Library](https://pybullet.org/wordpress/). We have used it to make the following features:

## Model physicalized

The whole model is physicalized so every GameObject has a collider that has the same size as their bounding box. The camera is surrounded by a sphere that collides with the other GameObjects. Finally, spheres can be shot through the camera.

## Contraint

In the editor, a point to point constraint can be created. In this case, one cube can be anchored to another cube.

## Vehicle

A vehicle is created to test the collision with the environment.

<img src="https://github.com/PerezEnric/Hydro-Engine/blob/master/docs/GifCollision.gif?raw=true">

<img src="https://github.com/PerezEnric/Hydro-Engine/blob/master/docs/GifBalls.gif?raw=true">

# Video

[Link](https://www.youtube.com/watch?v=yh2Jm9JitnQ) to the video

# Links

[Repository](https://github.com/PerezEnric/Hydro-Engine)

[Last Release](https://github.com/PerezEnric/Hydro-Engine/releases/tag/v1.0)

# License

MIT License

Copyright (c) 2019 Enric & Guillem

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
