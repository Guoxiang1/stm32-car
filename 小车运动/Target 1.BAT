SET PATH=E:\Keil_v5\ARM\ARMCC\Bin;E:\PADS\9.5PADS\SDD_HOME\common\win32\bin;E:\PADS\9.5PADS\SDD_HOME\common\win32\lib;E:\PADS\9.5PADS\MGC_HOME.ixn\bin;E:\PADS\9.5PADS\MGC_HOME.ixn\lib;C:\Program Files (x86)\STMicroelectronics\st_toolset\asm;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\WINDOWS\System32\OpenSSH\;D:\xshell\;E:\Cadence\SPB_16.6\openaccess\bin\win32\opt;E:\Cadence\SPB_16.6\tools\capture;E:\Cadence\SPB_16.6\tools\pspice;E:\Cadence\SPB_16.6\tools\specctra\bin;E:\Cadence\SPB_16.6\tools\fet\bin;E:\Cadence\SPB_16.6\tools\libutil\bin;E:\Cadence\SPB_16.6\tools\bin;E:\Cadence\SPB_16.6\tools\pcb\bin;C:\Users\Administrator\AppData\Local\Microsoft\WindowsApps;C:\Program Files (x86)\Tencent\QQGameTempest\Hall.57702\
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\main.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_it.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\system_stm32f10x.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\core_cm3.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\misc.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_adc.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_bkp.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_can.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_cec.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_crc.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_dac.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_dbgmcu.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_dma.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_exti.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_flash.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_fsmc.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_gpio.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_i2c.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_iwdg.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_pwr.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_rcc.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_rtc.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_sdio.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_spi.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_tim.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_usart.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\stm32f10x_wwdg.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\delay.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\sys.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\usart.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\objects\startup_stm32f10x_md._ia"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\objects\moto.__i"
"E:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Objects\run.lnp"
"E:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\Objects\run.axf" --i32combined --output ".\Objects\run.hex"
