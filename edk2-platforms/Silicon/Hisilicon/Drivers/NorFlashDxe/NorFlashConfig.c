/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/


#include "NorFlashHw.h"


#define COMMAND_TYPE1 0x1
#define COMMAND_TYPE2 0x2
#define COMMAND_TYPE3 0x4
#define COMMAND_TYPE4 0x8
#define COMMAND_TYPE5 0x10


NOR_FLASH_INFO_TABLE gFlashInfo[]=
{

    {//S29GL512m
      0x00010001,
      0x227E227E,
      0x22232223,
      0x22012201,
      1,
      0x20000000,
      0x20000,
      0x0010,
      COMMAND_TYPE1
    },
    {//S29GL1g
      0x00010001,
      0x227E227E,
      0x22282228,
      0x22012201,
      1,
      0x40000000,
      0x20000,
      0x0020,
      COMMAND_TYPE1
    },
    {//M29ew512m
      0x00890089,
      0x227E227E,
      0x22232223,
      0x22012201,
      1,
      0x20000000,
      0x20000,
      0x0010,
      COMMAND_TYPE1
    },
    {//M29EW2g
      0x00890089,
      0x227E227E,
      0x22482248,
      0x22012201,
      1,
      0x80000000,
      0x20000,
      0x0020,
      COMMAND_TYPE1
    },
    {
      0x00890089,
      0x227E227E,
      0x22282228,
      0x22012201,
      1,
      0x10000000,
      0x20000,
      0x0020,
      COMMAND_TYPE1
    },
    {
      0x00890089,
      0x227E227E,
      0x22282228,
      0x22012201,
      2,
      0x10000000,
      0x20000,
      0x0020,
      COMMAND_TYPE1
    }
};



FLASH_COMMAND_RESET gFlashCommandReset[]=
{
    {
       COMMAND_TYPE1,
       (0x00F000F0)
    }

};


FLASH_COMMAND_ID gFlashCommandId[]=
{
     {
       COMMAND_TYPE1,
       (0x0555),
       (0x00AA00AA),
       (0x02AA),
       (0x00550055),
       (0x0555),
       (0x00900090),
       (0x0000),

       (0x0001),
       (0x000E),
       (0x000F)
    }
};


FLASH_COMMAND_WRITE gFlashCommandWrite[]=
{
    {
       COMMAND_TYPE1,
       (0x0555),
       (0x00AA00AA),
       (0x02AA),
       (0x00550055),
       (0x00250025),
       (0x00290029)
    }

};


FLASH_COMMAND_ERASE gFlashCommandErase[]=
{
    {
       COMMAND_TYPE1,
       (0x0555),
       (0x00AA00AA),
       (0x02AA),
       (0x00550055),
       (0x0555),
       (0x00800080),
       (0x0555),
       (0x00AA00AA),
       (0x02AA),
       (0x00550055),
       (0x00300030)
    }

};

