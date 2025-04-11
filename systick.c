/* 
 * Gestion des ticks systèmes = base de temps à la milliseconde
 *  
 * File:   ticks.c
 * Author: phifo
 *
 * Created on 18 mars 2025, 09:28
 */

#include "mcc_generated_files/system/system.h"

static uint32_t system_ticks__ = 0;

/**
 * penser a ajouter (dans le fichier interrupt.c) :
 *         if(PIE2bits.TMR6IE == 1 && PIR2bits.TMR6IF == 1)
 *              {
 *                  TMR6_ISR();
 *              } 
 */
//inline void SYSTICK_interrupt_handler(void) {
void TMR6_ISR(void)
{
   PIR2bits.TMR6IF = 0;
   system_ticks__++;
}

/**
 * SYSTICK initialization
 *
 * Idée de base :   
 * Le systick est basé sur le TIMER0. La configuration et l'interruption est 
 * géré par le MCC.
 * La seule config à ajouter concenrne le callback de l'interruption
 * ==> Fonctionnement OK
 * 
 * Mais simplification et maitrise de la config sans MCC (problèmatique)
 * recup de la config de TIMER6 pour la génération d'un IT à la milliseconde
 *  
 * Et création de l'interruption (sans passer par le callback) gain d'un appel de fonction
 * 
 */
void SYSTICK_initialize(void) {
    T6CLKCON = (1 << _T6CLKCON_T6CS_POSN);  // T6CS FOSC/4

    T6HLT = (0 << _T6HLT_T6MODE_POSN)   // T6MODE Software control
        | (0 << _T6HLT_CSYNC_POSN)   // CSYNC Not Synchronized
        | (0 << _T6HLT_CPOL_POSN)   // CPOL Rising Edge
        | (0 << _T6HLT_T6PSYNC_POSN);  // T6PSYNC Not Synchronized

    T6RST = (0 << _T6RST_T6RSEL_POSN);  // T6RSEL T6CKIPPS pin

    T6PR = 0xF9;    // Period 0.001s; Prescaled Timer Frequency 250 kHz; Count 249; Postscaler 1:1; 

    T6TMR = 0x0;
   
    PIR2bits.TMR6IF = 0;   
    PIE2bits.TMR6IE = 1;

    T6CON = (5 << _T6CON_T6CKPS_POSN)   // T6CKPS 1:32
        | (1 << _T6CON_TMR6ON_POSN)   // TMR6ON on
        | (0 << _T6CON_T6OUTPS_POSN);  // T6OUTPS 1:1
}

/**
 * Lecture du tick système (avec protection des interruptions)
 * 
 * 
 * @return 
 */
uint32_t SYSTICK_read(void) {
    uint32_t tmp;

    INTERRUPT_GlobalInterruptDisable();
    tmp = system_ticks__;
    INTERRUPT_GlobalInterruptEnable();

    return tmp;
}

/**
 * Test de dépassement de temps.
 * 
 * Lit la valeur actuelle du compteur de ticks (avec protection des interruptions)
 * puis compare avec la valeur passée en paramètre.
 * 
 * 
 * @param t : la valeur à comparer
 * @return : vrai si le systick est superieur au parametre (dépassement)
 */
bool SYSTICK_isOverrun(uint32_t t) {
    uint32_t tmp;

    INTERRUPT_GlobalInterruptDisable();
    tmp = system_ticks__;
    INTERRUPT_GlobalInterruptEnable();

    if (t <= tmp)
        return true;
    else
        return false;
}


// End of file

