CC = g++
CFLAGS = -I.
DEPS = Board.h Gobang.h SCalculate.h Situation.h Step.h ZMap.h
OBJ = Gobang.o Gobang_API.o SCalculate.o ZMap.o Step.o
DLL = GOBANG_EXPORTS

%.o : %.cpp $(DEPS)
	$(CC) -c -D$(DLL) -o $@ $< $(CFLAGS)

GOBANG.dll : $(OBJ)
	$(CC) -shared -o $@ $^ $(CFLAGS)
