/*
    ZEsarUX  ZX Second-Emulator And Released for UniX
    Copyright (C) 2013 Cesar Hernandez Bano

    This file is part of ZEsarUX.

    ZEsarUX is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef AUDIONULL_H
#define AUDIONULL_H

#include "cpu.h"

extern int audionull_init(void);
extern void audionull_end(void);
extern int audionull_thread_finish(void);
extern void audionull_send_frame(char *buffer);
extern void audionull_get_buffer_info (int *buffer_size,int *current_buffer_position);
extern int audionull_can_record_input(void);

#endif
