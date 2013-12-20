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

/**
 * @file    lx200Shell.h
 * @brief   Simple CLI lx200Shell header.
 *
 * @addtogroup lx200Shell
 * @{
 */

#ifndef _lx200Shell_H_
#define _lx200Shell_H_

#define SHELL_WA_SIZE   THD_WA_SIZE(1024)



/**
 * @brief   lx200Shell maximum input line length.
 */
#if !defined(lx200Shell_MAX_LINE_LENGTH) || defined(__DOXYGEN__)
#define lx200Shell_MAX_LINE_LENGTH       20
#endif


/**
 * @brief   Command handler function type.
 */
typedef void (*lx200Shellcmd_t)(BaseSequentialStream *chp, int argc, char *argv);

/**
 * @brief   Custom command entry type.
 */
typedef struct {
  const char            *sc_name;           /**< @brief Command name.       */
  lx200Shellcmd_t            sc_function;        /**< @brief Command function.   */
} lx200ShellCommand;

/**
 * @brief   lx200Shell descriptor type.
 */
typedef struct {
  BaseSequentialStream  *sc_channel;        /**< @brief I/O channel associated
                                                 to the lx200Shell.              */
  const lx200ShellCommand    *sc_commands;       /**< @brief lx200Shell extra commands
                                                 table.                     */
} lx200ShellConfig;

#if !defined(__DOXYGEN__)
extern EventSource lx200Shell_terminated;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void lx200ShellInit(void);
  Thread *lx200ShellCreate(const lx200ShellConfig *scp, size_t size, tprio_t prio);
  Thread *lx200ShellCreateStatic(const lx200ShellConfig *scp, void *wsp,
                            size_t size, tprio_t prio);
  bool_t lx200ShellGetLine(BaseSequentialStream *chp, char *line, unsigned size);
#ifdef __cplusplus
}
#endif

#endif /* _lx200Shell_H_ */

/** @} */
