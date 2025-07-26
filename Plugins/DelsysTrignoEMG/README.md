# Delsys Trigno EMG Sensor System Plugin for Unreal Engine
## Introduction
* This plugin is created for data acquisition with Delsys's Trigno 16-sensor system.
* The data acquisition relies on [Trigno Software Development Kit (SDK)](https://delsys.com/support/trigno-sdk/) and its TCP data streaming.

## Blueprints
The following key Blueprint nodes are provided. The order listed is also the recommended sequence for a single data recording routine:
1. **Connect**: Connect to the Trigno sensor base station. The IP address and port have already been configured.
2. **StartAcquisition**: Request the Trigno sensor base station to start data streaming.
3. **StartRecording**: Start recording the data and save it to a user-defined directory, specified as an input argument.
4. **StopRecording**: Stop data recording.
5. **StopAcquisition**: Request the Trigno sensor base station to stop data streaming.
6. **Close**: Close the connection to the Trigno sensor base station.
  
## Usage
* Install [Trigno Software Development Kit (SDK)](https://delsys.com/support/trigno-sdk/).
* Copy the plugin folder to YourProjectName/Plugins (If no such folder, create one).
* In Source/YourProjectName/YourProjectName.Build.cs add the plugin:

  `PublicDependencyModuleNames.AddRange(new string[] { ... , "DelsysTrignoEMG" });`
* Open your project in UE editor, in Edit->Plugins, search "DelsysTrignoEMG" and enable.
