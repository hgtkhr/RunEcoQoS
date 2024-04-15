Welcome to the RunEcoQoS wiki!
# RunEcoQos
This repository describes the ability to explicitly launch an executable file as an Eco QoS process.
Eco QoS is a Windows 11 feature that allows app processes to run more power efficiently.

https://devblogs.microsoft.com/performance-diagnostics/introducing-ecoqos/

> Windows associates a Quality of Service (QoS) for processes and threads which indicates the balance of performance and power efficiency. Starting with Windows 10 Insider Preview Build 21359, Windows includes a new Quality of Service level called EcoQoS. This new QoS level is valuable for workloads that do not have a significant performance or latency requirement, to enable them to always run in an energy efficient manner. Developers can call APIs to explicitly opt in their processes and threads to be identified as EcoQoS, and Windows takes care of the rest. Windows will take this as a hint to automatically schedule this work to the most efficient processors, and to configure the processors to run at the most efficient clock speed. The usage of EcoQoS is ideal for background services, updaters, sync engines, indexing services, and more, where energy efficiency is the focus.

This repository describes the ability to explicitly launch an executable file as an Eco QoS process.
The program uses the following method to make an application process EcoQoS
1. Set **PROCESS_POWER_THROTTLING_EXECUTION_SPEED** to the **ControlMask** and **StateMask** members of the **PROCESS_POWER_THROTTLING_STATE** structure, and Apply the settings with the **SetProcessInformation** API function.
1. (if necessary) Set the priority of the application process to **IDLE_PRIORITY_CLASS** in the **SetPriorityClass** API function.
1. (Optional) You can also set the memory priority by command line option.

## Command line option: 
  
`/Priority:{ABOVE_NORMAL|BELOW_NORMAL|HIGH|IDLE|NORMAL|BACKGROUND|REALTIME}`  
  
The operating priority of processes and threads is set based on the value of this setting.  
Set an appropriate value for stable operation of the application.  
If the default **/Priority:NORMAL** does not attach the efficiency mode icon in the task manager, try setting it to **/Priority:LOW**.  
See **[SetPriorityClass function (processthreadsapi.h)](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setpriorityclass) .**  
  
Value|Meaning
---|---
ABOVE_NORMAL|Process that has priority above NORMAL but below HIGH.
BELOW_NORMAL|Process that has priority above IDLE but below NORMAL.
HIGH|Process that performs time-critical tasks that must be executed immediately. The threads of the process preempt the threads of normal or idle priority class processes. An example is the Task List, which must respond quickly when called by the user, regardless of the load on the operating system. Use extreme care when using the high-priority class, because a high-priority class application can use nearly all available CPU time.
IDLE|Process whose threads run only when the system is idle. The threads of the process are preempted by the threads of any process running in a higher priority class. An example is a screen saver. The idle-priority class is inherited by child processes.
NORMAL|Process with no special scheduling needs.
BACKGROUND|Begin background processing mode. The system lowers the resource scheduling priorities of the process (and its threads) so that it can perform background work without significantly affecting activity in the foreground.This value can be specified only if hProcess is a handle to the current process. The function fails if the process is already in background processing mode.
REALTIME|Process that has the highest possible priority. The threads of the process preempt the threads of all other processes, including operating system processes performing important tasks. For example, a real-time process that executes for more than a very brief interval can cause disk caches not to flush or cause the mouse to be unresponsive. 
  
`/MemPriority:{VERY_LOW|LOW|MEDIUM|BELOW_NORMAL|NORMAL}`   
  
Set the memory priority.  
This setting values affects keeping of working set.  
Set an appropriate value for stable operation of the application.  
The default value is **/MemPriority:NORMAL**.  
See [**MEMORY_PRIORITY_INFORMATION structure (processthreadsapi.h)**](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-memory_priority_information).**  
  
Value|Meaning
---|---
VERY_LOW|Very low memory priority.
LOW|Low memory priority.
MEDIUM|Medium memory priority.
BELOW_NORMAL|Below normal memory priority.
NORMAL|This is the default priority for all threads and processes on the system.  

## Requirements
RunEcoQos is written by Visual Studio C++ 2022. Therefore, Visual Studio 2022 is required. Also, please install the latest Windows SDK.

## How to build and install
Clone this repository and open `RunEcoQoS.sln`.
After setting the desired configuration and building, a bin folder will be created in the solution folder, so copy the contents of the bin folder and use it as needed.

## Lastly
Currently, there are few API samples for EcoQoS, and we hope that the contents of this repository will be helpful to developers who wish to use EcoQoS functionality.
