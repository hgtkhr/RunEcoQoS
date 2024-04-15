# RunEcoQos
This repository describes the ability to explicitly launch an executable file as an Eco QoS process.
Eco QoS is a Windows 11 feature that allows app processes to run more power efficiently.

https://devblogs.microsoft.com/performance-diagnostics/introducing-ecoqos/

> Windows associates a Quality of Service (QoS) for processes and threads which indicates the balance of performance and power efficiency. Starting with Windows 10 Insider Preview Build 21359, Windows includes a new Quality of Service level called EcoQoS. This new QoS level is valuable for workloads that do not have a significant performance or latency requirement, to enable them to always run in an energy efficient manner. Developers can call APIs to explicitly opt in their processes and threads to be identified as EcoQoS, and Windows takes care of the rest. Windows will take this as a hint to automatically schedule this work to the most efficient processors, and to configure the processors to run at the most efficient clock speed. The usage of EcoQoS is ideal for background services, updaters, sync engines, indexing services, and more, where energy efficiency is the focus.

This repository describes the ability to explicitly launch an executable file as an Eco QoS process.
The program uses the following method to make an application process EcoQoS
1. Set **PROCESS_POWER_THROTTLING_EXECUTION_SPEED** to the **ControlMask** and **StateMask** members of the **PROCESS_POWER_THROTTLING_STATE** structure, and Apply the settings with the **SetProcessInformation** API function.
2. Set the priority of the application process to **IDLE_PRIORITY_CLASS** in the **SetPriorityClass** API function.
3. (Optional) You can also set the memory priority by command line option.

Command line option: `/MemPriority:{VERY_LOW|LOW|MEDIUM|BELOW_NORMAL|NORMAL}`
|Value|Meaning|
|-----|------|
|VERY_LOW|Very low memory priority.|
|LOW|Low memory priority.|
|MEDIUM|Medium memory priority.|
|BELOW_NORMAL|Below normal memory priority.|
|NORMAL|This is the default priority for all threads and processes on the system.|

**MEMORY_PRIORITY_INFORMATION structure (processthreadsapi.h)**
https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-memory_priority_information

## Requirements
RunEcoQos is written by Visual Studio C++ 2022. Therefore, Visual Studio 2022 is required. Also, please install the latest Windows SDK.

## How to build and install
Clone this repository and open `RunEcoQoS.sln`.
After setting the desired configuration and building, a bin folder will be created in the solution folder, so copy the contents of the bin folder and use it as needed.

## Lastly
Currently, there are few API samples for EcoQoS, and we hope that the contents of this repository will be helpful to developers who wish to use EcoQoS functionality.
