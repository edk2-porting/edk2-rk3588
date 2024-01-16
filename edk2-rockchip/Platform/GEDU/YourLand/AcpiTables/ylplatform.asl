#include "AcpiTables.h"

Device (SCM0)
{
    Name (_HID, "GDYLA580")  // _HID: Hardware ID
    Alias (\_SB.PSUB, _SUB)
    Name (_UID, Zero)  // _UID: Unique ID
}
