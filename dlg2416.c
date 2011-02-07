/*-------------------------------------------------------------------------*/
/* DLG2416 control module                                                  */
/*-------------------------------------------------------------------------*/

#include "dlg2416.h"

static void disp_putc(uint8_t chr);
static void disp_write(char dat);

/*-------------------------------------------------------------------------*/
/* Platform dependent macros and functions needed to be modified           */
/*-------------------------------------------------------------------------*/

#include <avr/io.h> /* device specific include files */
#include <util/delay.h>

#define IF_INIT()   { PORTC = 0x80; DDRC = 0xFF; PORTD = 0xF4; DDRD = 0xFF; } /* initialize control ports */

#define WR_HIGH()   PORTC |= 0x80   /* set WR high */
#define WR_LOW()    PORTC &= 0x7F   /* set WR low */
#define A0_HIGH()   PORTD |= 0x01   /* set A0 high */
#define A0_LOW()    PORTD &= 0xFE   /* set A0 low */
#define A1_HIGH()   PORTD |= 0x02   /* set A1 high */
#define A1_LOW()    PORTD &= 0xFD   /* set A1 low */
#define CLR_HIGH()  PORTD |= 0x04   /* set CLR high */
#define CLR_LOW()   PORTD &= 0xFB   /* set CLR low */
#define BLK_HIGH()  PORTD |= 0x80   /* set BL high */
#define BLK_LOW()   PORTD &= 0x7F   /* set BL low */

#define DISP_1()    PORTD = (PORTD | 0x40) ^ (PORTD & 0x20)  /* select display 1 */
#define DISP_2()    PORTD = (PORTD | 0x20) ^ (PORTD & 0x40)  /* select display 2 */

#define OUT_DATA(d) PORTC = (PORTC & 0x80) | (d & 0x7F)      /* output d but mask WR */

#define DELAY_US(n) _delay_us(n)
#define NOP()       __asm__ __volatile__("nop")

/*---------------------------------------------------------------*/
/* avr-libc stdio compatible put character                       */
/*---------------------------------------------------------------*/
int disp_putchar(char chr, FILE *unused)
{
    disp_putc(chr);
    return 0;
}

/*-------------------------------------------------------------------------*/
/* Non-platform specific functions                                         */
/*-------------------------------------------------------------------------*/

static
uint8_t column; /* current cursor position */

/*---------------------------------------------------------------*/
/* Initialize the display                                        */
/*---------------------------------------------------------------*/

void disp_init(void)
{
    IF_INIT();

    disp_clear();
}

/*---------------------------------------------------------------*/
/* Clears the display                                            */
/*---------------------------------------------------------------*/

void disp_clear(void)
{
    CLR_LOW();
    NOP();
    NOP();
    CLR_HIGH();
}

/*---------------------------------------------------------------*/
/* Write byte to display                                         */
/*---------------------------------------------------------------*/

static
void disp_write(char dat)
{
    OUT_DATA(dat);

    WR_LOW();
    NOP();
    NOP();
    WR_HIGH();
}

/*---------------------------------------------------------------*/
/* Put a character                                               */
/*---------------------------------------------------------------*/

static
void disp_putc(uint8_t chr)
{
    switch (chr)
    {
        case '\n': /* newline / return home */
            column = 0;
            break;

        case '\f':  /* clear screen */
            column = 0;
            disp_clear();
            break;

        case '\b':  /* backspace */
            if (column) {
                column--;
            }
            break;

        default:    /* everything else */

            // magic numbers run away!
            // TODO - must allow for more than two displays
            if (column > 7) {
                column = 0;
            }

            if (column > 3) {
                DISP_2();
            } else {
                DISP_1();
            }

            if ((column & 0x01) == 0x01) {
                A0_LOW();
            } else {
                A0_HIGH();
            }

            if ((column & 0x02) == 0x02) {
                A1_LOW();
            } else {
                A1_HIGH();
            }

            disp_write(chr);

            column++;
            break;
    }
}
