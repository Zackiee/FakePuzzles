#include "game.h"

boxscan* collider(float a, float b, float c, float d, int boxitself)
{
	boxscan* surround = 0;
	surround = (boxscan*)malloc(sizeof(boxscan*));
	surround->a = (a) ? a : 0.0f;
	surround->b = (b) ? b : 0.0f;
	surround->height = (d) ? d : 1.0f;
	surround->width = (c) ? c : 1.0f;
	return (surround);
}
void collide(boxscan *surround)
{
	if (surround)
	{
		free(surround);
	}
}
int colliding(boxscan *surround,boxscan *enos)
{
	if (surround->a > enos->a + enos->width&&surround->a + surround->width<enos->a&&surround->b>enos->b + enos->height
		&&surround->b + surround->height < enos->b)
	{
		return(0);
	}
	return(1);
}
