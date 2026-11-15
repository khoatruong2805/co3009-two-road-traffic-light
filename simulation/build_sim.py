"""Build the lab C6 simulation from the same FSM sources. Requires ARM GCC on PATH or --bin."""
from pathlib import Path
import os,sys,subprocess,shutil,argparse
p=argparse.ArgumentParser();p.add_argument('--bin',type=Path);a=p.parse_args()
here=Path(__file__).resolve().parent
root=here.parent/'TrafficRB'; build=here/'build';build.mkdir(exist_ok=True)
target=here/'firmware/traffic_demo.hex';target.parent.mkdir(exist_ok=True)
def tool(n):
    name='arm-none-eabi-'+n
    return str(a.bin/(name+('.exe' if os.name=='nt' else ''))) if a.bin else name
common='global modes button scheduler tasks software_timer traffic_light fsm_automatic fsm_manual fsm_tuning'.split()
files=[root/'Core/Src'/(n+'.c') for n in common]+[here/'sim_main.c',root/'Core/Src/system_stm32f1xx.c',here/'startup_stm32f103x6.s']
args=['-mcpu=cortex-m3','-mthumb','-DSTM32F103x6','-DTRAFFIC_SIM','-Os','-ffunction-sections','-fdata-sections','-I'+str(root/'Core/Inc'),'-I'+str(root/'Drivers/CMSIS/Include'),'-I'+str(root/'Drivers/CMSIS/Device/ST/STM32F1xx/Include')]
elf=build/'traffic.elf'
subprocess.run([tool('gcc'),*args,*map(str,files),'-T'+str(here/'STM32F103C6UX_FLASH.ld'),'-Wl,--gc-sections','--specs=nosys.specs', '-o',str(elf)],check=True)
subprocess.run([tool('objcopy'),'-O','ihex',str(elf),str(target)],check=True)
subprocess.run([tool('size'),str(elf)],check=True)
