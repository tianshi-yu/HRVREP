# Human Robotics Virtual Reality Experimental Platform (In-progress)
This virtual reality experimental platform is developed using UE for human-robot interaction research, such as studying human behaviour in or while interacting with autonomous vehicles. Core logic is implemented in C++, with example usages for a haul truck maneouvering environment provided through Blueprints.

## Features
* Experimental flow control framework that handles the preparation, start, repetition, and end of experiments, as well as data logging during the experiments., see the [GameMode class](Source/HRVREP/Private/GameSystem/GenericExperimentGameMode.cpp) for details.
* Multi-modal sensor data acquisition system with a defined sensor interface and a sensor registration mechanism, see [SensorSystem](Source/HRVREP/Public/SensorSystem) for details. The integrated sensors includes:
  
  | Modality      | Sensor      |Note        |
  | ------------- |-------------|-------------|
  | Gaze          | [VIVE Full Face Tracker](https://www.vive.com/au/accessory/vive-full-face-tracker/)| Based on [VIVE OpenXR](https://developer.vive.com/resources/openxr/) plugin |
  | Electromyography (EMG)| [Delsys Trigno EMG](https://delsys.com/trigno/)| Up to 2kHz sampling rate, realized through Trigno Control Utility and dedicated UE plugin, see [DelsysTrignoEMG plugin folder](Plugins/DelsysTrignoEMG)|
  | Distance radar        | Virutal distance radar| Virtual sensor |

* User information and experimental data saving system, see [UserSystem](Source/HRVREP/Public/UserSystem) and [DataloggerSystem](Source/HRVREP/Public/DataLoggerSystem) for details.
* Compatible VR headset:
  
  | Headset        | XR Plugin   | 
  | ------------- |-------------|
  | HTC VIVE XR Elite| [VIVE OpenXR](https://developer.vive.com/resources/openxr/)|

* VR environments:
  * Mining haul truck maneouvering tasks for driver attention prediction research

* Performance:
  * NVIDIA's Deep Learning Super Sampling (DLSS) is used for improving VR performance.
## Documentation
To be prepared.

## Quick Demo
1. Install Unreal Engine 5.4.4.
2. Install [NVIDIA DLSS 4.0 UE plugin](https://developer.nvidia.com/rtx/dlss?sortBy=developer_learning_library%2Fsort%2Ffeatured%3Adesc%2Ctitle%3Aasc&hitsPerPage=6#getstarted).
3. For a quick demo,
   1. Open and run "LaunchMap" in the root folder.
   2. Under "Select Experiment", select "TruckTestMap".
   3. Under "User Setting", add new user information.
   4. Click "Start Experiment" and go. 
