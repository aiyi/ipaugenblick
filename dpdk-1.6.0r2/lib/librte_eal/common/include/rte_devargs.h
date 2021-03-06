/*-
 *   BSD LICENSE
 *
 *   Copyright 2014 6WIND S.A.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of 6WIND S.A nor the names of its contributors
 *       may be used to endorse or promote products derived from this
 *       software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _RTE_DEVARGS_H_
#define _RTE_DEVARGS_H_

/**
 * @file
 *
 * RTE devargs: list of devices and their user arguments
 *
 * This file stores a list of devices and their arguments given by
 * the user when a DPDK application is started. These devices can be PCI
 * devices or virtual devices. These devices are stored at startup in a
 * list of rte_devargs structures.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/queue.h>
#include <rte_pci.h>

/**
 * Type of generic device
 */
enum rte_devtype {
	RTE_DEVTYPE_WHITELISTED_PCI,
	RTE_DEVTYPE_BLACKLISTED_PCI,
	RTE_DEVTYPE_VIRTUAL,
};

/**
 * Structure that stores a device given by the user with its arguments
 *
 * A user device is a physical or a virtual device given by the user to
 * the DPDK application at startup through command line arguments.
 *
 * The structure stores the configuration of the device, its PCI
 * identifier if it's a PCI device or the driver name if it's a virtual
 * device.
 */
struct rte_devargs {
	/** Next in list. */
	TAILQ_ENTRY(rte_devargs) next;
	/** Type of device. */
	enum rte_devtype type;
	union {
		/** Used if type is RTE_DEVTYPE_*_PCI. */
		struct {
			/** PCI location. */
			struct rte_pci_addr addr;
		} pci;
		/** Used if type is RTE_DEVTYPE_VIRTUAL. */
		struct {
			/** Driver name. */
			char drv_name[32];
		} virtual;
	};
#define RTE_DEVARGS_LEN 256
	char args[RTE_DEVARGS_LEN]; /**< Arguments string as given by user. */
};

/** user device double-linked queue type definition */
TAILQ_HEAD(rte_devargs_list, rte_devargs);

/** Global list of user devices */
extern struct rte_devargs_list devargs_list;

/**
 * Add a device to the user device list
 *
 * For PCI devices, the format of arguments string is "PCI_ADDR" or
 * "PCI_ADDR,key=val,key2=val2,...". Examples: "08:00.1", "0000:5:00.0",
 * "04:00.0,arg=val".
 *
 * For virtual devices, the format of arguments string is "DRIVER_NAME*"
 * or "DRIVER_NAME*,key=val,key2=val2,...". Examples: "eth_ring",
 * "eth_ring0", "eth_pmdAnything,arg=0:arg2=1". The validity of the
 * driver name is not checked by this function, it is done when probing
 * the drivers.
 *
 * @param devtype
 *   The type of the device.
 * @param devargs_list
 *   The arguments as given by the user.
 *
 * @return
 *   - 0 on success
 *   - A negative value on error
 */
int rte_eal_devargs_add(enum rte_devtype devtype, const char *devargs_str);

/**
 * Count the number of user devices of a specified type
 *
 * @param devtype
 *   The type of the devices to counted.
 *
 * @return
 *   The number of devices.
 */
unsigned int
rte_eal_devargs_type_count(enum rte_devtype devtype);

/**
 * This function dumps the list of user device and their arguments.
 */
void rte_eal_devargs_dump(void);

#ifdef __cplusplus
}
#endif

#endif /* _RTE_DEVARGS_H_ */
