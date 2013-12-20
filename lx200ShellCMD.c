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

#include "ch.h"
#include "hal.h"

#include "chprintf.h"
//#include "shell.h"

#include "chrtclib.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "lx200Shell.h"
#include "lx200ShellCMD.h"
#include "libnova/transform.h"
#include <libnova/julian_day.h>
#include <libnova/utility.h>

/*GLOBALs*/
char* mount_mode={"P"};
float targetRA,targetDEC;
float telescopeRA,telescopeDEC;
struct ln_lnlat_posn siteLatLon;


void cmd_dummy(BaseSequentialStream *chp, int argc, char *argv)  {
    (void) argc;
    (void)argv;

    chprintf(chp, "NOT IMPLEMENTED YET\r\n");
    return;
}

void cmd_info(BaseSequentialStream *chp, int argc, char *argv) {

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: info\r\n");
    return;
  }

  chprintf(chp, "Kernel:       %s\r\n", CH_KERNEL_VERSION);
#ifdef CH_COMPILER_NAME
  chprintf(chp, "Compiler:     %s\r\n", CH_COMPILER_NAME);
#endif
  chprintf(chp, "Architecture: %s\r\n", CH_ARCHITECTURE_NAME);
#ifdef CH_CORE_VARIANT_NAME
  chprintf(chp, "Core Variant: %s\r\n", CH_CORE_VARIANT_NAME);
#endif
#ifdef CH_PORT_INFO
  chprintf(chp, "Port Info:    %s\r\n", CH_PORT_INFO);
#endif
#ifdef PLATFORM_NAME
  chprintf(chp, "Platform:     %s\r\n", PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  chprintf(chp, "Board:        %s\r\n", BOARD_NAME);
#endif
#ifdef __DATE__
#ifdef __TIME__
  chprintf(chp, "Build time:   %s%s%s\r\n", __DATE__, " - ", __TIME__);
#endif
#endif
}

void cmd_systime(BaseSequentialStream *chp, int argc, char *argv) {
  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: systime\r\n");
    return;
  }
  chprintf(chp, "%lu\r\n", (unsigned long)chTimeNow());
}

void cmd_date(BaseSequentialStream *chp, int argc, char *argv) {
  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: info\r\n");
    return;
  }
  struct ln_date date;
  ln_now(&date);
  chprintf(chp,"libnova UTC date %u/%u/%u %u:%u:%3.1f\n", date.days,date.months,date.years,date.hours,date.minutes,date.seconds);
}

void ln_now(struct ln_date *date) {
        struct tm timp;
	time_t unix_time;
	/* UT date and time */
  	unix_time = rtcGetTimeUnixSec(&RTCD1);
	if (unix_time == -1){
		return;
	} else{
		rtcGetTimeTm(&RTCD1, &timp);
		date->years = timp.tm_year+1900;
		date->months = timp.tm_mon+1;
		date->days = timp.tm_mday;
		date->hours = timp.tm_hour;
		date->minutes = timp.tm_min;
		date->seconds = timp.tm_sec;
	}
}

