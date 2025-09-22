/* This file is part of Lu programming language
 *
 * Copyright (C) 2025  Hugo Coto Flórez
 *
 * Lu is free software; you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or any later version.
 *
 * Lu is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY of FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Lu. If not, see <https://www.gnu.org/licenses/>
 *
 * For questions or support, contact: hugo.coto@member.fsf.org
 */

#ifndef _H_
#define _H_

#include "common.h"

#define reportfile "./report.log"

void
report(const char *format, ...);

#define report(format, ...)                                              \
        do {                                                             \
                FILE *file = fopen(reportfile, "a");                     \
                fprintf(file, "[%s:%s]  ", __FILE_NAME__, __FUNCTION__); \
                fprintf(file, format, ##__VA_ARGS__);                    \
                fprintf(file, "\n");                                     \
                fclose(file);                                            \
        } while (0)

#endif
