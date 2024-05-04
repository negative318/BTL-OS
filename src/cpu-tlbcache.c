/*
 * Copyright (C) 2024 pdnguyen of the HCMC University of Technology
 */
/*
 * Source Code License Grant: Authors hereby grants to Licensee
 * a personal to use and modify the Licensed Source Code for
 * the sole purpose of studying during attending the course CO2018.
 */
// #ifdef MM_TLB
/*
 * Memory physical based TLB Cache
 * TLB cache module tlb/tlbcache.c
 *
 * TLB cache is physically memory phy
 * supports random access
 * and runs at high speed
 */

#include "mm.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define GET_TAG(tlb_page) GETVAL(tlb_page, GENMASK(8, 0), 0)
#define GET_PID(tlb_page) GETVAL(tlb_page, GENMASK(29, 9), 9)

#define init_tlbcache(mp, sz, ...) init_memphy(mp, sz, (1, ##__VA_ARGS__))

/*
bit 31: TAG USED
bit 30: VALID
bit 29-9: PID
bit 8-0: TAG

*/

/*
 *  tlb_cache_read read TLB cache device
 *  @mp: memphy struct
 *  @pid: process id
 *  @pgnum: page number
 *  @value: obtained value
 */

int tlb_cache_read(struct memphy_struct *mp, int pid, int pgnum, BYTE *value)
{
   /* TODO: the identify info is mapped to
    *      cache line by employing:
    *      direct mapped, associated mapping etc.
    */
   int index = pgnum % 32;
   int tag = pgnum / 32;
   uint32_t tlb_page = tlb[index][0];
   if (tlb_page & BIT(30))
   {
      int tag_page = GET_TAG(tlb_page);
      int pid_page = GET_PID(tlb_page);
      if (tag == tag_page && pid == pid_page)
      {
         value = tlb[index][1];
         return 0;
      }
   }
   return -1;
}

/*
 *  tlb_cache_write write TLB cache device
 *  @mp: memphy struct
 *  @pid: process id
 *  @pgnum: page number
 *  @value: obtained value
 */
int tlb_cache_write(struct memphy_struct *mp, int pid, int pgnum, BYTE *value)
{
   /* TODO: the identify info is mapped to
    *      cache line by employing:
    *      direct mapped, associated mapping etc.
    */
   int index = pgnum % 32;
   int tag = pgnum / 32;
   uint32_t tlb_page = tlb[index][0];
   if (tlb_page & BIT(30))
   {
      int tag_page = GET_TAG(tlb_page);
      int pid_page = GET_PID(tlb_page);
      if (tag == tag_page && pid == pid_page)
      {
         tlb[index][1] = value;
         return 0;
      }
   }
   return -1;
}

/*
 *  TLBMEMPHY_read natively supports MEMPHY device interfaces
 *  @mp: memphy struct
 *  @addr: address
 *  @value: obtained value
 */
int TLBMEMPHY_read(struct memphy_struct *mp, int addr, BYTE *value)
{
   if (mp == NULL)
      return -1;

   /* TLB cached is random access by native */
   *value = mp->storage[addr];
   return 0;
}

/*
 *  TLBMEMPHY_write natively supports MEMPHY device interfaces
 *  @mp: memphy struct
 *  @addr: address
 *  @data: written data
 */
int TLBMEMPHY_write(struct memphy_struct *mp, int addr, BYTE data)
{
   if (mp == NULL)
      return -1;

   /* TLB cached is random access by native */
   mp->storage[addr] = data;

   return 0;
}

/*
 *  TLBMEMPHY_format natively supports MEMPHY device interfaces
 *  @mp: memphy struct
 */

int TLBMEMPHY_dump(struct memphy_struct *mp)
{
   // in ra các PID, TAG, FPN trong tlb
   /*TODO dump memphy contnt mp->storage
    *     for tracing the memory content
    */
   printf("=============START TLB DUMP=============\n");
   for (int i = 0; i < MAX_TLB; i++)
   {
      if (tlb[i][0] & BIT(30))
      {
         printf("PID:%d ------------- TAG:%d ------------- FPN:%d\n", GET_PID(tlb[i][0]), GET_TAG(tlb[i][0]), tlb[i][1]);
      }
   }
   printf("=============END TLB DUMP=============\n");
   return 0;
}

/*
 *  Init TLBMEMPHY struct
 */
int init_tlbmemphy(struct memphy_struct *mp, int max_size)
{
   mp->storage = (BYTE *)malloc(max_size * sizeof(BYTE));
   for (int i = 0; i < MAX_TLB; i++)
   {
      tlb[i] = (uint32_t *)(mp->storage + i * 8);
      tlb[i][0] = 0; // tag
      tlb[i][1] = 0; // frame number
   }
   mp->maxsz = max_size;

   mp->rdmflg = 1;

   return 0;
}

// #endif
