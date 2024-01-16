/*
 * Event Trace for UEFI
 * 
 * Copyright 2023 GEDU Tech. 
 */
#include <stdint.h>

#define ETU_VER_MAJOR 0
#define ETU_VER_MINOR 1

#define ETU_MAGIC 0x47454455
#define ETU_FACILITY_ETM  0xEDCB0000
// please add other UEFI components IDs here


typedef struct _ETU_HDR {
    uint32_t Magic; // 标识码，固定为‘GETU’的ASCII代码 
    uint16_t VerMajor; // 主版本号
    uint16_t VerMinor;
    uint16_t SizeOfEventMark;
    uint16_t Reserved1;
    uint32_t Reserved2;
} ETU_HDR; // total 16 bytes

#define ETU_MAX_MSG 16
typedef struct _ETU_MARK {
    uint32_t SequenceID; // 打点的序号
    uint32_t FacilityID; // 事件源的ID
    uint64_t TimeStamp;  // 事件戳
    char     ShortMessage[ETU_MAX_MSG]; // 简短的文字消息，最大16个字符，超长截断
    uint32_t Paras[4];   // 事件的参数
} ETU_MARK;

// intefaces
int EtuInit(int nMaxMarks);
int EtuMark(uint32_t FacilityID, const char* Msg, uint32_t P1, uint32_t P2, uint32_t P3, uint32_t P4);
int EtuGetTotalMarks();
int EtuReadMark(uint32_t SequenceID, ETU_MARK* pMark);

typedef struct _ETU_MGR {
    uint32_t MarkCounter; // 打点的数量
    uint32_t MaxMarks;  // 可容纳的最大打点数量   
    void*    MemBase;     // ETU内存空间的基地址
    void*    MemCursor;   // 游标指针，指向可用的内存区
} ETU_MGR;

#define ETU_ERROR_BASE 0xffee0000
#define E_ETU_OUTOFMEMORY ETU_ERROR_BASE + 1 