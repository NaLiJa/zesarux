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

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>


#include "if1.h"
#include "cpu.h"
#include "debug.h"
#include "utils.h"
#include "operaciones.h"
#include "zxvision.h"
#include "compileoptions.h"

z80_bit if1_enabled={0};
z80_byte *if1_memory_pointer;
z80_bit if1_rom_paged={0};




#define IF1_ROM_NAME "if1-v2.rom"

/*
http://thespeccyzone.emuunlim.org/hardwarefiles/technical_files/i1_microdrives.htm

http://faqwiki.zxnet.co.uk/wiki/ZX_Interface_1

http://www.sinclair.hu/speccyalista/konyvtar/kezikonyvek/ZXInterface1_Microdrive_Manual.pdf

http://k1.spdns.de/Vintage/Sinclair/82/Peripherals/Interface%201%20and%20Microdrives%20(Sinclair)/ROMs/


https://worldofspectrum.org/faq/reference/formats.htm

*/


//puntero a cpu core normal sin if1
//void (*cpu_core_loop_no_if1) (void);

//Rutinas originales antes de cambiarlas
//void (*if1_original_poke_byte)(z80_int dir,z80_byte valor);
//void (*if1_original_poke_byte_no_time)(z80_int dir,z80_byte valor);
//z80_byte (*if1_original_peek_byte)(z80_int dir);
//z80_byte (*if1_original_peek_byte_no_time)(z80_int dir);

int if1_nested_id_core;
int if1_nested_id_poke_byte;
int if1_nested_id_poke_byte_no_time;
int if1_nested_id_peek_byte;
int if1_nested_id_peek_byte_no_time;



z80_byte cpu_core_loop_if1(z80_int dir GCC_UNUSED, z80_byte value GCC_UNUSED)
{

	int despaginar=0;

	//Gestionar traps
	if (if1_rom_paged.v==0) {
		if (reg_pc==8 || reg_pc==5896) {
			printf ("paging if1 rom when pc=%d\n",reg_pc);
			if1_rom_paged.v=1;
		}
	}


	if (if1_rom_paged.v==1) {
		if (reg_pc==1792) {
                        printf ("unpaging if1 rom after pc=%d\n",reg_pc);
                        //if1_rom_paged.v=0;
			despaginar=1;
                }



    }

        //Llamar a anterior
        debug_nested_core_call_previous(if1_nested_id_core);

	if (despaginar) if1_rom_paged.v=0;

        //Para que no se queje el compilador, aunque este valor de retorno no lo usamos
        return 0;

}

z80_byte if1_poke_byte(z80_int dir,z80_byte valor)
{
	//Como ambas som rom, esto no tiene tratamiento especial... quiza si metiesemos una ram entre 8192-16383 entonces si...
	//if1_original_poke_byte(dir,valor);
        //Llamar a anterior
        debug_nested_poke_byte_call_previous(if1_nested_id_poke_byte,dir,valor);

        //Para que no se queje el compilador, aunque este valor de retorno no lo usamos
        return 0;
}

z80_byte if1_poke_byte_no_time(z80_int dir,z80_byte valor)
{
        //Como ambas som rom, esto no tiene tratamiento especial... quiza si metiesemos una ram entre 8192-16383 entonces si...
        //if1_original_poke_byte_no_time(dir,valor);
        //Llamar a anterior
        debug_nested_poke_byte_no_time_call_previous(if1_nested_id_poke_byte_no_time,dir,valor);


        //Para que no se queje el compilador, aunque este valor de retorno no lo usamos
        return 0;


}

z80_byte if1_peek_byte(z80_int dir,z80_byte value GCC_UNUSED)
{

	z80_byte valor_leido=debug_nested_peek_byte_call_previous(if1_nested_id_peek_byte,dir);


	if (if1_rom_paged.v && dir<16384) {
		//printf ("Returning peek at if1 %d 0x%04X\n",dir,dir);
		return if1_memory_pointer[dir&8191];
	}

	//return if1_original_peek_byte(dir);
	return valor_leido;
}

