/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2020 rt-labs AB, Sweden.
 *
 * This software is dual-licensed under GPLv3 and a commercial
 * license. See the file LICENSE.md distributed with this software for
 * full license information.
 ********************************************************************/

/*
 * Note: this file originally auto-generated by mib2c
 * using mib2c.scalar.conf
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#undef LOG_DEBUG
#undef LOG_WARNING
#undef LOG_INFO
#undef LOG_ERROR
#undef LOG_FATAL

#include "lldpLocalSystemData.h"

/** Initializes the lldpLocalSystemData module */
void init_lldpLocalSystemData (pnet_t * pnet)
{
   const oid lldpLocChassisIdSubtype_oid[] = {1, 0, 8802, 1, 1, 2, 1, 3, 1};
   const oid lldpLocChassisId_oid[] = {1, 0, 8802, 1, 1, 2, 1, 3, 2};
   netsnmp_handler_registration * reg;

   netsnmp_register_scalar (
      reg = netsnmp_create_handler_registration (
         "lldpLocChassisIdSubtype",
         handle_lldpLocChassisIdSubtype,
         lldpLocChassisIdSubtype_oid,
         OID_LENGTH (lldpLocChassisIdSubtype_oid),
         HANDLER_CAN_RONLY));

   reg->my_reg_void = pnet;

   netsnmp_register_scalar (
      reg = netsnmp_create_handler_registration (
         "lldpLocChassisId",
         handle_lldpLocChassisId,
         lldpLocChassisId_oid,
         OID_LENGTH (lldpLocChassisId_oid),
         HANDLER_CAN_RONLY));

   reg->my_reg_void = pnet;
}

int handle_lldpLocChassisIdSubtype (
   netsnmp_mib_handler * handler,
   netsnmp_handler_registration * reginfo,
   netsnmp_agent_request_info * reqinfo,
   netsnmp_request_info * requests)
{
   pnet_t * pnet = reginfo->my_reg_void;
   pf_lldp_chassis_id_t chassis_id;

   /* We are never called for a GETNEXT if it's registered as a
      "instance", as it's "magically" handled for us.  */

   /* a instance handler also only hands us one request at a time, so
      we don't need to loop over a list of requests; we'll only get one. */

   pf_snmp_get_chassis_id (pnet, &chassis_id);

   switch (reqinfo->mode)
   {

   case MODE_GET:
      LOG_DEBUG (
         PF_SNMP_LOG,
         "lldpLocalSystemData(%d): GET chassis ID subtype.\n",
         __LINE__);
      snmp_set_var_typed_integer (
         requests->requestvb,
         ASN_INTEGER,
         chassis_id.subtype);
      break;

   default:
      /* we should never get here, so this is a really bad error */
      LOG_ERROR (
         PF_SNMP_LOG,
         "lldpLocalSystemData(%d): unknown mode (%d)\n",
         __LINE__,
         reqinfo->mode);
      return SNMP_ERR_GENERR;
   }

   return SNMP_ERR_NOERROR;
}

int handle_lldpLocChassisId (
   netsnmp_mib_handler * handler,
   netsnmp_handler_registration * reginfo,
   netsnmp_agent_request_info * reqinfo,
   netsnmp_request_info * requests)
{
   pnet_t * pnet = reginfo->my_reg_void;
   pf_lldp_chassis_id_t chassis_id;

   /* We are never called for a GETNEXT if it's registered as a
      "instance", as it's "magically" handled for us.  */

   /* a instance handler also only hands us one request at a time, so
      we don't need to loop over a list of requests; we'll only get one. */

   pf_snmp_get_chassis_id (pnet, &chassis_id);

   switch (reqinfo->mode)
   {

   case MODE_GET:
      LOG_DEBUG (
         PF_SNMP_LOG,
         "lldpLocalSystemData(%d): GET chassis ID.\n",
         __LINE__);

      snmp_set_var_typed_value (
         requests->requestvb,
         ASN_OCTET_STR,
         chassis_id.string,
         chassis_id.len);
      break;

   default:
      /* we should never get here, so this is a really bad error */
      LOG_ERROR (
         PF_SNMP_LOG,
         "lldpLocalSystemData(%d): unknown mode (%d)\n",
         __LINE__,
         reqinfo->mode);
      return SNMP_ERR_GENERR;
   }

   return SNMP_ERR_NOERROR;
}