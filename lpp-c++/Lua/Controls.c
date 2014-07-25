/*----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#------  This File is Part Of : ----------------------------------------------------------------------------------------#
#------- _  -------------------  ______   _   --------------------------------------------------------------------------#
#------ | | ------------------- (_____ \ | |  --------------------------------------------------------------------------#
#------ | | ---  _   _   ____    _____) )| |  ____  _   _   ____   ____   ----------------------------------------------#
#------ | | --- | | | | / _  |  |  ____/ | | / _  || | | | / _  ) / ___)  ----------------------------------------------#
#------ | |_____| |_| |( ( | |  | |      | |( ( | || |_| |( (/ / | |  --------------------------------------------------#
#------ |_______)\____| \_||_|  |_|      |_| \_||_| \__  | \____)|_|  --------------------------------------------------#
#------------------------------------------------- (____/  -------------------------------------------------------------#
#------------------------   ______   _   -------------------------------------------------------------------------------#
#------------------------  (_____ \ | |  -------------------------------------------------------------------------------#
#------------------------   _____) )| | _   _   ___   ------------------------------------------------------------------#
#------------------------  |  ____/ | || | | | /___)  ------------------------------------------------------------------#
#------------------------  | |      | || |_| ||___ |  ------------------------------------------------------------------#
#------------------------  |_|      |_| \____|(___/   ------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Licensed under the GPL License --------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Copyright (c) Nanni <cusunanni@hotmail.it> --------------------------------------------------------------------------#
#- Copyright (c) Rinnegatamante <rinnegatamante@eternalongju2.com> -----------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Official Forum : http://rinnegatamante.eu/luaplayerplus/forum.php ---------------------------------------------------#
#- For help using LuaPlayerPlus, coding help, and other please visit : http://rinnegatamante.eu/luaplayerplus/forum.php #
#-----------------------------------------------------------------------------------------------------------------------#
#- Credits : -----------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Homemister for LPHM sourcecode --------------------------------------------------------------------------------------#
#- Zack & Shine for LP Euphoria sourcecode -----------------------------------------------------------------------------#
#- ab5000 for support on psp-ita.com -----------------------------------------------------------------------------------#
#- valantin for sceIoMvdir and sceIoCpdir improved functions------------------------------------------------------------#
#- Dark_AleX for usbdevice ---------------------------------------------------------------------------------------------#
#- VirtuosFlame & ColdBird for iso drivers and kuBridge ----------------------------------------------------------------#
#- sakya for Media Engine and OslibMod ---------------------------------------------------------------------------------#
#- Booster & silverspring for EEPROM write/read functions --------------------------------------------------------------#
#- Akind for RemoteJoyLite ---------------------------------------------------------------------------------------------#
#- cooleyes for mpeg4 lib ----------------------------------------------------------------------------------------------#
#- Arshia001 for PSPAALib ----------------------------------------------------------------------------------------------#
#- InsertWittyName & MK2k for PGE sourcecode ---------------------------------------------------------------------------#
#- Youresam for LUA BMPLib ---------------------------------------------------------------------------------------------#
#- Raphael for vram manager code ---------------------------------------------------------------------------------------#
#- Dynodzzo for LSD concepts -------------------------------------------------------------------------------------------#
#- ab_portugal for Image.negative function -----------------------------------------------------------------------------#
#- JiC� for drawCircle function ----------------------------------------------------------------------------------------#
#- Rapper_skull & DarkGiovy for testing LuaPlayer Plus and coming up with some neat ideas for it. ----------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------*/

#include "../LPP.h"

typedef SceCtrlData Controls;

UserdataStubs(Controls, Controls);

static int luaControls__tostring(lua_State *L)
{
    char buff[32];
    sprintf(buff, "%d", toControls(L, -1)->Buttons);
    lua_pushfstring(L, "Controls (%s)", buff);
    return 1;
}

static int luaControls_read(lua_State*L)
{
	if(lua_gettop(L))
        return luaL_error(L, "Argument error: Controls.read() cannot be called from an instance.");
	Controls* pad = pushControls(L);
	sceCtrlReadBufferPositive(pad, 1);
	return 1;
}

