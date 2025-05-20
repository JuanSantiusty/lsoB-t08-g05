/**
 * @file
 * @ingroup kernel_code
 * @author Erwin Meza <emezav@gmail.com>
 * @copyright GNU Public License.
 *
 * @brief Código de inicialización del kernel en C
 *
 * Este codigo recibe el control de start.S y continúa con la ejecución.
*/
#include <asm.h>
#include <console.h>
#include <irq.h>
#include <kmem.h>
#include <paging.h>
#include <pci.h>
#include <pm.h>
#include <physmem.h>
#include <stdlib.h>
#include <string.h>

//Variable global para estado del teclado
int estado_ctrl=0;
//Mapa de ctrl
char mapa_ctrl[]={0,0};

void cmain(){

    /* Inicializar y limpiar la consola console.c */
    setup_console();

     /* Inicializar la estructura para gestionar la memoria física. physmem.c */
    setup_physical_memory();

    /* Las subrutinas que se deben ejecutar ANTES de habilitar las
     * interrupciones se deben invicar en este punto */
     
     

    /* Configura la IDT y el PIC.interrupt.c */
    setup_interrupts();

    /* Completa la configuración de la memoria virtual. paging.c */
    setup_paging();

    /* Configurar la gestión de páginas de memoria virtual. kmem.c */
    setup_kmem();

    /* Las subrutinas que se deben ejecutar DESPUES de habilitar las
     * interrupciones se deben invicar en este punto */

    /* Inicializar la informacion del bus PCI. pci.c  */
    setup_pci();

    /* Imprimir un mensaje por pantalla. */
    console_printf("Kernel started.\n");

}

//Función para procesar teclas modificadas
void procesar_modificadoras(unsigned char code){
  switch(code){
    case 0x1D:  //Ctrl make
      estado_ctrl = 1;
      break;
    case 0x9D:  //Ctrl break
      estado_ctrl = 0;
      break;
  }
}

//Función para saber si algo es imprimible
int es_imprimible(unsigned char code) {
  if((code >= 0x02 && code <= 0x0D) ||
    (code >= 0x10 && code <= 0x1C) ||
    (code >= 0x1E && code <= 0x26) ||
    (code >= 0x2C && code <= 0x32) ||
    (code == 0x39 )
    ){
    return 1;
  }
  return 0;
}

//Función para traducir ScanCode
unsigned char traducir(unsigned char code){
     static const unsigned char ascii_table[] = {
        0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
        0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
        0, ' ', 0, 0
    };
    
     if (code >= sizeof(ascii_table)/sizeof(ascii_table[0])) {
        return 0;
    }
        return ascii_table[code];
}
