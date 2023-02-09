/**
 * @file
 * Management Information Base II (RFC1213) SYSTEM objects and functions.
 */

/*
 * Copyright (c) 2006 Axon Digital Design B.V., The Netherlands.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Dirk Ziegelmeier <dziegel@gmx.de>
 *         Christiaan Simons <christiaan.simons@axon.tv>
 */

#include "lwip/snmp.h"
#include "lwip/apps/snmp.h"
#include "lwip/apps/snmp_core.h"
#include "lwip/apps/snmp_mib2.h"
#include "lwip/apps/snmp_table.h"
#include "lwip/apps/snmp_scalar.h"
#include "lwip/sys.h"

#include <string.h>

#if LWIP_SNMP && SNMP_LWIP_MIB2

#if SNMP_USE_NETCONN
#define SYNC_NODE_NAME(node_name) node_name ## _synced
#define CREATE_LWIP_SYNC_NODE(oid, node_name) \
   static const struct snmp_threadsync_node node_name ## _synced = SNMP_CREATE_THREAD_SYNC_NODE(oid, &node_name.node, &snmp_mib2_lwip_locks);
#else
#define SYNC_NODE_NAME(node_name) node_name
#define CREATE_LWIP_SYNC_NODE(oid, node_name)
#endif

/* --- system .1.3.6.1.2.1.1 ----------------------------------------------------- */

static snmp_mib2_get_callback_fct snmp_mib2_system_get;
static snmp_mib2_test_set_callback_fct snmp_mib2_system_test_set;
static snmp_mib2_set_callback_fct snmp_mib2_system_set;

void snmp_mib2_system_set_callbacks(
  snmp_mib2_get_callback_fct get,
  snmp_mib2_test_set_callback_fct test_set,
  snmp_mib2_set_callback_fct set)
{
  snmp_mib2_system_get = get;
  snmp_mib2_system_test_set = test_set;
  snmp_mib2_system_set = set;
}


static s16_t
system_get_value(const struct snmp_scalar_array_node_def *node, void *value)
{
  if (snmp_mib2_system_get == NULL)
  {
    return SNMP_ERR_GENERROR;
  }
  else
  {
    return snmp_mib2_system_get(node->oid, value, SNMP_MAX_VALUE_SIZE);
  }
}

static snmp_err_t
system_set_test(const struct snmp_scalar_array_node_def *node, u16_t len, void *value)
{
  if (snmp_mib2_system_test_set == NULL)
  {
    return SNMP_ERR_GENERROR;
  }
  else
  {
    return snmp_mib2_system_test_set(node->oid, value, len);
  }
}

static snmp_err_t
system_set_value(const struct snmp_scalar_array_node_def *node, u16_t len, void *value)
{
  if (snmp_mib2_system_set == NULL)
  {
    return SNMP_ERR_GENERROR;
  }
  else
  {
    return snmp_mib2_system_set(node->oid, value, len);
  }
}

static const struct snmp_scalar_array_node_def system_nodes[] = {
  {1, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_ONLY},  /* sysDescr */
  {2, SNMP_ASN1_TYPE_OBJECT_ID,    SNMP_NODE_INSTANCE_READ_ONLY},  /* sysObjectID */
  {3, SNMP_ASN1_TYPE_TIMETICKS,    SNMP_NODE_INSTANCE_READ_ONLY},  /* sysUpTime */
  {4, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_WRITE}, /* sysContact */
  {5, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_WRITE}, /* sysName */
  {6, SNMP_ASN1_TYPE_OCTET_STRING, SNMP_NODE_INSTANCE_READ_WRITE}, /* sysLocation */
  {7, SNMP_ASN1_TYPE_INTEGER,      SNMP_NODE_INSTANCE_READ_ONLY}   /* sysServices */
};

const struct snmp_scalar_array_node snmp_mib2_system_node = SNMP_SCALAR_CREATE_ARRAY_NODE(1, system_nodes, system_get_value, system_set_test, system_set_value);

#endif /* LWIP_SNMP && SNMP_LWIP_MIB2 */
