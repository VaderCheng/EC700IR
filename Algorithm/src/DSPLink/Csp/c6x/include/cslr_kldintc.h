/***************************************************************************\
* THE FOLLOWING REGISTERS or REGISTER'S bit fields ARE NOT VALID(RESERVED) * 
* IN KALEIDO11                                                             *
*                                                                          *  
* -> STAT,MASK,RAW_STAT,CLR and INTC_TEST_SET Regsiter's Bit 11 and Bit 10 * 
* -> SRC10CFG and SRC11CFG REGISTERS                                       *
*                                                                          *  
*                                                                          *  
\**************************************************************************/

#ifndef _CSLR_KLDINTC_1_H_
#define _CSLR_KLDINTC_1_H_

#include <cslr.h>

#include "tistdtypes.h"


/* Minimum unit = 4 bytes */

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 CFG;
    volatile Uint32 STAT;
    volatile Uint32 MSK;
    volatile Uint32 RAW_STAT;
    volatile Uint32 ENTRY;
    volatile Uint32 CLR;
    volatile Uint32 RSVD0[2];
    volatile Uint32 GEN;
    volatile Uint32 RSVD1[7];
    volatile Uint32 TEST_CTL;
    volatile Uint32 INTC_TEST_SET;
    volatile Uint32 RSVD2[14];
    volatile Uint32 SRC0CFG;
    volatile Uint32 SRC1CFG;
    volatile Uint32 SRC2CFG;
    volatile Uint32 SRC3CFG;
    volatile Uint32 SRC4CFG;
    volatile Uint32 SRC5CFG;
    volatile Uint32 SRC6CFG;
    volatile Uint32 SRC7CFG;
    volatile Uint32 SRC8CFG;
    volatile Uint32 SRC9CFG;
    volatile Uint32 SRC10CFG;
    volatile Uint32 SRC11CFG;
} CSL_KldintcRegs;

/**************************************************************************\
* Field Definition Macros
\**************************************************************************/

/* CFG */


#define CSL_KLDINTC_CFG_SYNC_INT_MASK    (0x00000002u)
#define CSL_KLDINTC_CFG_SYNC_INT_SHIFT   (0x00000001u)
#define CSL_KLDINTC_CFG_SYNC_INT_RESETVAL (0x00000000u)
/*----SYNC_INT Tokens----*/
#define CSL_KLDINTC_CFG_SYNC_INT_SYNC    (0x00000000u)
#define CSL_KLDINTC_CFG_SYNC_INT_INT     (0x00000001u)

#define CSL_KLDINTC_CFG_SYNC_LGC_MASK    (0x00000001u)
#define CSL_KLDINTC_CFG_SYNC_LGC_SHIFT   (0x00000000u)
#define CSL_KLDINTC_CFG_SYNC_LGC_RESETVAL (0x00000000u)
/*----SYNC_LGC Tokens----*/
#define CSL_KLDINTC_CFG_SYNC_LGC_AND     (0x00000000u)
#define CSL_KLDINTC_CFG_SYNC_LGC_OR      (0x00000001u)

#define CSL_KLDINTC_CFG_RESETVAL         (0x00000000u)

/* STAT */


