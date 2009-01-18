/* babl - dynamically extendable universal pixel conversion library.
 * Copyright (C) 2005-2008, Øyvind Kolås and others.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef _BABL_MAIN_H
#define _BABL_MAIN_H

#ifndef _BABL_H
#error  this file is only to be included by babl.h
#endif

/**
 * Initialize the babl library
 */
void   babl_init       (void);

/**
 * Deinitialize the babl library (frees any resources used, if the
 * number of calls to babl_exit() is is equal to the number of calls
 * to babl_init()
 */
void   babl_exit       (void);


#ifndef BABL_DISABLE_DEPRECATED

#define babl_destroy babl_exit

#endif /* BABL_DISABLE_DEPRECATED */


#endif
