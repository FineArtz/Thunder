

# 使用指南

# 你需要完成的函数
## initialize
```c++
void initialize();
```
    
## work
```c++
int	work( bool& stop );
```
    主循环中每次运行都将被调用一次
    将stop赋值为true将会结束主循环
    程序的返回值为本函数最后一次的返回值

## mousePress
```c++
void mousePress();
```

## mouseMove
```c++
void mouseMove();
```

##  mouseRelease
```c++
void mouseRelease();
```

## keyDown
```c++
void keyDown();
```

## keyUp
```c++
void keyUp();
```

## finale
```c++
void finale();
```
    进程结束时调用

# 数据类型
## Image
    保存图片的类型
    以指针类型保存，最后需要使用cleanup()函数释放（不要用delete）
    


## Color
    保存颜色的类型
    
> 成员

    * r
        红色的深度(0~255)
    * g
        绿色的深度(0~255)
    * b
        蓝色的深度(0~255)
    * a
        不透明度(0~255)

## Rect
    矩阵类型
    
> 成员

    * x (int)
        横坐标
    * y (int)
        纵坐标
    * w (int)
        矩阵宽度
    * h (int)
        矩阵

## Point
    保存一个点的类型

> 成员

    * x (int)
        横坐标
    * y (int)
        纵坐标

## FlipType
    图片绘制时是否需要翻转（含义见函数drawImage）


> 取值

    * SDL_FLIP_NONE
        不翻转
    * SDL_FLIP_HORIZONTAL
        水平翻转
    * SDL_FLIP_VERTICAL
        上下翻转

# 变量

## FPS_DISPLAY
## nowFPS
## duration
## mousePressed
## mouseDragged
## mouseButton

## mouseX
## mouseY
## pmouseX
## pmouseY
## keyPressed
## keyValue

## SCREEN_WIDTH
    （需要在main.cpp中定义）
## SCREEN_HEIGHT
    （需要在main.cpp中定义）



# 函数

## textToImage

## loadImage

## drawImage

## getImageSize

## setPenColor

## setPenColor

## drawPoint

## drawLine

## drawLines

## drawText

## setCanvas

 





