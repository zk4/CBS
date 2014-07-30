#pragma  once

enum eComponent
{
    Component_MOVE,
    Component_COCOS,
	Component_SPRITE,
    Component_HP,
    Component_WEAPON,
	Component_TRAIL,
    Component_AI,
    Component_RADAR,
	Component_BOX2D,
	Component_WALLS,

    Entity_,
    Component_COUNT,


};
enum eTelegram
{
    Telegram_NONE,
    Telegram_HURT,
    Telegram_DEAD,
    Telegram_ACTION_HURT,
    Telegram_SET_POS,
	Telegram_SET_ROTATION,
	Telegram_SET_SCALE,
	Telegram_SET_CONTENSIZE,
    Telegram_ARRIVE,
    Telegram_UPDATE,
    Telegram_AI,
    Telegram_DRAW,
	

	Telegram_ADD_WALL,
    Telegram_ADD_TARGET,
    Telegram_SEARCH,
    Telegram_SEARCH_RESULT,
};
