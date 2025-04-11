/* 
 * File:   ticks.h
 * Author: phifo
 *
 * Created on 14 mars 2025, 09:52
 */

#ifndef TICKS_H
#define	TICKS_H

#ifdef	__cplusplus
extern "C" {
#endif

void SYSTICK_initialize (void); 
    
uint32_t SYSTICK_read (void);

bool SYSTICK_isOverrun (uint32_t);
    //if (currentTime - lastUpdate >= interval) {
        //lastUpdate = currentTime;


#ifdef	__cplusplus
}
#endif

#endif	/* TICKS_H */

