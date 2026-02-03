
##  Welcome

freeRTOSMonitor is an early debugging library for programs; you can use any of the 5 available functions to print **via UART** the status of the scheduler, CPU, heap, and more.

```ASCII
----- [freeRTOSMonitor] - GENERAL SNAPSHOT -----
[freeRTOSMonitor] Uptime: 9030 ms (9 s)
[freeRTOSMonitor] Heap total: 320424 bytes - Free heap: 301628 bytes
[freeRTOSMonitor] Heap used: 18796 (5%) bytes

----- [freeRTOSMonitor] - SCHEDULER SNAPSHOT -----
Task            State   Prio    Stack   Num
button_task     X       5       200     6       -1
IDLE1           R       0       1024    5       1
IDLE0           R       0       1028    4       0
Producer        B       5       1088    7       -1
ipc0            S       1       364     1       0
Consumer        B       5       280     8       -1
ipc1            S       24      500     2       1


----- [freeRTOSMonitor] - CPU SNAPSHOT -----
Task            CPU %   Runtime
button_task             0%      18
IDLE1           99%     9051319
IDLE0           98%     9007006
Producer                0%      345
Consumer                0%      21753
ipc1            0%      33199
ipc0            0%      32576
============================
```

**PLEASE** note that you must have the corresponding debug options enabled on your device:

```
CONFIG_FREERTOS_USE_TRACE_FACILITY=y
CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS=y
CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS=y
```

If you are using ESP-IDF, you aslo can activate it using:

```
 idf.py menuconfig
```


**Component config -->  FreeRTOS -->  Kernel -->  configUSE_TRACE_FACILITY / Enable display of xCoreID in vTaskList** 

---

##  How to use

###  Option 1: Copy to your project

1. Copy the `components/freeRTOSMonitor` folder into your project's `components/` directory
2. Add to your main component:
```cmake
   idf_component_register(SRCS "main.c"
                       REQUIRES freeRTOSMonitor)
```
3. Include the header:
```c
   #include "monitor.h"
```

### Option 2: ESP Component Registry (coming soon)
```bash
idf.py add-dependency "freeRTOSMonitor"
```

---

##  Functions you can use


####  Uptime

```c
void freeRTOSMonitorSnapshotUptime(void);
```

Via UART:

```
[freeRTOSMonitor] Uptime: XXXX ms (X s)
```

The simplest of all, it shows the total time since the system started.


####  Heap

```c
void freeRTOSMonitorSnapshotHeap(void);
```

Via UART:

```
[freeRTOSMonitor] Heap total: 320424 bytes - Free heap: 301628 bytes
[freeRTOSMonitor] Heap used: 18796 (5%) bytes
```

It displays two messages showing the total heap, how much is used, its percentage, and how much is available.


####  Scheduler

```c
void freeRTOSMonitorSnapshotScheduler(void);
```

Via UART:

```
----- [freeRTOSMonitor] - SCHEDULER SNAPSHOT -----
Task            State   Prio    Stack   Num
button_task     X       5       200     6       -1
IDLE1           R       0       1024    5       1
IDLE0           R       0       1028    4       0
Producer        B       5       1088    7       -1
ipc0            S       1       364     1       0
Consumer        B       5       280     8       -1
ipc1            S       24      500     2       1
```

Displays the system scheduler, for each task it shows: task, status, priority, stack, task number (Kernel), and number of cores assigned.

####  CPU

```c
void freeRTOSMonitorSnapshotCpu(void);
```

Via UART:

```
----- [freeRTOSMonitor] - CPU SNAPSHOT -----
Task            CPU %   Runtime
button_task             0%      18
IDLE1           99%     9051319
IDLE0           98%     9007006
Producer                0%      345
Consumer                0%      21753
ipc1            0%      33199
ipc0            0%      32576
```

It shows the CPU usage of each task. **Just in case:**

High CPU usage reported by IDLE tasks (e.g. ~99%) is normal in FreeRTOS.
IDLE tasks represent unused CPU time, so a high percentage means the system is mostly idle and application tasks are either blocked or waiting.
When system load increases, the CPU percentage of IDLE tasks decreases accordingly.

####  ALL

```c
void freeRTOSMonitorSnapshotAll(void);
```

Displays all reading parameters in order (As in the example at the beginning.)

---

####  To conclude

This library should be used only in the early stage of develop of your code and never on a finished product, **can** be heavy in resources.

I will be updating some things over time. Thanks for reading :D
