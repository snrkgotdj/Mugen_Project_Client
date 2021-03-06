#pragma once

#define SERVER_IP "127.0.0.1"
//#define SERVER_IP "172.30.1.37"

#define PORT 2018

enum PACKET_HEADER
{
	PH_JOIN,
	PH_LOGIN,
	PH_KEY_INPUT,
	PH_KEY_OUTPUT,
	PH_POSITION,
	PH_CHARACTORSEL,
	PH_GAMESTART,
	PH_HP,
	PH_HP_EX,
	PH_STATE,
	PH_POS,
	PH_GUARD_HP,
	PH_END
};

struct PACKET_HP
{
	bool bPlayer;
	float fDamage;
};

struct PACKET_STATE
{
	bool bPlayer;
	UINT eState;
};

