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

#ifndef PNET_VERSION_H
#define PNET_VERSION_H

#define PROFINET_GIT_REVISION "v0.2.0-11-g05929f0-dirty"

#if !defined(PNET_VERSION_BUILD) && defined(PROFINET_GIT_REVISION)
#define PNET_VERSION_BUILD PROFINET_GIT_REVISION
#endif

/* clang-format-off */

#define PNET_VERSION_MAJOR 0
#define PNET_VERSION_MINOR 2
#define PNET_VERSION_PATCH 0

#if defined(PNET_VERSION_BUILD)
#define PNET_VERSION \
   "0.2.0+" PNET_VERSION_BUILD
#else
#define PNET_VERSION \
   "0.2.0"
#endif

/* clang-format-on */

#endif /* PNET_VERSION_H */
