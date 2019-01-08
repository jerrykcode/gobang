G = g++
Gobang.o : Gobang.cpp
	$G -c Gobang.cpp
SCalculate.o : SCalculate.cpp
	$G -c SCalculate.cpp
ZMap.o : ZMap.cpp
	$G -c ZMap.cpp
Step.o : Step.cpp
	$G -c Step.cpp
DLL = GOBANG_EXPORTS
Gobang_API.o : Gobang_API.cpp
	$G -c -D$(DLL) Gobang_API.cpp

GOBANG.dll : Gobang.o SCalculate.o ZMap.o Step.o Gobang_API.o
	$G -shared -o GOBANG.dll Gobang.o SCalculate.o ZMap.o Step.o Gobang_API.o