static int luaControls_readPeek(lua_State*L)
{
	if(lua_gettop(L))
        return luaL_error(L, "Argument error: Controls.readPeek() cannot be called from an instance.");
	Controls* pad = pushControls(L);
	sceCtrlPeekBufferPositive(pad, 1);
	return 1;
}

const char* g_errorMessage = "Argument error: The Controls functions take no arguments (and also, must be called with a colon from an instance: e g mycontrols:left().";

#define CHECK_CTRL(NAME, BIT) \
	static int NAME(lua_State *L) \
{ \
	int argc = lua_gettop(L); \
	if (argc > 1) return luaL_error(L, g_errorMessage); \
	if(argc == 0) { \
	lua_pushinteger(L, BIT); \
	return(1); \
	} \
	Controls *a = toControls(L, 1);\
	lua_pushboolean(L, (a->Buttons & BIT) == BIT); \
	return(1); \
}

    CHECK_CTRL(luaControls_select, PSP_CTRL_SELECT)
    CHECK_CTRL(luaControls_start, PSP_CTRL_START)
    CHECK_CTRL(luaControls_up, PSP_CTRL_UP)
    CHECK_CTRL(luaControls_right, PSP_CTRL_RIGHT)
    CHECK_CTRL(luaControls_down, PSP_CTRL_DOWN)
    CHECK_CTRL(luaControls_left, PSP_CTRL_LEFT)
    CHECK_CTRL(luaControls_l, PSP_CTRL_LTRIGGER)
    CHECK_CTRL(luaControls_r, PSP_CTRL_RTRIGGER)
    CHECK_CTRL(luaControls_triangle, PSP_CTRL_TRIANGLE)
    CHECK_CTRL(luaControls_circle, PSP_CTRL_CIRCLE)
    CHECK_CTRL(luaControls_cross, PSP_CTRL_CROSS)
    CHECK_CTRL(luaControls_square, PSP_CTRL_SQUARE)
    CHECK_CTRL(luaControls_home, PSP_CTRL_HOME)
    CHECK_CTRL(luaControls_hold, PSP_CTRL_HOLD)
    CHECK_CTRL(luaControls_note, PSP_CTRL_NOTE)

static int luaControls_buttons(lua_State *L)
{
	if (lua_gettop(L) != 1) return luaL_error(L, g_errorMessage);
	Controls *a = toControls(L, 1);
	lua_pushnumber(L, a->Buttons);
	return 1;
}

static int luaControls_analogX(lua_State *L)
{
	if (lua_gettop(L) != 1) return luaL_error(L, "Argument error: The Controls functions take no arguments.");
	Controls *a = toControls(L, 1);
	lua_pushnumber(L, a->Lx -128);
	return 1;
}

static int luaControls_analogY(lua_State *L)
{
	if (lua_gettop(L) != 1) return luaL_error(L, "Argument error: The Controls functions take no arguments.");
	Controls *a = toControls(L, 1);
	lua_pushnumber(L, a->Ly -128);
	return 1;
}

static int luaControls__eq(lua_State *L) {
	Controls* a = toControls(L, 1);
	Controls* b = toControls(L, 2);
	lua_pushboolean(L, a->Buttons == b->Buttons );
	return 1;
}

static const luaL_reg luaControls_methods[] = {
	{"read",      luaControls_read},
	{"readPeek",  luaControls_readPeek},
	{"select",    luaControls_select},
	{"start",     luaControls_start},
	{"up",        luaControls_up},
	{"right",     luaControls_right},
	{"down",      luaControls_down},
	{"left",      luaControls_left},
	{"l",         luaControls_l},
	{"r",         luaControls_r},
	{"triangle",  luaControls_triangle},
	{"circle",    luaControls_circle},
	{"cross",     luaControls_cross},
	{"square",    luaControls_square},
	{"home",      luaControls_home},
	{"hold",      luaControls_hold},
	{"note",      luaControls_note},
	{"analogX",   luaControls_analogX},
	{"analogY",   luaControls_analogY},
	{"buttons",   luaControls_buttons},
	{0, 0}
};

static const luaL_reg luaControls_meta[] = {
	{"__tostring", luaControls__tostring},
	{"__eq", luaControls__eq},
	{0, 0}
};

UserdataRegister(Controls, luaControls_methods, luaControls_meta)

void luaControls_init(lua_State *L) {
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	Controls_register(L);
}

