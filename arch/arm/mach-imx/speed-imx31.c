/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/clock.h>
#include <init.h>

ulong imx_get_mpl_dpdgck_clk(void)
{
	ulong infreq;

	if ((__REG(CCM_CCMR) & CCMR_PRCS_MASK) == CCMR_FPM)
		infreq = CONFIG_MX31_CLK32 * 1024;
	else
		infreq = CONFIG_MX31_HCLK_FREQ;

	return imx_decode_pll(__REG(CCM_MPCTL), infreq);
}

ulong imx_get_mcu_main_clk(void)
{
	/* For now we assume mpl_dpdgck_clk == mcu_main_clk
	 * which should be correct for most boards
	 */
	return imx_get_mpl_dpdgck_clk();
}

ulong imx_get_perclk1(void)
{
	u32 freq = imx_get_mcu_main_clk();
	u32 pdr0 = __REG(CCM_PDR0);

	freq /= ((pdr0 >> 3) & 0x7) + 1;
	freq /= ((pdr0 >> 6) & 0x3) + 1;

	return freq;
}

int imx_dump_clocks(void)
{
	ulong cpufreq = imx_get_mcu_main_clk();
	printf("mx31 cpu clock: %dMHz\n",cpufreq / 1000000);
	printf("ipg clock     : %dHz\n", imx_get_perclk1());

	return 0;
}

late_initcall(imx_dump_clocks);