#define CSL_KLDINTC_STAT_STATUS_11_MASK  (0x00000800u)
#define CSL_KLDINTC_STAT_STATUS_11_SHIFT (0x0000000Bu)
#define CSL_KLDINTC_STAT_STATUS_11_RESETVAL (0x00000000u)
/*----STATUS_11 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_11_ZERO  (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_11_ONE   (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_10_MASK  (0x00000400u)
#define CSL_KLDINTC_STAT_STATUS_10_SHIFT (0x0000000Au)
#define CSL_KLDINTC_STAT_STATUS_10_RESETVAL (0x00000000u)
/*----STATUS_10 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_10_ZERO  (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_10_ONE   (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_9_MASK   (0x00000200u)
#define CSL_KLDINTC_STAT_STATUS_9_SHIFT  (0x00000009u)
#define CSL_KLDINTC_STAT_STATUS_9_RESETVAL (0x00000000u)
/*----STATUS_9 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_9_ZERO   (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_9_ONE    (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_8_MASK   (0x00000100u)
#define CSL_KLDINTC_STAT_STATUS_8_SHIFT  (0x00000008u)
#define CSL_KLDINTC_STAT_STATUS_8_RESETVAL (0x00000000u)
/*----STATUS_8 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_8_ZERO   (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_8_ONE    (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_7_MASK   (0x00000080u)
#define CSL_KLDINTC_STAT_STATUS_7_SHIFT  (0x00000007u)
#define CSL_KLDINTC_STAT_STATUS_7_RESETVAL (0x00000000u)
/*----STATUS_7 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_7_ZERO   (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_7_ONE    (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_6_MASK   (0x00000040u)
#define CSL_KLDINTC_STAT_STATUS_6_SHIFT  (0x00000006u)
#define CSL_KLDINTC_STAT_STATUS_6_RESETVAL (0x00000000u)
/*----STATUS_6 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_6_ZERO   (0x00000000u)

#define CSL_KLDINTC_STAT_STATUS_5_MASK   (0x00000020u)
#define CSL_KLDINTC_STAT_STATUS_5_SHIFT  (0x00000005u)
#define CSL_KLDINTC_STAT_STATUS_5_RESETVAL (0x00000000u)
/*----STATUS_5 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_5_ZERO   (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_5_ONE    (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_4_MASK   (0x00000010u)
#define CSL_KLDINTC_STAT_STATUS_4_SHIFT  (0x00000004u)
#define CSL_KLDINTC_STAT_STATUS_4_RESETVAL (0x00000000u)
/*----STATUS_4 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_4_ZERO   (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_4_ONE    (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_3_MASK   (0x00000008u)
#define CSL_KLDINTC_STAT_STATUS_3_SHIFT  (0x00000003u)
#define CSL_KLDINTC_STAT_STATUS_3_RESETVAL (0x00000000u)
/*----STATUS_3 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_3_ZERO   (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_3_ONE    (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_2_MASK   (0x00000004u)
#define CSL_KLDINTC_STAT_STATUS_2_SHIFT  (0x00000002u)
#define CSL_KLDINTC_STAT_STATUS_2_RESETVAL (0x00000000u)
/*----STATUS_2 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_2_ZERO   (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_2_ONE    (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_1_MASK   (0x00000002u)
#define CSL_KLDINTC_STAT_STATUS_1_SHIFT  (0x00000001u)
#define CSL_KLDINTC_STAT_STATUS_1_RESETVAL (0x00000000u)
/*----STATUS_1 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_1_ZERO   (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_1_ONE    (0x00000001u)

#define CSL_KLDINTC_STAT_STATUS_0_MASK   (0x00000001u)
#define CSL_KLDINTC_STAT_STATUS_0_SHIFT  (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_0_RESETVAL (0x00000000u)
/*----STATUS_0 Tokens----*/
#define CSL_KLDINTC_STAT_STATUS_0_ZERO   (0x00000000u)
#define CSL_KLDINTC_STAT_STATUS_0_ONE    (0x00000001u)

#define CSL_KLDINTC_STAT_RESETVAL        (0x00000000u)

/* MSK */


