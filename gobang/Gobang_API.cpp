#ifdef GOBANG_EXPORTS
#define GOBANG_API __declspec(dllexport)
#else
#define GOBANG_API __declspec(dllimport)
#endif

#include "Gobang.h"

#ifdef __cplusplus
extern "C" {
#endif

	PGobang pGobang;

	GOBANG_API void init(char chessType) {
		pGobang = new Gobang(chessType);
	}

	GOBANG_API void endGame() {
		delete pGobang;
		pGobang = NULL;
	}

	GOBANG_API bool isValid(int row, int col) {
		return pGobang->isValid(row, col);
	}

	GOBANG_API void human(int row, int col) {
		pGobang->human(row, col);
	}

	GOBANG_API void computer() {
		pGobang->computer();
	}

	GOBANG_API int getComputerRow() {
		return pGobang->getComputerRow();
	}

	GOBANG_API int getComputerCol() {
		return pGobang->getComputerCol();
	}

	GOBANG_API void updateTurnAfterComputer() {
		return pGobang->updateTurnAfterComputer();
	}

	GOBANG_API bool isGameOver(int row, int col) {
		return pGobang->isGameOver(row, col);
	}

	GOBANG_API bool undoStep() {
		return pGobang->undoStep();
	}

	GOBANG_API bool redoStep() {
		return pGobang->redoStep();
	}

	GOBANG_API void getLastSteps(int *steps) {
		pGobang->getLastSteps(steps);
	}

#ifdef __cplusplus 
}
#endif
