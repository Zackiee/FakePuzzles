#ifndef _ENEMYDATA_H
#define _ENEMYDATA_H

struct melee {
	int hp = 3;
	char targetX;
	char targetY;
};
struct ranged {
	int hp = 1;
};
#endif