# CPU&NUMA

1. numa node path
    Core per socket, physic core

    1. /sys/devices/system/node/node{socket_id}/cpu
    2. {logic_core_id}/topology/core_id

2. cpu path
    /sys/device/system/cpu/cpu{core_id}

3. program runtime directory
    1. /var/run/{program_name}/ if has root
    2. $XDG_RUNTIME_DIR/{program_name}
    3. /tmp/{program_name}