z80_byte if1_peek_byte_no_time(z80_int dir,z80_byte value GCC_UNUSED)
{

	z80_byte valor_leido=debug_nested_peek_byte_no_time_call_previous(if1_nested_id_peek_byte_no_time,dir);

        if (if1_rom_paged.v && dir<16384) {
		//printf ("Returning peek at if1 %d 0x%04X\n",dir,dir);
		return if1_memory_pointer[dir&8191];
	}

        //return if1_original_peek_byte_no_time(dir);
	return valor_leido;
}



//Establecer rutinas propias
void if1_set_peek_poke_functions(void)
{

        int activar=0;

        //Ver si ya no estaban activas. Porque ? Tiene sentido esto? Esto seguramente vino de diviface.c en que a veces se llama aqui
	//estando ya la intefaz activa. Pero quiza en este if1 no sucedera nunca. Quitar esta comprobacion?
        if (poke_byte!=poke_byte_nested_handler) {
                debug_printf (VERBOSE_DEBUG,"poke_byte_nested_handler is not enabled calling if1_set_peek_poke_functions. Enabling");
                activar=1;
        }

        else {
                //Esta activo el handler. Vamos a ver si esta activo el if1 dentro
                if (debug_nested_find_function_name(nested_list_poke_byte,"if1 poke_byte")==NULL) {
                        //No estaba en la lista
                        activar=1;
                        debug_printf (VERBOSE_DEBUG,"poke_byte_nested_handler is enabled but not found if1 poke_byte. Enabling");
                }

        }


        if (activar) {
                debug_printf (VERBOSE_DEBUG,"Setting IF1 poke / peek functions");
                //Guardar anteriores
                //if1_original_poke_byte=poke_byte;
                //if1_original_poke_byte_no_time=poke_byte_no_time;
                //if1_original_peek_byte=peek_byte;
                //if1_original_peek_byte_no_time=peek_byte_no_time;

                //Modificar y poner las de if1
                //poke_byte=if1_poke_byte;
                //poke_byte_no_time=if1_poke_byte_no_time;
                //peek_byte=if1_peek_byte;
                //peek_byte_no_time=if1_peek_byte_no_time;


	        //Asignar mediante nuevas funciones de core anidados
	        if1_nested_id_poke_byte=debug_nested_poke_byte_add(if1_poke_byte,"if1 poke_byte");
        	if1_nested_id_poke_byte_no_time=debug_nested_poke_byte_no_time_add(if1_poke_byte_no_time,"if1 poke_byte_no_time");
	        if1_nested_id_peek_byte=debug_nested_peek_byte_add(if1_peek_byte,"if1 peek_byte");
        	if1_nested_id_peek_byte_no_time=debug_nested_peek_byte_no_time_add(if1_peek_byte_no_time,"if1 peek_byte_no_time");

        }
}

//Restaurar rutinas de if1
void if1_restore_peek_poke_functions(void)
{
                debug_printf (VERBOSE_DEBUG,"Restoring original poke / peek functions before IF1");
                //poke_byte=if1_original_poke_byte;
                //poke_byte_no_time=if1_original_poke_byte_no_time;
                //peek_byte=if1_original_peek_byte;
                //peek_byte_no_time=if1_original_peek_byte_no_time;

        debug_nested_poke_byte_del(if1_nested_id_poke_byte);
        debug_nested_poke_byte_no_time_del(if1_nested_id_poke_byte_no_time);
        debug_nested_peek_byte_del(if1_nested_id_peek_byte);
        debug_nested_peek_byte_no_time_del(if1_nested_id_peek_byte_no_time);

}

void if1_set_core_function(void)
{
        int activar=0;

        //Ver si ya no estaban activas. Porque ? Tiene sentido esto? Esto seguramente vino de diviface.c en que a veces se llama aqui
        //estando ya la intefaz activa. Pero quiza en este if1 no sucedera nunca. Quitar esta comprobacion?
        if (cpu_core_loop!=cpu_core_loop_nested_handler) {
                debug_printf (VERBOSE_DEBUG,"cpu_core_loop_nested_handler is not enabled calling if1_set_core_functions. Enabling");
                activar=1;
        }

        else {
                //Esta activo el handler. Vamos a ver si esta activo el if1 dentro
                if (debug_nested_find_function_name(nested_list_core,"if1 cpu_core_loop")==NULL) {
                        //No estaba en la lista
                        activar=1;
                        debug_printf (VERBOSE_DEBUG,"cpu_core_loop_nested_handler is enabled but not found if1 cpu_core_loop. Enabling");
                }

        }


        if (activar) {
		debug_printf (VERBOSE_DEBUG,"Setting IF1 Core loop");
		//Guardar anterior
		//cpu_core_loop_no_if1=cpu_core_loop;

		//Modificar
		//cpu_core_loop=cpu_core_loop_if1;
		if1_nested_id_core=debug_nested_core_add(cpu_core_loop_if1,"if1 cpu_core_loop");
	}
}

