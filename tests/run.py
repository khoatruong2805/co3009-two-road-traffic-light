"""Run on a host with GCC (or set CC). Does not require a board."""
from pathlib import Path
import os,subprocess,tempfile
root=Path(__file__).resolve().parents[1]; app=root/'TrafficRB/Core'
mods='scheduler global software_timer traffic_light button modes fsm_automatic fsm_manual fsm_tuning tasks dht20 lcd'.split()
with tempfile.TemporaryDirectory() as tmp:
 exe=Path(tmp)/('controller.exe' if os.name=='nt' else 'controller')
 subprocess.run([os.environ.get('CC','gcc'),'-std=c11','-Wall','-Wextra','-Werror','-DHOST_TEST','-I'+str(app/'Inc'),str(root/'tests/test_controller.c'),*[str(app/'Src'/(m+'.c')) for m in mods],'-o',str(exe)],check=True)
 subprocess.run([exe],check=True)
