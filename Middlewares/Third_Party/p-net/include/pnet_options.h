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

#ifndef PNET_OPTIONS_H
#define PNET_OPTIONS_H

#if !defined (PNET_MAX_PHYSICAL_PORTS)
/** Max number of physical ports */
#define PNET_MAX_PHYSICAL_PORTS 2
#endif

#if !defined (PNET_MAX_DIRECTORYPATH_SIZE)
/** Max directory path size, including termination */
#define PNET_MAX_DIRECTORYPATH_SIZE 240
#endif

#if !defined (PNET_OPTION_DRIVER_ENABLE)
#define PNET_OPTION_DRIVER_ENABLE 0
#endif

#endif  /* PNET_OPTIONS_H */