#define CSL_KLDINTC_MSK_MASK_11_MASK     (0x00000800u)
#define CSL_KLDINTC_MSK_MASK_11_SHIFT    (0x0000000Bu)
#define CSL_KLDINTC_MSK_MASK_11_RESETVAL (0x00000000u)
/*----MASK_11 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_11_DISABLE  (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_11_ENABLE   (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_10_MASK     (0x00000400u)
#define CSL_KLDINTC_MSK_MASK_10_SHIFT    (0x0000000Au)
#define CSL_KLDINTC_MSK_MASK_10_RESETVAL (0x00000000u)
/*----MASK_10 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_10_DISABLE  (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_10_ENABLE   (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_9_MASK      (0x00000200u)
#define CSL_KLDINTC_MSK_MASK_9_SHIFT     (0x00000009u)
#define CSL_KLDINTC_MSK_MASK_9_RESETVAL  (0x00000000u)
/*----MASK_9 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_9_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_9_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_8_MASK      (0x00000100u)
#define CSL_KLDINTC_MSK_MASK_8_SHIFT     (0x00000008u)
#define CSL_KLDINTC_MSK_MASK_8_RESETVAL  (0x00000000u)
/*----MASK_8 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_8_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_8_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_7_MASK      (0x00000080u)
#define CSL_KLDINTC_MSK_MASK_7_SHIFT     (0x00000007u)
#define CSL_KLDINTC_MSK_MASK_7_RESETVAL  (0x00000000u)
/*----MASK_7 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_7_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_7_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_6_MASK      (0x00000040u)
#define CSL_KLDINTC_MSK_MASK_6_SHIFT     (0x00000006u)
#define CSL_KLDINTC_MSK_MASK_6_RESETVAL  (0x00000000u)
/*----MASK_6 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_6_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_6_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_5_MASK      (0x00000020u)
#define CSL_KLDINTC_MSK_MASK_5_SHIFT     (0x00000005u)
#define CSL_KLDINTC_MSK_MASK_5_RESETVAL  (0x00000000u)
/*----MASK_5 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_5_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_5_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_4_MASK      (0x00000010u)
#define CSL_KLDINTC_MSK_MASK_4_SHIFT     (0x00000004u)
#define CSL_KLDINTC_MSK_MASK_4_RESETVAL  (0x00000000u)
/*----MASK_4 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_4_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_4_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_3_MASK      (0x00000008u)
#define CSL_KLDINTC_MSK_MASK_3_SHIFT     (0x00000003u)
#define CSL_KLDINTC_MSK_MASK_3_RESETVAL  (0x00000000u)
/*----MASK_3 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_3_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_3_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_2_MASK      (0x00000004u)
#define CSL_KLDINTC_MSK_MASK_2_SHIFT     (0x00000002u)
#define CSL_KLDINTC_MSK_MASK_2_RESETVAL  (0x00000000u)
/*----MASK_2 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_2_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_2_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_1_MASK      (0x00000002u)
#define CSL_KLDINTC_MSK_MASK_1_SHIFT     (0x00000001u)
#define CSL_KLDINTC_MSK_MASK_1_RESETVAL  (0x00000000u)
/*----MASK_1 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_1_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_1_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_MASK_0_MASK      (0x00000001u)
#define CSL_KLDINTC_MSK_MASK_0_SHIFT     (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_0_RESETVAL  (0x00000000u)
/*----MASK_0 Tokens----*/
#define CSL_KLDINTC_MSK_MASK_0_DISABLE   (0x00000000u)
#define CSL_KLDINTC_MSK_MASK_0_ENABLE    (0x00000001u)

#define CSL_KLDINTC_MSK_RESETVAL         (0x00000000u)

/* RAW_STAT */


#define CSL_KLDINTC_RAW_STAT_RSTATUS_11_MASK (0x00000800u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_11_SHIFT (0x0000000Bu)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_11_RESETVAL (0x00000000u)
/*----RSTATUS_11 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_11_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_11_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_10_MASK (0x00000400u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_10_SHIFT (0x0000000Au)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_10_RESETVAL (0x00000000u)
/*----RSTATUS_10 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_10_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_10_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_9_MASK (0x00000200u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_9_SHIFT (0x00000009u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_9_RESETVAL (0x00000000u)
/*----RSTATUS_9 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_9_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_9_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_8_MASK (0x00000100u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_8_SHIFT (0x00000008u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_8_RESETVAL (0x00000000u)
/*----RSTATUS_8 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_8_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_8_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_7_MASK (0x00000080u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_7_SHIFT (0x00000007u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_7_RESETVAL (0x00000000u)
/*----RSTATUS_7 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_7_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_7_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_6_MASK (0x00000040u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_6_SHIFT (0x00000006u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_6_RESETVAL (0x00000000u)
/*----RSTATUS_6 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_6_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_6_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_5_MASK (0x00000020u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_5_SHIFT (0x00000005u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_5_RESETVAL (0x00000000u)
/*----RSTATUS_5 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_5_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_5_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_4_MASK (0x00000010u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_4_SHIFT (0x00000004u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_4_RESETVAL (0x00000000u)
/*----RSTATUS_4 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_4_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_4_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_3_MASK (0x00000008u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_3_SHIFT (0x00000003u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_3_RESETVAL (0x00000000u)
/*----RSTATUS_3 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_3_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_3_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_2_MASK (0x00000004u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_2_SHIFT (0x00000002u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_2_RESETVAL (0x00000000u)
/*----RSTATUS_2 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_2_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_2_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_1_MASK (0x00000002u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_1_SHIFT (0x00000001u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_1_RESETVAL (0x00000000u)
/*----RSTATUS_1 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_1_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_1_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RSTATUS_0_MASK (0x00000001u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_0_SHIFT (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_0_RESETVAL (0x00000000u)
/*----RSTATUS_0 Tokens----*/
#define CSL_KLDINTC_RAW_STAT_RSTATUS_0_ZERO (0x00000000u)
#define CSL_KLDINTC_RAW_STAT_RSTATUS_0_ONE (0x00000001u)