void cmd_libnova_test (BaseSequentialStream *chp, int argc, char *argv) {
       (void) argv;
 	 if (argc > 0) {
 	   chprintf(chp, "Usage: novatest\r\n");
 	   return;
 	 }
	struct lnh_equ_posn hobject, hequ;
	struct lnh_lnlat_posn hobserver;
	struct ln_equ_posn object, equ;
	struct ln_hrz_posn hrz;
	struct lnh_hrz_posn hhrz;
	struct ln_lnlat_posn observer;

	double JD;
	struct ln_date date;

	
	/* 
	 * observers position
	 * longitude is measured positively eastwards
	 * Madrid
	 */
	hobserver.lng.degrees = -3;
	hobserver.lng.minutes = 42;
	hobserver.lng.seconds = 36;
	hobserver.lat.degrees = 40;
	hobserver.lat.minutes = 25;
	hobserver.lat.seconds = 11.99;

	/* Alnilam */
	hobject.ra.hours = 5;
	hobject.ra.minutes = 36;
	hobject.ra.seconds = 12.8;
	hobject.dec.neg = 1;
	hobject.dec.degrees = 1;
	hobject.dec.minutes = 12;
	hobject.dec.seconds = 7;

	chprintf(chp,"ANILAM: RA %d:%d:%f  DEC %d:%d:%f\n", hobject.ra.hours, hobject.ra.minutes,
		hobject.ra.seconds, hobject.dec.degrees, hobject.dec.minutes, hobject.dec.seconds);


        ln_now(&date);
        
	JD = ln_get_julian_day (&date);
	ln_hequ_to_equ (&hobject, &object);
	ln_hlnlat_to_lnlat (&hobserver, &observer);



	ln_get_hrz_from_equ (&object, &observer, JD, &hrz);
	chprintf(chp,"(Alnilam) Equ to Horiz ALT %f\n", hrz.alt);
	chprintf(chp,"(Alnilam) Equ to Horiz AZ %f\n", hrz.az);
	ln_hrz_to_hhrz(&hrz, &hhrz);
	chprintf(chp,"ALT %d:%d:%f  AZ %d:%d:%f\n", hhrz.alt.degrees, hhrz.alt.minutes,
		hhrz.alt.seconds, hhrz.az.degrees, hhrz.az.minutes, hhrz.az.seconds);

	ln_get_equ_from_hrz (&hrz, &observer, JD, &equ);
	chprintf(chp,"(Alnilam) Horiz to Equ RA %f\n", equ.ra);
	chprintf(chp,"(Alnilam) Horiz to Equ DEC %f\n", equ.dec);
	ln_equ_to_hequ(&equ, &hequ);
	chprintf(chp,"RA %d:%d:%f  DEC %d:%d:%f\n", hequ.ra.hours, hequ.ra.minutes,
		hequ.ra.seconds, hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);


}



void cmd_mem(BaseSequentialStream *chp, int argc, char *argv) {
  size_t n, size;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreStatus());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

void cmd_threads(BaseSequentialStream *chp, int argc, char *argv) {
  static const char *states[] = {THD_STATE_NAMES};
  Thread *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "    addr    stack prio refs     state time        Name\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%.8lx %.8lx %4lu %4lu %9s %lu        %s\r\n",
            (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
            (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
            states[tp->p_state], (uint32_t)tp->p_time,tp->p_name);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

int f_scansexa (const char *str0,float *dp) {
	float a = 0, b = 0, c = 0;
	char str[128];
	char *neg;
	int r;

	/* copy str0 so we can play with it */
	strncpy (str, str0, sizeof(str)-1);
	str[sizeof(str)-1] = '\0';

	neg = strchr(str, '-');
	if (neg)
	    *neg = ' ';
	//r = sscanf (str, "*[^0-9]%f%*[^0-9]%f%*[^0-9]%f", &a, &b, &c);
	r = sscanf (str, "%f%*[^0-9]%f%*[^0-9]%f", &a, &b, &c);
	if (r < 1) {
		c=0;
  		r = sscanf (str, "%f%*[^0-9]%f", &a, &b); 
        }
	if (r < 1)
	    return (-1);
	*dp = a + b/60 + c/3600;
	if (neg)
	    *dp *= -1;
	return (0);
}

int fs_sexa (char *out, float a, int w, int fracbase,int separator) {
	char *out0 = out;
	unsigned long n;
	int d;
	int f;
	int m;
	int s;
	int isneg;

	/* save whether it's negative but do all the rest with a positive */
	isneg = (a < 0);
	if (isneg)
	    a = -a;

	/* convert to an integral number of whole portions */
	n = (unsigned long)(a * fracbase + 0.5);
	d = n/fracbase;
	f = n%fracbase;

	/* form the whole part; "negative 0" is a special case */
	if (isneg && d == 0)
	    out += sprintf (out, "%*s-0", w-2, "");
	else
	    out += sprintf (out, "%*d", w, isneg ? -d : d);

        /* Separator '*' or ':' */ 
	    if (separator==1) {
		out += sprintf (out, "*");
	    } else {	
	    	out += sprintf (out, ":");
            }
	/* do the rest */
	switch (fracbase) {
	case 60:	/* dd:mm */
	    m = f/(fracbase/60);
	    out += sprintf (out, "%02d", m);
	    break;
	case 600:	/* dd:mm.m */
	    	out += sprintf (out, "%02d.%1d", f/10, f%10);
	    break;
	case 3600:	/* dd:mm:ss */
	    m = f/(fracbase/60);
	    s = f%(fracbase/60);
	    out += sprintf (out, "%02d:%02d", m, s);
	    break;
	case 36000:	/* dd:mm:ss.s*/
	    m = f/(fracbase/60);
	    s = f%(fracbase/60);
	    out += sprintf (out, "%02d:%02d.%1d", m, s/10, s%10);
	    break;
	case 360000:	/* dd:mm:ss.ss */
	    m = f/(fracbase/60);
	    s = f%(fracbase/60);
	    out += sprintf (out, "%02d:%02d.%02d", m, s/100, s%100);
	    break;
	default:
            return -1;
	}

	return (out - out0);
}

/*####################LX200##########################*/
void cmd_firware_date(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "01/07/13\n");    
}
void cmd_firware_time(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "12:16:00\n");    
}
void cmd_firware_number(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "0.1\n");    
}
void cmd_firware_product(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "AAM radioantena\n");    
}
void cmd_firware_ver(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "LX200 Clone. Ver 0.1\n");    
}