void if1_restore_core_function(void)
{
	debug_printf (VERBOSE_DEBUG,"Restoring original if1 core loop");
	//cpu_core_loop=cpu_core_loop_no_if1;
        debug_nested_core_del(if1_nested_id_core);
}

void disable_if1(void)
{
	if1_restore_core_function();
	if1_restore_peek_poke_functions();
	if1_enabled.v=0;
	free(if1_memory_pointer);
}

z80_byte *if1_microdrive_buffer;


int mdr_total_sectors=0;

#define MDR_BYTES_PER_SECTOR 543

void enable_if1(void)
{
	if (if1_enabled.v) return;
	//Asignar memoria
    int size=8192;

    debug_printf (VERBOSE_DEBUG,"Allocating %d kb of memory for Interface 1 emulation",size/1024);

    if1_memory_pointer=malloc(size);
    if (if1_memory_pointer==NULL) {
            cpu_panic ("No enough memory for Interface 1 emulation emulation");
    }

	//Cargar ROM
	FILE *ptr_if1_romfile;
    int leidos=0;

    debug_printf (VERBOSE_INFO,"Loading if1 firmware %s",IF1_ROM_NAME);

    open_sharedfile(IF1_ROM_NAME,&ptr_if1_romfile);


    if (ptr_if1_romfile!=NULL) {
            leidos=fread(if1_memory_pointer,1,size,ptr_if1_romfile);
            fclose(ptr_if1_romfile);
    }



    if (leidos!=size || ptr_if1_romfile==NULL) {
            debug_printf (VERBOSE_ERR,"Error reading Interface 1 firmware, file " IF1_ROM_NAME );
            //Lo desactivamos asi porque el disable hace otras cosas, como cambiar el core loop, que no queremos
            if1_enabled.v=0;
            return ;
    }




      //Cargar microdrive de prueba
       if1_microdrive_buffer=malloc(256*1024);
       if (if1_microdrive_buffer==NULL) {
                cpu_panic ("No enough memory for Microdrive buffer");
       }

       FILE *ptr_microdrive_file;
       //Leer archivo mdr
        ptr_microdrive_file=fopen("prueba.mdr","rb");

       if (ptr_microdrive_file==NULL) {
               debug_printf (VERBOSE_ERR,"Cannot locate prueba.mdr");
       }

       else {
               //Leer todo el archivo microdrive de prueba
               int leidos=fread(if1_microdrive_buffer,1,97740,ptr_microdrive_file);
               printf ("leidos %d bytes de microdrive\n",leidos);

                mdr_total_sectors=leidos/MDR_BYTES_PER_SECTOR;

               fclose(ptr_microdrive_file);
       }





	//Cambio core cpu
	if1_set_core_function();
	//Cambio peek, poke function
	if1_set_peek_poke_functions();


	if1_rom_paged.v=0;
	if1_enabled.v=1;
}


//int puntero_mdr=0;

//-1 si no aplica
int microdrive_get_visualmem_position(unsigned int address)
{
#ifdef EMULATE_VISUALMEM


    //El buffer de visualmem en este caso tiene mismo tamaño que dispositivo microdrive
    int posicion_final=address;

    //por si acaso
    if (posicion_final>=0 && posicion_final<VISUALMEM_MICRODRIVE_BUFFER_SIZE) {
        return posicion_final;


    }


#endif

	return -1;
}

void microdrive_set_visualmem_read(unsigned int address)
{
#ifdef EMULATE_VISUALMEM
	int posicion_final=microdrive_get_visualmem_position(address);
	if (posicion_final>=0) {
		set_visualmemmicrodrive_read_buffer(posicion_final);
	}

#endif
}

