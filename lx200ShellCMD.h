/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _lx200ShellCMD_H_
#define _lx200ShellCMD_H_
#include "libnova/ln_types.h"

/*GLOBALs*/
extern char* mount_mode;
extern float targetRA,targetDEC;
extern float telescopeRA,telescopeDEC;
extern struct ln_lnlat_posn siteLatLon;

int f_scansexa (const char *str0,float *dp);
int fs_sexa (char *out, float a, int w, int fracbase,int separator);

void cmd_mount_mode(BaseSequentialStream *chp, int argc, char *argv);
void cmd_dummy(BaseSequentialStream *chp, int argc, char *argv);
void cmd_info(BaseSequentialStream *chp, int argc, char *argv);
void cmd_systime(BaseSequentialStream *chp, int argc, char *argv);
void cmd_date(BaseSequentialStream *chp, int argc, char *argv);
void cmd_libnova_test (BaseSequentialStream *chp, int argc, char *argv);
void cmd_GetCalendar(BaseSequentialStream *chp, int argc, char *argv);
void cmd_mem(BaseSequentialStream *chp, int argc, char *argv) ;
void cmd_threads(BaseSequentialStream *chp, int argc, char *argv);

void ln_now(struct ln_date *date);
void cmd_date(BaseSequentialStream *chp, int argc, char *argv);

/*####################LX200#############################*/
void cmd_firware_date(BaseSequentialStream *chp, int argc, char *argv);
void cmd_firware_time(BaseSequentialStream *chp, int argc, char *argv);
void cmd_firware_number(BaseSequentialStream *chp, int argc, char *argv);
void cmd_firware_product(BaseSequentialStream *chp, int argc, char *argv);
void cmd_firware_ver(BaseSequentialStream *chp, int argc, char *argv);

void cmd_getSite1Name(BaseSequentialStream *chp, int argc, char *argv);

void cmd_setLocalTime(BaseSequentialStream *chp, int argc, char *argv);
void cmd_setLocalDate(BaseSequentialStream *chp, int argc, char *argv);
void cmd_getLocalTime(BaseSequentialStream *chp, int argc, char *argv);
void cmd_getLocalDate(BaseSequentialStream *chp, int argc, char *argv);
void cmd_getClockFormat(BaseSequentialStream *chp, int argc, char *argv);

void cmd_setSiteLongitude(BaseSequentialStream *chp, int argc, char *argv);
void cmd_getSiteLongitude(BaseSequentialStream *chp, int argc, char *argv);
void cmd_setSiteLatitude(BaseSequentialStream *chp, int argc, char *argv);
void cmd_getSiteLatitude(BaseSequentialStream *chp, int argc, char *argv);

void cmd_setTargetRA(BaseSequentialStream *chp, int argc, char *argv);
void cmd_setTargetDEC(BaseSequentialStream *chp, int argc, char *argv);
void cmd_getTargetRA(BaseSequentialStream *chp, int argc, char *argv);
void cmd_getTargetDEC(BaseSequentialStream *chp, int argc, char *argv);

void cmd_getTelescopeRA(BaseSequentialStream *chp, int argc, char *argv);
void cmd_getTelescopeDEC(BaseSequentialStream *chp, int argc, char *argv);

void cmd_slew(BaseSequentialStream *chp, int argc, char *argv);
void cmd_abortMotion(BaseSequentialStream *chp, int argc, char *argv);

static const lx200ShellConfig shell_cfg1;
static char ACK[1] = { (char) 0x06 };

static const lx200ShellCommand LX200commands[] = {

  {"info", cmd_info},
  {"systime", cmd_systime},
  {"date", cmd_date},
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"novatest",cmd_libnova_test},
  {":FirmWareDate",cmd_firware_date},
  {":GVD",cmd_firware_date}, 
  {":GVN",cmd_firware_number}, 
  {":GVP",cmd_firware_product},
  {":GVT",cmd_firware_time},  
  {":GVF",cmd_firware_ver}, 
  {":GM",cmd_getSite1Name}, 
  {ACK,cmd_mount_mode}, 
  {":Sa",cmd_dummy}, 
  {":Sz",cmd_dummy}, 
  {":Sr",cmd_setTargetRA}, 
  {":Sd",cmd_setTargetDEC}, 
  {":Sg",cmd_setSiteLongitude}, 
  {":St",cmd_setSiteLatitude}, 
  {":SL",cmd_setLocalTime}, 
  {":SC",cmd_setLocalDate}, 
  {":SG",cmd_dummy}, 
  {":GA",cmd_dummy}, 
  {":GZ",cmd_dummy}, 
  {":GR",cmd_getTelescopeRA}, 
  {":GD",cmd_getTelescopeDEC}, 
  {":Gr",cmd_getTargetRA}, 
  {":Gd",cmd_getTargetDEC}, 
  {":Gg",cmd_getSiteLongitude}, 
  {":Gt",cmd_getSiteLatitude}, 
  {":Gc",cmd_getClockFormat},
  {":GL",cmd_getLocalTime}, 
  {":GC",cmd_getLocalDate},
  {":GG",cmd_dummy},  
  {":MA",cmd_dummy},
  {":Me",cmd_dummy},  
  {":Mn",cmd_dummy}, 
  {":Ms",cmd_dummy}, 
  {":Mw",cmd_dummy}, 
  {":MS",cmd_slew}, 
  {":Q",cmd_abortMotion}, 
  {":CM",cmd_dummy}, 
  {NULL, NULL}
};

#endif /* _lx200Shell_H_ */



