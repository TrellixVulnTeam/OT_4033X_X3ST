
#include "typedefs.h"
#include "platform.h"

#include "mt6572.h"
#include "mtk_pll.h"
#include "mtk_timer.h"
#include "custom_emi.h"

kal_uint32 mtk_get_bus_freq(void)
{
    kal_uint32 bus_clk = 26000;

    #if !CFG_FPGA_PLATFORM

    /* For MT6572, check CLK_MUX_SEL(CLK_SEL_0) to get such information */

    kal_uint32 mainpll_con1 = 0, main_diff = 0;
    kal_uint32 clk_sel = 0, pre_div = 1, post_div = 0, vco_div = 1;
    float      n_info = 0, output_freq = 0;

    clk_sel = DRV_Reg32(CLK_SEL_0);

    mainpll_con1 = DRV_Reg32(MAINPLL_CON1);

    post_div = (mainpll_con1 >> 24) & 0x07;
    if(post_div > 0x100)
    {
        output_freq = 0;
    }
    else
    {
        post_div = 0x1 << post_div;
        n_info = (mainpll_con1 & 0x3FFF);
        n_info /= (0x1 << 14);
        n_info += ((mainpll_con1 >> 14) & 0x7F);
        output_freq = 26*n_info*vco_div/pre_div/post_div; //Mhz

        clk_sel = (clk_sel >> 5) & 0x7;
        if( 0x2 == clk_sel )
        {
            bus_clk = output_freq*1000/10;
        }
        else if( 0x4 == clk_sel )
        {
            bus_clk = output_freq*1000/12;
        }
        else
        {
            bus_clk = 26*1000;
        }
    }

    #endif

    return bus_clk; // Khz
}

void mtk_pll_post_init(void)
{
    return;
}

void mtk_pll_init(void)
{
#if !(CFG_FPGA_PLATFORM)

    kal_uint32 reg_val = 0;

    if (reg_val = (DRV_Reg32(0x10009100) >> 4) & 0xF)
    {
        reg_val = DRV_Reg32(0x10205600) & 0xFFFF0FFF | (reg_val << 12);
        DRV_WriteReg32(0x10205600, reg_val);
    }

    if (reg_val = (DRV_Reg32(0x10009100) >> 0) & 0xF)
    {
        reg_val = DRV_Reg32(0x10205600) & 0xFFFFF0FF | (reg_val << 8);
        DRV_WriteReg32(0x10205600, reg_val);
    }

    /* Diable bypass delay */
    reg_val = DRV_Reg32(AP_PLL_CON3);
    reg_val &= 0xFFFFFC00;
    DRV_WriteReg32(AP_PLL_CON3, reg_val);

    reg_val = DRV_Reg32(AP_PLL_CON3);
    reg_val |= 0x80;
    DRV_WriteReg32(AP_PLL_CON3, reg_val);

    /* check if SRAM safe mode is enabled */
    if (0) // (EFUSE_PLL_Safe_IsEnabled()) // TODO: add EFUSE check API for preloader
    {
        DRV_WriteReg32(ARMPLL_CON1, 0x0109A000);
    }
    else
    {
        DRV_WriteReg32(ARMPLL_CON1, 0x01114000);
    }

    #if defined(__EMI_CLK_333MHZ__)
    /* adjust main pll frequency for EMI @ 667Mhz */

    DRV_WriteReg32(CLK_SEL_0, 0x08603022); // rg_axibus_gfmux_sel: xtal

    reg_val = 0x000CC000 | ((DRV_Reg32(MAINPLL_CON0) & 1) << 31);

    DRV_WriteReg32(MAINPLL_CON1, reg_val);
    #endif /* __EMI_CLK_333MHZ__ */

    /* switch to HW mode */
    reg_val = DRV_Reg32(AP_PLL_CON1);
    reg_val &= 0xF8F8CF8C;
    DRV_WriteReg32(AP_PLL_CON1, reg_val);

    /* wait for 1ms */
    gpt_busy_wait_us(1000);

    /* CPU clock */
    DRV_WriteReg32(ACLKEN_DIV, 0x12); // CPU bus clock is MCU clock /2
    DRV_WriteReg32(PCLKEN_DIV, 0x15); // CPU debug APB bus clock is MCU clokc /5

#if !defined(CFG_MEM_PRESERVED_MODE)
    /* EMI and bus clock */
    #if defined(__EMI_CLK_266MHZ__)
    DRV_WriteReg32(CLK_SWCG_3, 0x80000000);
    DRV_WriteReg32(CLK_SEL_0, 0x10000492); // rg_axibus_gfmux_sel: main pll/12
                                           // rg_emi2x_gfmux_sel : main pll/3
    #elif defined(__EMI_CLK_200MHZ__)
    DRV_WriteReg32(CLK_SWCG_3, 0x80000000);
    DRV_WriteReg32(CLK_SEL_0, 0x10000494); // rg_axibus_gfmux_sel: main pll/12
                                           // rg_emi2x_gfmux_sel : main pll/4
    #elif defined(__EMI_CLK_333MHZ__)
    DRV_WriteReg32(CLK_SWCG_3, 0x80000000);
    DRV_WriteReg32(CLK_SEL_0, 0x10000458); // rg_axibus_gfmux_sel: main pll/10
                                           // rg_emi2x_gfmux_sel : main pll/2
    #endif
#else
    /* for memory preserved mode */
    // do not init pll/emi in memory preserved mode, due to code is located in EMI
    // set all pll except EMI
    /* EMI and bus clock */
    #if defined(__EMI_CLK_266MHZ__)
    DRV_WriteReg32(CLK_SWCG_3, 0x80000000);
    DRV_WriteReg32(CLK_SEL_0, 0x10000482); // rg_axibus_gfmux_sel: main pll/12
                                           // rg_emi2x_gfmux_sel : main pll/3
    #elif defined(__EMI_CLK_200MHZ__)
    DRV_WriteReg32(CLK_SWCG_3, 0x80000000);
    DRV_WriteReg32(CLK_SEL_0, 0x10000484); // rg_axibus_gfmux_sel: main pll/12
                                           // rg_emi2x_gfmux_sel : main pll/4
    #elif defined(__EMI_CLK_333MHZ__)
    DRV_WriteReg32(CLK_SWCG_3, 0x80000000);
    DRV_WriteReg32(CLK_SEL_0, 0x10000448); // rg_axibus_gfmux_sel: main pll/10
                                           // rg_emi2x_gfmux_sel : main pll/2
    #endif

#endif

    /* clock switch - switch AP MCU clock */
    reg_val = DRV_Reg32(INFRA_TOPCKGEN_CKMUXSEL);
    reg_val |= 0x4;
    DRV_WriteReg32(INFRA_TOPCKGEN_CKMUXSEL, reg_val);

#endif // !defined(CFG_FPGA_PLATFORM)
}

kal_uint32 mtk_get_uart_clock(pll_uart_id uart_num)
{
    #if !CFG_FPGA_PLATFORM

    kal_uint32 reg_val = 0;
    if(uart_num >= NUM_OF_UART)
    {
        return 0;
    }
    else
    {
        reg_val = DRV_Reg32(CLK_SEL_0);
        if(PLL_UART0 == uart_num)
        {
            if(reg_val & 0x1)
            {
                return 52*1000;
            }
            else
            {
                return 26*1000;
            }
        }
        else if(PLL_UART0 == uart_num)
        {
            if(reg_val & 0x80000)
            {
                return 52*1000;
            }
            else
            {
                return 26*1000;
            }
        }
    }
    #else
    return 26*1000;
    #endif /* !CFG_FPGA_PLATFORM */
}