void microdrive_set_visualmem_write(unsigned int address)
{
#ifdef EMULATE_VISUALMEM
	int posicion_final=microdrive_get_visualmem_position(address);
	if (posicion_final>=0) {
		set_visualmemmicrodrive_write_buffer(posicion_final);
	}

#endif
}

int mdr_current_sector=0;
int mdr_current_offset_in_sector=0;

void mdr_next_sector(void)
{

    mdr_current_offset_in_sector=0;
    mdr_current_sector++;
    if (mdr_current_sector>=mdr_total_sectors) mdr_current_sector=0;
}


z80_byte mdr_next_byte(void)
{



    int offset_to_sector=mdr_current_sector*MDR_BYTES_PER_SECTOR;

    int offset_efectivo;


    offset_efectivo=mdr_current_offset_in_sector;


    offset_efectivo +=offset_to_sector;

    z80_byte valor=if1_microdrive_buffer[offset_efectivo];

    microdrive_set_visualmem_read(offset_efectivo);

    printf("Retornando byte mdr de offset en PC=%04XH sector %d, offset %d (offset_efectivo=%d) =0x%02X\n",
        reg_pc,mdr_current_sector,mdr_current_offset_in_sector,offset_efectivo,valor);



    mdr_current_offset_in_sector++;


    if (mdr_current_offset_in_sector>=MDR_BYTES_PER_SECTOR) {
        mdr_next_sector();

    }



    return valor;

}

//Contador simple para saber si tenemos que devolver gap, sync o datos
int contador_estado_microdrive=0;




z80_byte interface1_get_value_port(z80_byte puerto_l)
{

    //Puerto de estado
    if (puerto_l==0xef) {
        //printf ("In Port %x asked, PC after=0x%x\n",puerto_l+256*puerto_h,reg_pc);

        /*
        Microdrive cartridge
        GAP      PREAMBLE      15 byte      GAP      PREAMBLE      15 byte    512     1
        [-----][00 00 ... ff ff][BLOCK HEAD][-----][00 00 ... ff ff][REC HEAD][ DATA ][CHK]
        Preamble = 10 * 0x00 + 2 * 0xff (12 byte)
        */


        contador_estado_microdrive++;



        z80_byte return_value=0;

        //numero arbitrario realmente, cada cuanto incrementamos el contador para pasar de un estado al otro
        //La rom del interface1 por ejemplo cuando está leyendo datos (puerto e7) no está leyendo el puerto de estado (ef)
        //por tanto ese incremento del estado de datos (valor 0) a paso a estado gap lo producimos cuando ha pasado el contadort
        //aunque en dispositivo real esto sucederia justo al dejar de enviar los 543 bytes
        //Logicamente esto no va a la velocidad real ni cuento t-estados ni nada, por ejemplo si lees del puerto
        //de datos, te llegará el siguiente byte, y su vuelves a leer, aunque no haya pasado el tiempo "real" del microdrive
        //para que llegue el siguiente byte, te llegará
        #define MICRODRIVE_PASOS_CAMBIO_ESTADO 20

        if      (contador_estado_microdrive<MICRODRIVE_PASOS_CAMBIO_ESTADO) return_value=4; //gap
        else if (contador_estado_microdrive<MICRODRIVE_PASOS_CAMBIO_ESTADO*2) return_value=2; //sync
        else if (contador_estado_microdrive<MICRODRIVE_PASOS_CAMBIO_ESTADO*3) return_value=0; //datos

        else if (contador_estado_microdrive<MICRODRIVE_PASOS_CAMBIO_ESTADO*4) return_value=4; //gap
        else if (contador_estado_microdrive<MICRODRIVE_PASOS_CAMBIO_ESTADO*5) return_value=2; //sync
        else if (contador_estado_microdrive<MICRODRIVE_PASOS_CAMBIO_ESTADO*6) return_value=0; //datos

        if (contador_estado_microdrive>=MICRODRIVE_PASOS_CAMBIO_ESTADO*6) {
            mdr_next_sector();
            contador_estado_microdrive=0; //1000
        }

        printf ("In Port %x asked, PC after=0x%x contador_estado_microdrive=%d return_value=0x%x\n",puerto_l,reg_pc,contador_estado_microdrive,return_value);

        return return_value;
    }

    if (puerto_l==0xe7) {

        return mdr_next_byte();
    }


    return 0;

}