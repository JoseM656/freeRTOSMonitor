

**PLEASE** note that you must have the corresponding debug options enabled on your device:

```
CONFIG_FREERTOS_USE_TRACE_FACILITY=y
CONFIG_FREERTOS_USE_STATS_FORMATTING_FUNCTIONS=y
CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS=y
```

If you are using ESP-IDF, you aslo can activate it using:

￼```
 idf.py menuconfig
```

**Component config -->  FreeRTOS -->  Kernel -->  configUSE_TRACE_FACILITY / Enable display of xCoreID in vTaskList** 


￼### Functions you can use


￼#### Uptime

```c
void freeRTOSMonitorSnapshotUptime(void);
```

Via UART:

```
[freeRTOSMonitor] Uptime: XXXX ms (X s)
```

The simplest of all, it shows the total time since the system started.


￼#### Heap

```c
void freeRTOSMonitorSnapshotHeap(void);
```

Via UART:

```
[freeRTOSMonitor] Heap total: 320424 bytes - Free heap: 301628 bytes
[freeRTOSMonitor] Heap used: 18796 (5%) bytes
```

It displays two messages showing the total heap, how much is used, its percentage, and how much is available.


￼#### Scheluder

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

￼#### CPU

```c
void freeRTOSMonitorSnapshotCpu(void);
```

Via UART:
I will be updating some things over time. Thanks for reading :D