#define CSL_KLDINTC_RAW_STAT_RESETVAL    (0x00000000u)

/* ENTRY */


#define CSL_KLDINTC_ENTRY_ENTRY_MASK     (0x00001FFFu)
#define CSL_KLDINTC_ENTRY_ENTRY_SHIFT    (0x00000000u)
#define CSL_KLDINTC_ENTRY_ENTRY_RESETVAL (0x00000000u)

#define CSL_KLDINTC_ENTRY_RESETVAL       (0x00000000u)

/* CLR */


#define CSL_KLDINTC_CLR_CLEAR_11_MASK    (0x00000800u)
#define CSL_KLDINTC_CLR_CLEAR_11_SHIFT   (0x0000000Bu)
#define CSL_KLDINTC_CLR_CLEAR_11_RESETVAL (0x00000000u)
/*----CLEAR_11 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_11_SET     (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_10_MASK    (0x00000400u)
#define CSL_KLDINTC_CLR_CLEAR_10_SHIFT   (0x0000000Au)
#define CSL_KLDINTC_CLR_CLEAR_10_RESETVAL (0x00000000u)
/*----CLEAR_10 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_10_SET     (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_9_MASK     (0x00000200u)
#define CSL_KLDINTC_CLR_CLEAR_9_SHIFT    (0x00000009u)
#define CSL_KLDINTC_CLR_CLEAR_9_RESETVAL (0x00000000u)
/*----CLEAR_9 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_9_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_8_MASK     (0x00000100u)
#define CSL_KLDINTC_CLR_CLEAR_8_SHIFT    (0x00000008u)
#define CSL_KLDINTC_CLR_CLEAR_8_RESETVAL (0x00000000u)
/*----CLEAR_8 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_8_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_7_MASK     (0x00000080u)
#define CSL_KLDINTC_CLR_CLEAR_7_SHIFT    (0x00000007u)
#define CSL_KLDINTC_CLR_CLEAR_7_RESETVAL (0x00000000u)
/*----CLEAR_7 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_7_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_6_MASK     (0x00000040u)
#define CSL_KLDINTC_CLR_CLEAR_6_SHIFT    (0x00000006u)
#define CSL_KLDINTC_CLR_CLEAR_6_RESETVAL (0x00000000u)
/*----CLEAR_6 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_6_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_5_MASK     (0x00000020u)
#define CSL_KLDINTC_CLR_CLEAR_5_SHIFT    (0x00000005u)
#define CSL_KLDINTC_CLR_CLEAR_5_RESETVAL (0x00000000u)
/*----CLEAR_5 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_5_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_4_MASK     (0x00000010u)
#define CSL_KLDINTC_CLR_CLEAR_4_SHIFT    (0x00000004u)
#define CSL_KLDINTC_CLR_CLEAR_4_RESETVAL (0x00000000u)
/*----CLEAR_4 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_4_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_3_MASK     (0x00000008u)
#define CSL_KLDINTC_CLR_CLEAR_3_SHIFT    (0x00000003u)
#define CSL_KLDINTC_CLR_CLEAR_3_RESETVAL (0x00000000u)
/*----CLEAR_3 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_3_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_2_MASK     (0x00000004u)
#define CSL_KLDINTC_CLR_CLEAR_2_SHIFT    (0x00000002u)
#define CSL_KLDINTC_CLR_CLEAR_2_RESETVAL (0x00000000u)
/*----CLEAR_2 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_2_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_1_MASK     (0x00000002u)
#define CSL_KLDINTC_CLR_CLEAR_1_SHIFT    (0x00000001u)
#define CSL_KLDINTC_CLR_CLEAR_1_RESETVAL (0x00000000u)
/*----CLEAR_1 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_1_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_CLEAR_0_MASK     (0x00000001u)
#define CSL_KLDINTC_CLR_CLEAR_0_SHIFT    (0x00000000u)
#define CSL_KLDINTC_CLR_CLEAR_0_RESETVAL (0x00000000u)
/*----CLEAR_0 Tokens----*/
#define CSL_KLDINTC_CLR_CLEAR_0_SET      (0x00000001u)

