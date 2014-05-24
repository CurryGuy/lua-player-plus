/** LPP Math lib by Nanni */

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

#include "Math.h"
#include "vfpu_ops.h"

#include <pspgu.h> // For 'GU_PI'

float vfpu_vars[4] __attribute__((aligned(64)));

int NextPow2(unsigned int w)
{
    if(!w) return w;
    unsigned int n = 2;
    while(w > n) n <<= 1;
    return n;
}

int NextMul8(unsigned int w)
{
    return ((w + 7) &~0x7);
}

void Sincos(float r, float *s, float *c)
{
	__asm__ volatile (
		"mtv      %2, S002\n"
		"vcst.s   S003, VFPU_2_PI\n"
		"vmul.s   S002, S002, S003\n"
		"vrot.p   C000, S002, [s, c]\n"
		"mfv      %0, S000\n"
		"mfv      %1, S001\n"
	: "=r"(*s), "=r"(*c): "r"(r));
}

int sqrt2(unsigned int n) {
    unsigned int a;
    for(a = 0; n >= (a * 2) + 1; n -= ((a++) << 1)+1);
    return (a);
}

double lanczos(double x, int r)
{
    if(x == 0.0) return 1.0;
    if(x <= -r || x >= r) return 0.0;
    double tmp = x * GU_PI;
    return r * sin(tmp) * sin(tmp / r) / (tmp * tmp);
}

float LPP_cosf(float f1, float f2) {
    register void *ptr __asm ("a0") = vfpu_vars;
   *(float*)ptr = f1;
   *((float*)ptr+1) = f2;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))

	  
	  cgen_asm(vcst_s(2, 5))
      cgen_asm(vmul_s(0, 0, 2))

      cgen_asm(vcos_s(124, 0))
      cgen_asm(vmul_s(124, 124, 1))
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
      : "=r"(ptr) : "r"(ptr) : "memory");
    return vfpu_vars[0];
}

float LPP_sinf(float f1, float f2) {
    register void *ptr __asm ("a0") = vfpu_vars;
   *(float*)ptr = f1;
   *((float*)ptr+1) = f2;
   __asm__ volatile (
      cgen_asm(lv_s(0, 0, R_a0, 0))
      cgen_asm(lv_s(1, 1, R_a0, 0))

	  cgen_asm(vcst_s(2, 5))
      cgen_asm(vmul_s(0, 0, 2))

      cgen_asm(vsin_s(124, 0))
      cgen_asm(vmul_s(124, 124, 1))
      cgen_asm(sv_q(31, 0 * 4, R_a0, 0))
   : "=r"(ptr) : "r"(ptr) : "memory");
   return vfpu_vars[0];
}