#ifndef _dumperc_h_
#define _dumperc_h_

#include "xtcp_client.h"

#define DUMP_SERVER_PORT 8008

void dumper_init(void);

void xdumperd(chanend tcpip_svr,
            chanend led_svr);


void dumper_handle_event(chanend tcp_svr,
                        REFERENCE_PARAM(xtcp_connection_t, conn),
                        chanend led_svr);

#endif // _dumper_h_
