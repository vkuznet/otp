#include <stdio.h>
#include <dlfcn.h>
#include "erl_driver.h"
#include "global.h"

char extern_symbol = MAGIC;


static ErlDrvData
global_drv_start(ErlDrvPort port, char *buff)
{
    set_port_control_flags(port, PORT_CONTROL_FLAG_BINARY);

    return 0;
}


static ErlDrvSSizeT
global_drv_control(ErlDrvData handle, unsigned int command, char *buf,
		   ErlDrvSizeT len, char **rbuf, ErlDrvSizeT rlen)
{
    if (command == MAGIC) {
	void *handle;

	handle = dlopen(buf, RTLD_NOW);
	if (handle == NULL)
	    *rbuf[0] = 2;
	else {
	    int (*global)(void);

	    global = (int (*)(void))dlsym(handle, "global");
	    if (global != NULL)
		*rbuf[0] = global(); /* will be set to 0 or 1 */
	    else
		*rbuf[0] = 3;

	    dlclose(handle);
	}

	return 1;
    } else
	return 0;
}


static ErlDrvEntry driver_entry = {
    NULL,
    global_drv_start,
    NULL,
    NULL,
    NULL,
    NULL,
    "global_drv",
    NULL,
    NULL,
    global_drv_control,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ERL_DRV_EXTENDED_MARKER,
    ERL_DRV_EXTENDED_MAJOR_VERSION,
    ERL_DRV_EXTENDED_MINOR_VERSION,
    0,
    NULL,
    NULL,
    NULL
};


DRIVER_INIT(global_drv)
{
    return &driver_entry;
}
