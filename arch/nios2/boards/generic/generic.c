#include <common.h>
#include <init.h>
#include <driver.h>
#include <partition.h>
#include <fs.h>

static int phy_address = 1;

static struct resource mac_resources[] = {
	[0] = {
		.start	= NIOS_SOPC_TSE_BASE,
		.size	= 0x400,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= NIOS_SOPC_SGDMA_RX_BASE,
		.size	= 0x40,
		.flags	= IORESOURCE_MEM,
	},
	[2] = {
		.start	= NIOS_SOPC_SGDMA_TX_BASE,
		.size	= 0x40,
		.flags	= IORESOURCE_MEM,
	},
};

static struct device_d mac_dev = {
	.id            = -1,
	.name          = "altera_tse",
	.num_resources = ARRAY_SIZE(mac_resources),
	.resource      = mac_resources,
	.platform_data = &phy_address,
};

static int generic_devices_init(void)
{
	add_cfi_flash_device(-1, NIOS_SOPC_FLASH_BASE, NIOS_SOPC_FLASH_SIZE, 0);
	add_mem_device("ram0", NIOS_SOPC_MEMORY_BASE, NIOS_SOPC_MEMORY_SIZE,
		       IORESOURCE_MEM_WRITEABLE);
	register_device(&mac_dev);
	/*register_device(&epcs_flash_device);*/

	devfs_add_partition("nor0", 0x00000, 0x40000, PARTITION_FIXED, "self0");
	devfs_add_partition("nor0", 0x40000, 0x20000, PARTITION_FIXED, "env0");

	protect_file("/dev/env0", 1);

	return 0;
}

device_initcall(generic_devices_init);


static int altera_console_init(void)
{
	add_generic_device("altera_serial", -1, NULL, NIOS_SOPC_UART_BASE, 0x20,
			   IORESOURCE_MEM, NULL);

	return 0;
}

console_initcall(altera_console_init);
