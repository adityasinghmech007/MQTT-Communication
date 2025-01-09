/*
 * config.h
 *
 *  Created on: Jun 3, 2024
 *      Author: SEIL
 */

#ifndef CONFIG_H
#define CONFIG_H

//Interval between consecutive transmissions
#define MINUTES_INTERVAL 60000 // 1 minutes in milliseconds

// Define macros for conditional compilation
#define SINGLE_PHASE  // Uncomment this line to define SINGLE_PHASE
//#define THREE_PHASE   // Uncomment this line to define THREE_PHASE

// Define packet data length and txData array based on the phase
#ifdef SINGLE_PHASE
    #define PACKET_DATA_LENGTH 70
    #define TX_DATA {0x47, 0x01, 0x37}
    //static char topic[] = "testsub";
#elif defined(THREE_PHASE)
    #define PACKET_DATA_LENGTH 122
    #define TX_DATA {0x52, 0x47, 0x01, 0xC5}
    static char topic[] = "inverter/3p/1/data";
#endif

#endif // CONFIG_H