void cmd_mount_mode(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   mount_mode="P";
   chprintf(chp, "%s\n",mount_mode);    
}

/*###########TIME##############*/

void cmd_setLocalDate(BaseSequentialStream *chp, int argc, char *argv) {
  time_t unix_time;
  struct tm timp,*ptm;
  if (argc != 1 ) {
    chprintf(chp, "0");
    return;
  }
//http://pubs.opengroup.org/onlinepubs/007904975/functions/strptime.html
   if (strptime(argv, "%m/%d/%y", &timp) == NULL) {
      chprintf(chp, "0");
     return;
   }
   unix_time = rtcGetTimeUnixSec(&RTCD1);
   ptm = gmtime(&unix_time);
   timp.tm_hour= ptm->tm_hour;
   timp.tm_min= ptm->tm_min;
   timp.tm_sec= ptm->tm_sec;
   timp.tm_isdst=0;
   unix_time=mktime(&timp);
  if (unix_time == -1){
      chprintf(chp, "0");
      return;
   }
   rtcSetTimeUnixSec(&RTCD1, unix_time);
   chprintf(chp, "1");    
}

void cmd_getLocalDate(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
  struct tm timp;
  time_t unix_time;
  char date[10];
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
  unix_time = rtcGetTimeUnixSec(&RTCD1);
  if (unix_time == -1){
      chprintf(chp, "incorrect time in RTC cell\r\n");
      chprintf(chp, "0");
      return;
  } else{
      rtcGetTimeTm(&RTCD1, &timp);
      strftime(date,10,"%m/%d/%y",&timp);
      chprintf(chp, "%s#",date);
  }  
}

void cmd_setLocalTime(BaseSequentialStream *chp, int argc, char *argv) {
  struct tm timp,*ptm;
  time_t unix_time;
  if (argc != 1 ) {
    chprintf(chp, "0");
    return;
  }
//http://pubs.opengroup.org/onlinepubs/007904975/functions/strptime.html
   if (strptime(argv, "%H:%M:%S", &timp) == NULL) {
      chprintf(chp, "0");
     return;
   }
   unix_time = rtcGetTimeUnixSec(&RTCD1);
   ptm = gmtime(&unix_time);
   timp.tm_year= ptm->tm_year;
   timp.tm_mon= ptm->tm_mon;
   timp.tm_mday= ptm->tm_mday;
   timp.tm_isdst=0;
   unix_time=mktime(&timp);
  if (unix_time == -1){
      chprintf(chp, "0");
      return;
   }
   rtcSetTimeUnixSec(&RTCD1, unix_time);
   chprintf(chp, "1");    
}

