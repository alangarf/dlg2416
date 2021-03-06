/*-------------------------------------------------------------------------*/
/* DLG2416 control module include/configuration file                       */
/*-------------------------------------------------------------------------*/

#ifndef _DISP2416
#define _DISP2416

/*-------------------------------------------------------------------------*/
/* Configuration Options                                                   */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* API declareations                                                       */
/*-------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>

void disp_init(void);
void disp_clear(void);
int disp_putchar(char chr, FILE *unused);

#endif /* #ifndef _DISP2416 */
