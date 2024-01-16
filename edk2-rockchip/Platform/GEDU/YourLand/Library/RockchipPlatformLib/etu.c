/*
 * Event Trace for UEFI
 * 
 * Copyright 2023 GEDU Tech. 
 */
#include <string.h>
#include "etu.h"
#include <Library/DebugLib.h>

ETU_MGR etm;

#define ETU_SRAM_BASE  0x0000000000FF0A0000ULL
#define ETU_SRAM_SIZE  0x10000 // 64KB
/* 
Note: IRAM for NanoCode Debugger
GDK8_IRAM_BASE 0xff090000
GDK8_IRAM_SIZE 0x9000
GDK8_NDB_NOTE_OFFSET 0x10
*/
//char etu_buffer[0x1000];

int EtuInit(int nMaxMarks)
{
    ETU_HDR * hdr;
    // IRAM 
    etm.MemBase = (void*)ETU_SRAM_BASE; // etu_buffer;
    hdr = (ETU_HDR*)etm.MemBase;
    hdr->Magic = ETU_MAGIC;
    hdr->VerMajor = ETU_VER_MAJOR;
    hdr->VerMinor = ETU_VER_MINOR;
    hdr->SizeOfEventMark = sizeof(ETU_MARK);

    etm.MaxMarks = (ETU_SRAM_SIZE- sizeof(ETU_HDR))/sizeof(ETU_MARK);
    etm.MarkCounter = 0;
    etm.MemCursor = etm.MemBase + sizeof(ETU_HDR);

    EtuMark(ETU_FACILITY_ETM, "UEFI-4-YOURLAND!", ETU_SRAM_BASE, 
        etm.MaxMarks, 0, sizeof(etm));

    return 0;
}

static char* gd_strncpy(char *dest, const char* src, int dst_size)
{
    int i;
    for(i = 0; i < dst_size; i++){
        *dest++ = *src++;
        if(*src == 0)
            break;
    }
    return dest;
}

int EtuMark(uint32_t FacilityID, const char* Msg, uint32_t P1, uint32_t P2, uint32_t P3, uint32_t P4)
{
    ETU_MARK * pPoint;
    int n = 0;
    if(etm.MarkCounter >= etm.MaxMarks)
        return E_ETU_OUTOFMEMORY; 
    etm.MarkCounter++;
    pPoint = (ETU_MARK*)etm.MemCursor;

#ifdef __amd_x64__
    pPoint->TimeStamp = __rdtsc(); // GetTickCount64();
#else
	int64_t virtual_timer_value;
	asm volatile("mrs %0, cntvct_el0" : "=r"(virtual_timer_value));
	pPoint->TimeStamp = virtual_timer_value;
#endif
    pPoint->SequenceID = etm.MarkCounter;
    pPoint->FacilityID = FacilityID;
    if(Msg != NULL) {
        gd_strncpy(pPoint->ShortMessage, Msg, ETU_MAX_MSG);
    }

    pPoint->Paras[0] = P1;
    pPoint->Paras[1] = P2;
    pPoint->Paras[2] = P3;
    pPoint->Paras[3] = P4;
    etm.MemCursor += sizeof(ETU_MARK);

    return 0; 
}

int EtuGetTotalMarks()
{
    return etm.MarkCounter;
}

int EtuReadMark(uint32_t SequenceID, ETU_MARK* pMark)
{
    // TODO
    if (SequenceID >= etm.MarkCounter) {
        return -1; 
    }
    char* pReadPos = (char*)etm.MemBase + SequenceID * sizeof(ETU_MARK);
    memcpy(pMark, pReadPos, sizeof(ETU_MARK));
    return 0;
}