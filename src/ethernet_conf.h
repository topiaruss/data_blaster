/**
 * Module:  app_xc2_firmware
 * Version: 1v3
 * Build:   ceb87a043f18842a34b85935baf3f2a402246dbd
 * File:    ethernet_conf.h
 *
 * The copyrights, all other intellectual and industrial 
 * property rights are retained by XMOS and/or its licensors. 
 * Terms and conditions covering the use of this code can
 * be found in the Xmos End User License Agreement.
 *
 * Copyright XMOS Ltd 2009
 *
 * In the case where this code is a modification of existing code
 * under a separate license, the separate license terms are shown
 * below. The modifications to the code are still covered by the 
 * copyright notice above.
 *
 **/                                   
// The maximum ip payload size we can use is 500 bytes, so
// there is no point accepted larger packets, this allows
// us to have more buffers in total
#define MAX_ETHERNET_PACKET_SIZE (1514)

#define NUM_MII_RX_BUF 6
#define NUM_MII_TX_BUF 4

#define MAX_ETHERNET_CLIENTS   (1)    