void cmd_getLocalTime(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
  struct tm timp;
  time_t unix_time;
  char date[10];
  if (argc != 0 ) {
    chprintf(chp, "0\n");
    return;
  }
  unix_time = rtcGetTimeUnixSec(&RTCD1);
  if (unix_time == -1){
      chprintf(chp, "incorrect time in RTC cell\r\n");
      chprintf(chp, "0");
      return;
  } else{
      rtcGetTimeTm(&RTCD1, &timp);
      strftime(date,10,"%H:%M:%S",&timp);
      chprintf(chp, "%s#",date);
  }  
}

void cmd_getClockFormat(BaseSequentialStream *chp, int argc, char *argv){
  (void) argv;
 if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "24#");
}

/*############SITE LAT/LON#####*/

void cmd_getSite1Name(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
 if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "Bonilla#");
}


void cmd_setSiteLongitude(BaseSequentialStream *chp, int argc, char *argv){
  float dp;
  if (argc != 1 || (f_scansexa (argv, &dp)!=0 )) {
      chprintf(chp, "0");
      return;
   }
//Format DDD*MM
   siteLatLon.lng=dp;
   chprintf(chp, "1");  
}

void cmd_getSiteLongitude(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
 if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "%f#",siteLatLon.lng);
}

void cmd_setSiteLatitude(BaseSequentialStream *chp, int argc, char *argv){
  float dp;
  if (argc != 1 || (f_scansexa (argv, &dp)!=0 )) {
      chprintf(chp, "0");
      return;
   }
//Format sDD*MM
   siteLatLon.lat=dp;
   chprintf(chp, "1");  
}

void cmd_getSiteLatitude(BaseSequentialStream *chp, int argc, char *argv) {
  (void) argv;
 if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "%f#",siteLatLon.lat);
}

/*#############COORDS##############*/

void cmd_setTargetRA(BaseSequentialStream *chp, int argc, char *argv){
  float dp;
  if (argc != 1 || (f_scansexa (argv, &dp)!=0 )) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "1");    
   targetRA=dp;
   telescopeRA=dp; 	
}

void cmd_setTargetDEC(BaseSequentialStream *chp, int argc, char *argv){
  float dp;
  if (argc != 1 || (f_scansexa (argv, &dp)!=0 )) {
    chprintf(chp, "0");
    return;
  }
   chprintf(chp, "1");
   targetDEC=dp;
   telescopeDEC=dp;
}

void cmd_getTargetRA(BaseSequentialStream *chp, int argc, char *argv){
  (void) argv;
 char out[20];
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   fs_sexa (out, targetRA, 0, 3600,0);
   chprintf(chp, "%s#",out);
}

void cmd_getTargetDEC(BaseSequentialStream *chp, int argc, char *argv){
  (void) argv;
 char out[20];
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   fs_sexa (out, targetDEC, 0, 3600,1);
   chprintf(chp, "%s#",out);
}

void cmd_getTelescopeRA(BaseSequentialStream *chp, int argc, char *argv){
  (void) argv;
 char out[20];
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   fs_sexa (out, telescopeRA, 0, 3600,0);
   chprintf(chp, "%s#",out);
}

void cmd_getTelescopeDEC(BaseSequentialStream *chp, int argc, char *argv){
  (void) argv;
 char out[20];
  if (argc != 0 ) {
    chprintf(chp, "0");
    return;
  }
   fs_sexa (out, telescopeDEC, 0, 3600,1);
   chprintf(chp, "%s#",out);
}

/*######### Motion #######*/
void cmd_slew(BaseSequentialStream *chp, int argc, char *argv){
  (void) argv;
  if (argc != 0 ) {
    return;
  }
    chprintf(chp, "0");
}

void cmd_abortMotion(BaseSequentialStream *chp, int argc, char *argv){
  (void) argv;
  (void) chp;
  if (argc != 0 ) {
    return;
  }
}

