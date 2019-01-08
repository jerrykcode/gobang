# Gobang

五子棋AI，极大值极小值搜索

主要算法在gobang中，编译成dll，在wpf界面部分gobang_gui中调用。

在gobang目录下可用```make GOBANG.dll```生成dll文件

### gobang
gobang主要类：

* Gobang 包括主要算法，在Gobang.cpp中Gobang::getBestResult计算落子位置，使用极大值极小值搜索，alpha-beta剪枝，并用zobrist缓存记录已计算过的状态。

* ZMap 封装std::map实现zobrist缓存。

* SCalculate 计算分数类，提供函数判断游戏是否结束，计算一个坐标的分数，及整个棋盘的分数。

* Step 存储落子位置，用于撤销与重做。

Gobang类的函数通过Gobang_API.cpp导出到dll。

### gobang_gui

wpf界面，调用GOBANG.dll。