#define CSL_KLDINTC_CLR_RESETVAL         (0x00000000u)

/* GEN */


#define CSL_KLDINTC_GEN_SW_INT_1_MASK    (0x00000002u)
#define CSL_KLDINTC_GEN_SW_INT_1_SHIFT   (0x00000001u)
#define CSL_KLDINTC_GEN_SW_INT_1_RESETVAL (0x00000000u)
/*----SW_INT_1 Tokens----*/
#define CSL_KLDINTC_GEN_SW_INT_1_SET     (0x00000001u)

#define CSL_KLDINTC_GEN_SW_INT_0_MASK    (0x00000001u)
#define CSL_KLDINTC_GEN_SW_INT_0_SHIFT   (0x00000000u)
#define CSL_KLDINTC_GEN_SW_INT_0_RESETVAL (0x00000000u)
/*----SW_INT_0 Tokens----*/
#define CSL_KLDINTC_GEN_SW_INT_0_SET     (0x00000001u)

#define CSL_KLDINTC_GEN_RESETVAL         (0x00000000u)

/* TEST_CTL */


#define CSL_KLDINTC_TEST_CTL_INT_TEST_MASK (0x00000001u)
#define CSL_KLDINTC_TEST_CTL_INT_TEST_SHIFT (0x00000000u)
#define CSL_KLDINTC_TEST_CTL_INT_TEST_RESETVAL (0x00000000u)
/*----INT_TEST Tokens----*/
#define CSL_KLDINTC_TEST_CTL_INT_TEST_NORMAL (0x00000000u)
#define CSL_KLDINTC_TEST_CTL_INT_TEST_TEST (0x00000001u)

#define CSL_KLDINTC_TEST_CTL_RESETVAL    (0x00000000u)

/* INTC_TEST_SET */


#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_11_MASK (0x00000800u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_11_SHIFT (0x0000000Bu)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_11_RESETVAL (0x00000000u)
/*----SRC_SET_11 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_11_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_10_MASK (0x00000400u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_10_SHIFT (0x0000000Au)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_10_RESETVAL (0x00000000u)
/*----SRC_SET_10 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_10_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_9_MASK (0x00000200u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_9_SHIFT (0x00000009u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_9_RESETVAL (0x00000000u)
/*----SRC_SET_9 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_9_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_8_MASK (0x00000100u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_8_SHIFT (0x00000008u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_8_RESETVAL (0x00000000u)
/*----SRC_SET_8 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_8_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_7_MASK (0x00000080u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_7_SHIFT (0x00000007u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_7_RESETVAL (0x00000000u)
/*----SRC_SET_7 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_7_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_6_MASK (0x00000040u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_6_SHIFT (0x00000006u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_6_RESETVAL (0x00000000u)
/*----SRC_SET_6 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_6_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_5_MASK (0x00000020u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_5_SHIFT (0x00000005u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_5_RESETVAL (0x00000000u)
/*----SRC_SET_5 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_5_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_4_MASK (0x00000010u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_4_SHIFT (0x00000004u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_4_RESETVAL (0x00000000u)
/*----SRC_SET_4 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_4_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_3_MASK (0x00000008u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_3_SHIFT (0x00000003u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_3_RESETVAL (0x00000000u)
/*----SRC_SET_3 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_3_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_2_MASK (0x00000004u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_2_SHIFT (0x00000002u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_2_RESETVAL (0x00000000u)
/*----SRC_SET_2 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_2_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_1_MASK (0x00000002u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_1_SHIFT (0x00000001u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_1_RESETVAL (0x00000000u)
/*----SRC_SET_1 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_1_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_0_MASK (0x00000001u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_0_SHIFT (0x00000000u)
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_0_RESETVAL (0x00000000u)
/*----SRC_SET_0 Tokens----*/
#define CSL_KLDINTC_INTC_TEST_SET_SRC_SET_0_SET (0x00000001u)

