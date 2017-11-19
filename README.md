# STG_Game
A simple STG Game for 2017 Programming Lesson.

You can see a completed version which have basis functions in branch master.

## Files
src/main.cpp

src/SDL2_header.cpp

include/SDL2_header.h

include/res_path.h

include/cleanup.h

include/pointd.h

res/...

### You *needn't* care about these files

> src/SDL2_header.cpp
>
> include/res_path.h
>
> include/cleanup.h
>
> include/pointd.h


### Unused Files
> src/main2.cpp

## Modifications
### 10-22

Construct the basis structure.

* Add new Function: `drawImageC`.

* Add new classes: `item` and its derived classes `Plane`, `Enemy` and `Bullet`.

### 10-23(By Assitant Teacher Chen Jingxiao)

You can use muti-file structures now.

A bug about memory-leak has been fixed.

* Rename pointd.hpp -> pointd.h

* Add new file: src/SDL2_header.cpp

* Modify files in *include* folder.

* add duration_i

* fixed main.cpp & main2.cpp memory leak bug

	( in drawHint(), a value of Image* did not call clearup() )

### 10-21

The first commitment, including initial files and two simple demos.