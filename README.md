# STG_Game
A simple STG Game for 2017 Programming Lesson B, ACM Class, SJTU.

Mainly coded by Zhao Hanye.

## Files

main.cpp

SDL2_header.cpp

item.cpp

draw.cpp

deal.cpp

include/SDL2_header.h

include/res_path.h

include/cleanup.h

include/pointd.h

include/others.h

include/item.h

include/draw.h

include/deal.h

res/...

### You *needn't* care about these files

> SDL2_header.cpp
>
> include/res_path.h
>
> include/cleanup.h
>
> include/pointd.h


### Unused Files
> src/main2.cpp

## Modifications

### 11-22

* Perhaps the final version.

* Prerelease.

### 11-19

* Add bonus tools.

* Repair some bugs.

### 11-09

* Adjust the speed of enemy 2 to avoid unavoidable tracking.

### 11-06

* Repair bugs with the directions of bullets of enemy 0.

* Add a new type of enemy: `eType = 2`, which always move to the player.

### 11-02

* Add a new type of enemy: `eType = 1`.

* Adjust the attack speed of enemy 0.

### 11-01

* Add bursting animation for enemies.

* Change the screen into two columns: the right side is used for showing information.

* Add new function: 'drawGameInfo'.

* Create ending screen.

* Add new function: 'drawEndInfo'.

* Deal with collision between player and enemies.

* Repair bugs with bomb.

### 10-30

* Change the structure of project: real multifile structure constructed!

* Repair bugs with bullets' angle.

* Draw game-over animation.

### 10-28

* Add new function: `dealWithCollision`.

* But only complete bullets with planes part.

* Add comments about classes hierarchy.

### 10-25(By assistant teacher Chen Jingxiao)

Support to multifile.

>You can use muti-file structures now.
>
>A bug about memory-leak has been fixed.
>
>* Rename pointd.hpp -> pointd.h
>
>* Add new file: src/SDL2_header.cpp
>
>* Modify files in *include* folder.
>
>* add duration_i
>
>* fixed main.cpp & main2.cpp memory leak bug
>
>	( in drawHint(), a value of Image* did not call clearup() )

### 10-23

Add a type of enemy planes: `eType = 0`.

### 10-22

Construct the basis structure.

* Add new function: `drawImageC`.

* Add new classes: `item` and its derived classes `Plane`, `Enemy` and `Bullet`.

### 10-21

The first commitment, including initial files and two simple demos privided by assistant teachers.
