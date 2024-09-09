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

#ifndef IF1_H
#define IF1_H

#include "cpu.h"

extern z80_bit if1_enabled;

extern void enable_if1(void);
extern void disable_if1(void);

extern z80_bit if1_rom_paged;

extern z80_byte interface1_get_value_port(z80_byte puerto_l);
extern void interface1_write_value_port(z80_byte puerto_l,z80_byte value);

#endif
