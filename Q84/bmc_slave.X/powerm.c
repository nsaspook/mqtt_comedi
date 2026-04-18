#include "powerm.h"

void powerm_version(void)
{
	strncpy(em_info, "POWERM Driver      ", 32);
	strncpy(modbus_name [1], "POWERM", 12);
}
