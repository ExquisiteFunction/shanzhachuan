# 山楂串求解器

- 游戏链接：http://www.wesane.com/game/848/

- 游戏规则：

  1. 游戏中有6根竹签，每根竹签最多可以串7个珠子。

  2. 珠子的等级分为1-8共8个等级，每3个或4个相邻的相同等级的珠子可以合成一个更高等级的珠子。3个等级为8的珠子可以合成一串山楂串（并移除），同时左上角的成绩+1。

  3. 玩家每次可以将一根竹签最上面的珠子移动到另一根竹签上，如果最上面有两个相同等级的珠子，则可以同时移动两个。如果竹签上没有珠子，则无法移动。

  4. 每4次移动，竹签上方会提示落下新的珠子，并且会在下一次移动完成后掉落。

  5. 有任何一根竹签上的珠子数超过7且无法合并则游戏结束。

- How to run

  - cpp求解

  ```shell
  cd ./cpp
  g++ ./main.cpp -std=c++11 -O2
  ./a.out
  cd ..
  ```
  - py求解

  ```shell
  cd ./py
  python3 main.py
  ```

  - lua求解

  ```shell
  cd ./lua
  lua test.lua
  ```

- TODO List

  2. 实现在python中调用C++代码

  3. 实现自动OCR和模拟鼠标点击自动打游戏

  4. 使用强化学习或者深度学习

  5. 能够完成图形界面，提供更好的游戏体验

- 性能

  - ThinkPad P51笔记本，CPU Intel Xeon E3-1535M v6@3.1GHz（多线程在3.8G，单线程4.0G）4C8T
  - Loongson 3A5000笔记本，CPU 3A5000@2.0GHz，4C4T

|               | 3A5000 | P51    | 比值  |
| ------------- | ------ | ------ | ----- |
| python3单线程 | 70.45s | 28.98s | 41.1% |
| python3多线程 | 51.18s | 21.8s  | 42.6% |
| lua5.2        | 8.57s  | 4.25s  | 49.6% |
| CPP单线程     | 0.55s  | 0.21s  | 38.2% |
| CPP多线程     | 0.13s  | 0.05s  | 38.5% |

