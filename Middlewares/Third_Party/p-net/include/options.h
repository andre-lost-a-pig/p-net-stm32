/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2018 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

#ifndef OPTIONS_H
#define OPTIONS_H

/**
 * # Profinet Stack Options
 *
 * The defines named `PNET_OPTION_*` may be used to extend or reduce
 * the functionality of the stack. Setting these values to 0 excludes
 * the specific function and may also reduce the memory usage of the
 * Profinet stack.
 *
 * Note that none of these options are currently supported (even if
 * enabled by setting the value to 1), except for parsing the RPC
 * Connect request message and generating the connect RPC Connect
 * response message.
 */

#if !defined (PNET_OPTION_FAST_STARTUP)
#define PNET_OPTION_FAST_STARTUP 1
#endif

#if !defined (PNET_OPTION_PARAMETER_SERVER)
#define PNET_OPTION_PARAMETER_SERVER 1
#endif

#if !defined (PNET_OPTION_IR)
#define PNET_OPTION_IR 1
#endif

#if !defined (PNET_OPTION_SR)
#define PNET_OPTION_SR 1
#endif

#if !defined (PNET_OPTION_REDUNDANCY)
#define PNET_OPTION_REDUNDANCY 1
#endif

#if !defined (PNET_OPTION_AR_VENDOR_BLOCKS)
#define PNET_OPTION_AR_VENDOR_BLOCKS 1
#endif

#if !defined (PNET_OPTION_RS)
#define PNET_OPTION_RS 1
#endif

#if !defined (PNET_OPTION_MC_CR)
#define PNET_OPTION_MC_CR 1
#endif

#if !defined (PNET_OPTION_SRL)
#define PNET_OPTION_SRL 0
#endif

#if !defined (PNET_OPTION_SNMP)
#define PNET_OPTION_SNMP 0
#endif

/**
 * Disable use of atomic operations (stdatomic.h).
 * If the compiler supports it then set this define to 1.
 */
/* TODO: compiler abstraction should be handled by cc.h */
#if !defined (PNET_USE_ATOMICS)
#define PNET_USE_ATOMICS 0
#endif

/**
 * # Memory Usage
 *
 * Memory usage is controlled by the `PNET_MAX_*` defines. Define the
 * required number of supported items.
 *
 * These values directly affect the memory usage of the
 * implementation. Sometimes in complicated ways.
 *
 * Please note that some defines have minimum requirements.
 * Only a few of the defines are validated.
 */

#if !defined (PNET_MAX_AR)
/** Number of connections. Must be > 0. "Automated RT Tester" uses 2 */
#define PNET_MAX_AR               2
#endif

#if !defined (PNET_MAX_API)
/** Number of Application Processes. Must be > 0. */
#define PNET_MAX_API              1
#endif

#if !defined (PNET_MAX_CR)
/** Per AR. 1 input and 1 output. */
#define PNET_MAX_CR               2
#endif

#if !defined (PNET_MAX_SLOTS)
/** Per API. Should be > 1 to allow at least one I/O module. */
#define PNET_MAX_SLOTS          5
#endif

#if !defined (PNET_MAX_SUBSLOTS)
/** Per slot (3 needed for DAP). */
#define PNET_MAX_SUBSLOTS       4
#endif

#if !defined (PNET_MAX_DFP_IOCR)
/** Allowed values are 0 (zero) or 2. */
#define PNET_MAX_DFP_IOCR         2
#endif

#if !defined (PNET_MAX_LOG_BOOK_ENTRIES)
#define PNET_MAX_LOG_BOOK_ENTRIES 16
#endif

#if !defined (PNET_MAX_ALARMS)
/** Per AR and queue. One queue for hi and one for lo alarms. */
#define PNET_MAX_ALARMS           6
#endif

#if !defined (PNET_MAX_ALARM_PAYLOAD_DATA_SIZE)
/** Max size of alarm payload */
#define PNET_MAX_ALARM_PAYLOAD_DATA_SIZE    28
#endif

#if !defined (PNET_MAX_DIAG_ITEMS)
/** Total, per device. Max is 65534 items. */
#define PNET_MAX_DIAG_ITEMS       200
#endif

#if !defined (PNET_MAX_DIAG_MANUF_DATA_SIZE)
/** Max size of manufacturer specific diagnosis data */
#define PNET_MAX_DIAG_MANUF_DATA_SIZE    16
#endif

#if PNET_OPTION_MC_CR

#if !defined (PNET_MAX_MC_CR)
/**< Per AR. */
#define PNET_MAX_MC_CR            1
#endif

#endif  /* PNET_OPTION_MC_CR */

#if PNET_OPTION_AR_VENDOR_BLOCKS

#if !defined (PNET_MAX_AR_VENDOR_BLOCKS)
/**< Must be > 0 */
#define PNET_MAX_AR_VENDOR_BLOCKS 1
#endif

#if !defined (PNET_MAX_AR_VENDOR_BLOCK_DATA_LENGTH)
#define PNET_MAX_AR_VENDOR_BLOCK_DATA_LENGTH 512
#endif

#endif  /* PNET_OPTION_AR_VENDOR_BLOCKS */

#if !defined (PNET_MAX_MAN_SPECIFIC_FAST_STARTUP_DATA_LENGTH)
/** or 512 (bytes) */
#define PNET_MAX_MAN_SPECIFIC_FAST_STARTUP_DATA_LENGTH 0
#endif

#if !defined (PNET_MAX_SESSION_BUFFER_SIZE)
/** Max fragmented RPC request/response length */
#define PNET_MAX_SESSION_BUFFER_SIZE 4500
#endif

#if !defined (PNET_MAX_FILENAME_SIZE)
/** Max filename size, including termination  */
#define PNET_MAX_FILENAME_SIZE 30
#endif

#if !defined (PNET_MAX_PORT_DESCRIPTION_SIZE)
/** Max port description size, including termination  */
#define PNET_MAX_PORT_DESCRIPTION_SIZE 60
#endif


/**
 * # Logging
 *
 * The following options controlling logging.
 */

#ifndef LOG_LEVEL
#define LOG_LEVEL               (LOG_LEVEL_DEBUG)
#endif

#ifndef PF_ETH_LOG
#define PF_ETH_LOG              (LOG_STATE_ON)
#endif

#ifndef PF_LLDP_LOG
#define PF_LLDP_LOG             (LOG_STATE_ON)
#endif

#ifndef PF_SNMP_LOG
#define PF_SNMP_LOG             (LOG_STATE_ON)
#endif

#ifndef PF_CPM_LOG
#define PF_CPM_LOG              (LOG_STATE_ON)
#endif

#ifndef PF_PPM_LOG
#define PF_PPM_LOG              (LOG_STATE_ON)
#endif

#ifndef PF_DCP_LOG
#define PF_DCP_LOG              (LOG_STATE_ON)
#endif

#ifndef PF_RPC_LOG
#define PF_RPC_LOG              (LOG_STATE_ON)
#endif

#ifndef PF_ALARM_LOG
#define PF_ALARM_LOG            (LOG_STATE_ON)
#endif

#ifndef PF_AL_BUF_LOG
#define PF_AL_BUF_LOG           (LOG_STATE_ON)
#endif

#ifndef PF_PNAL_LOG
#define PF_PNAL_LOG             (LOG_STATE_ON)
#endif

#ifndef PNET_LOG
#define PNET_LOG                (LOG_STATE_ON)
#endif

#endif  /* OPTIONS_H */
