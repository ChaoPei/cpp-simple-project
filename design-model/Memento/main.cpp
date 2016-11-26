#include "Memento.hpp"

int main() {
	
	CareTake caretake;
	
	// 游戏进度
	GameRole role;
	role.show();
	
	// 保存进度
	caretake.save(role.saveProcess());
	
	// 游戏进度更新
	role.attack();
	role.show();

	// 恢复以前的进度
	role.load(role.loadProcess(0));
	role.show();

	return 0;
}