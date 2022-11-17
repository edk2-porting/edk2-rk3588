/** @file
  The Semaphore Library API provides the necessary functions to acquire/release
  the global or a socket's semaphore.

  This API is designed to allow a calling agent to acquire a global (the SBSP)
  semaphore or a socket's semaphore.  It also provides functionality to release
  the semaphore and check if ownership has been obtained.  If a semaphore is
  desired, an agent should first attempt to acquire it, then check if it has
  ownership.  If ownership has not been obtained, the agent must wait until
  ownership has been obtained before proceeding.  Once the desired task is complete
  the semaphore must be released.  Semaphores should be used for when ensuring
  exclusive access to resoruces among CPU sockets is necessary.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SEMAPHORE_LIB_H_
#define _SEMAPHORE_LIB_H_

#include <Uefi.h>

///
/// Used to identify which System Sempahore is being accessed. There are
/// currently two System Semaphores available for use (0 and 1).
///
typedef enum {
  SystemSemaphore0,     ///< Semaphore 0 - Used for SPD/SMBus access
  SystemSemaphore1,     ///< Semaphore 1 - Used for Debug print
  SystemSemaphoreMax
} SYSTEM_SEMAPHORE_NUMBER;

///
/// Used to identify which Local Sempahore is being accessed. There are
/// currently two Local Semaphores available for use (0 and 1).
///
typedef enum {
  LocalSemaphore0,
  LocalSemaphore1,
  LocalSemaphoreMax
} LOCAL_SEMAPHORE_NUMBER;

#define DO_NOT_ADD_TO_QUEUE  FALSE
#define ADD_TO_QUEUE         TRUE

/**
  Acquire a global (BSP) semaphore for the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireGlobalSemaphore (SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsGlobalSemaphoreOwned (SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseGlobalSemaphore (SystemSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)
  @param[in]  AddToQueue        - specifices whether to add the requesting
                                  socket to the queue (TRUE or FALSE)
  @param[out] QueueNumber       - assigned place in line of semaphore request,
                                  if adding to queue

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
AcquireGlobalSemaphore (
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber,
  IN  BOOLEAN                   AddToQueue,
  OUT UINT32                    *QueueNumber      OPTIONAL
  );

/**
  Checks if a global (BSP) semaphore is owned by the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireGlobalSemaphore (SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsGlobalSemaphoreOwned (SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseGlobalSemaphore (SystemSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)
  @param[in]  QueueNumber       - assigned place in line of semaphore request
                                  that was returned by AcquireGlobalSemaphore

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
IsGlobalSemaphoreOwned (
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber,
  IN  UINT32                    QueueNumber
  );

/**
  Release a global (BSP) semaphore owned by the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireGlobalSemaphore (SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsGlobalSemaphoreOwned (SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseGlobalSemaphore (SystemSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)

  @retval EFI_SUCCESS           - successfully released semaphore
  @retval EFI_DEVICE_ERROR      - error releasing semaphore
**/
EFI_STATUS
EFIAPI
ReleaseGlobalSemaphore (
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber
  );


/**
  Acquire a socket semaphore for the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireSocketSemaphore (Socket, SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsSocketSemaphoreOwned (Socket, SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseSocketSemaphore (Socket, SystemSemaphore0);
      ......

  @param[in]  Socket            - Socket where the semaphore is located
  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)
  @param[in]  AddToQueue        - specifices whether to add the requesting
                                  socket to the queue (TRUE or FALSE)
  @param[out] QueueNumber       - assigned place in line of semaphore request,
                                  if adding to queue

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
AcquireSocketSemaphore (
  IN  UINT8                     Socket,
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber,
  IN  BOOLEAN                   AddToQueue,
  OUT UINT32                    *QueueNumber      OPTIONAL
  );

/**
  Checks if a socket semaphore is owned by the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireSocketSemaphore (Socket, SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsSocketSemaphoreOwned (Socket, SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseSocketSemaphore (Socket, SystemSemaphore0);
      ......

  @param[in]  Socket            - Socket where the semaphore is located
  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)
  @param[in]  QueueNumber       - assigned place in line of semaphore request
                                  that was returned by AcquireSocketSemaphore

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
IsSocketSemaphoreOwned (
  IN  UINT8                     Socket,
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber,
  IN  UINT32                    QueueNumber
  );

/**
  Release a socket semaphore owned by the calling socket.

  Used for ensuring exclusive access to resources among CPU sockets.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireSocketSemaphore (Socket, SystemSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsSocketSemaphoreOwned (Socket, SystemSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseSocketSemaphore (Socket, SystemSemaphore0);
      ......

  @param[in]  Socket            - Socket to release semaphore
  @param[in]  SemaphoreNumber   - SYSTEMSEMAPHORE register number (0 or 1)

  @retval EFI_SUCCESS           - successfully released semaphore
  @retval EFI_DEVICE_ERROR      - error releasing semaphore
**/
EFI_STATUS
EFIAPI
ReleaseSocketSemaphore (
  IN  UINT8                     Socket,
  IN  SYSTEM_SEMAPHORE_NUMBER   SemaphoreNumber
  );

/**
  Acquire a local semaphore for the calling thread.

  Used for ensuring exclusive access to resources among CPU threads.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireLocalSemaphore (LocalSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsLocalSemaphoreOwned (LocalSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseLocalSemaphore (LocalSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - LOCALSEMAPHORE register number (0 or 1)
  @param[in]  AddToQueue        - specifices whether to add the requesting
                                  thread to the queue (TRUE or FALSE)
  @param[out] QueueNumber       - assigned place in line of semaphore request,
                                  if adding to queue

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
AcquireLocalSemaphore (
  IN  LOCAL_SEMAPHORE_NUMBER    SemaphoreNumber,
  IN  BOOLEAN                   AddToQueue,
  OUT UINT32                    *QueueNumber      OPTIONAL
  );

/**
  Checks if a local semaphore is owned by the calling thread.

  Used for ensuring exclusive access to resources among CPU threads.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireLocalSemaphore (LocalSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsLocalSemaphoreOwned (LocalSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseLocalSemaphore (LocalSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - LOCALSEMAPHORE register number (0 or 1)
  @param[in]  QueueNumber       - assigned place in line of semaphore request
                                  that was returned by AcquireLocalSemaphore

  @retval TRUE                  - successfully acquired semaphore
  @retval FALSE                 - semaphore not acquired
**/
BOOLEAN
EFIAPI
IsLocalSemaphoreOwned (
  IN  LOCAL_SEMAPHORE_NUMBER    SemaphoreNumber,
  IN  UINT32                    QueueNumber
  );

/**
  Release a local semaphore owned by the calling thread.

  Used for ensuring exclusive access to resources among CPU threads.

  IMPORTANT:
    The functions must be called in the sequence below:
      ......
      Owned = AcquireLocalSemaphore (LocalSemaphore0, ADD_TO_QUEUE, &QNum);
      while (!Owned) {
        Owned = IsLocalSemaphoreOwned (LocalSemaphore0, QNum));
      }
      DoSomething ();
      ReleaseLocalSemaphore (LocalSemaphore0);
      ......

  @param[in]  SemaphoreNumber   - LOCALSEMAPHORE register number (0 or 1)

  @retval EFI_SUCCESS           - successfully released semaphore
  @retval EFI_INVALID_PARAMETER - semaphore number is out of range
  @retval EFI_DEVICE_ERROR      - error releasing semaphore
**/
EFI_STATUS
EFIAPI
ReleaseLocalSemaphore (
  IN  LOCAL_SEMAPHORE_NUMBER    SemaphoreNumber
  );

#endif  // _SEMAPHORE_LIB_H_
