#ifdef GOBANG_EXPORTS
#define GOBANG_API __declspec(dllexport)
#else
#define GOBANG_API __declspec(dllimport)
#endif

#include "Gobang.h"

#ifdef __cplusplus
extern "C" {
#endif

	Gobang gobang;

	GOBANG_API void init(char chessType) {
		gobang = *(new Gobang(chessType));
	}

	GOBANG_API bool isValid(int row, int col) {
		return gobang.isValid(row, col);
	}

	GOBANG_API void human(int row, int col) {
		gobang.human(row, col);
	}

	GOBANG_API void computer() {
		gobang.computer();
	}

	GOBANG_API int getComputerRow() {
		return gobang.getComputerRow();
	}

	GOBANG_API int getComputerCol() {
		return gobang.getComputerCol();
	}

	GOBANG_API void updateTurnAfterComputer() {
		return gobang.updateTurnAfterComputer();
	}

	GOBANG_API bool isGameOver(int row, int col) {
		return gobang.isGameOver(row, col);
	}

#ifdef __cplusplus 
}
#endif