#define CSL_KLDINTC_INTC_TEST_SET_RESETVAL (0x00000000u)

/* SRC0CFG */


#define CSL_KLDINTC_SRC0CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC0CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC0CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC0CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC0CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC0CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC0CFG_RESETVAL     (0x00000000u)

/* SRC1CFG */


#define CSL_KLDINTC_SRC1CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC1CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC1CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC1CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC1CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC1CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC1CFG_RESETVAL     (0x00000000u)

/* SRC2CFG */


#define CSL_KLDINTC_SRC2CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC2CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC2CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC2CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC2CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC2CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC2CFG_RESETVAL     (0x00000000u)

/* SRC3CFG */


#define CSL_KLDINTC_SRC3CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC3CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC3CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC3CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC3CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC3CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC3CFG_RESETVAL     (0x00000000u)

/* SRC4CFG */


#define CSL_KLDINTC_SRC4CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC4CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC4CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC4CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC4CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC4CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC4CFG_RESETVAL     (0x00000000u)

/* SRC5CFG */


#define CSL_KLDINTC_SRC5CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC5CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC5CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC5CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC5CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC5CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC5CFG_RESETVAL     (0x00000000u)

/* SRC6CFG */


#define CSL_KLDINTC_SRC6CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC6CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC6CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC6CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC6CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC6CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC6CFG_RESETVAL     (0x00000000u)

/* SRC7CFG */


#define CSL_KLDINTC_SRC7CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC7CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC7CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC7CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC7CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC7CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC7CFG_RESETVAL     (0x00000000u)

/* SRC8CFG */


#define CSL_KLDINTC_SRC8CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC8CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC8CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC8CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC8CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC8CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC8CFG_RESETVAL     (0x00000000u)

/* SRC9CFG */


#define CSL_KLDINTC_SRC9CFG_PRI_MASK     (0x03000000u)
#define CSL_KLDINTC_SRC9CFG_PRI_SHIFT    (0x00000018u)
#define CSL_KLDINTC_SRC9CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC9CFG_INTVEC_MASK  (0x00001FFFu)
#define CSL_KLDINTC_SRC9CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC9CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC9CFG_RESETVAL     (0x00000000u)

/* SRC10CFG */


#define CSL_KLDINTC_SRC10CFG_PRI_MASK    (0x03000000u)
#define CSL_KLDINTC_SRC10CFG_PRI_SHIFT   (0x00000018u)
#define CSL_KLDINTC_SRC10CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC10CFG_INTVEC_MASK (0x00001FFFu)
#define CSL_KLDINTC_SRC10CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC10CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC10CFG_RESETVAL    (0x00000000u)

/* SRC11CFG */


#define CSL_KLDINTC_SRC11CFG_PRI_MASK    (0x03000000u)
#define CSL_KLDINTC_SRC11CFG_PRI_SHIFT   (0x00000018u)
#define CSL_KLDINTC_SRC11CFG_PRI_RESETVAL (0x00000000u)


#define CSL_KLDINTC_SRC11CFG_INTVEC_MASK (0x00001FFFu)
#define CSL_KLDINTC_SRC11CFG_INTVEC_SHIFT (0x00000000u)
#define CSL_KLDINTC_SRC11CFG_INTVEC_RESETVAL (0x00000000u)

#define CSL_KLDINTC_SRC11CFG_RESETVAL    (0x00000000u)

#endif