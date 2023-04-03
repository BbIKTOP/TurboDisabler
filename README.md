# TurboDisabler
A simple kernel extension for disabling CPU turbo modes on Intel Mac.

## Purpose
Some computers (especially fanless) cannot dissipate Intel's heat. Disabling turbo mode could reduce TDP by reducing the peak performance.

## Usage
Just load kext and turbo will be disabled. Unload it to reenable default turbo mode behaviour.
You have to change the owner to allow macos to use it. Use `chown 0:0 TurboDisabler.kext` in terminal before loading it with `kextload TurboDisabler.kext`.
TurboDisablerClient command line tool is intended to control turbo on/off state from the command line. Last state set is stored in the "TurboMode" NVRAM property. This property is read on the kext start and saved on exit.

## Copyright
No rocket science here, but do not use it in commercial products.

## Todo
Notification area menu tool to control turbo mode.