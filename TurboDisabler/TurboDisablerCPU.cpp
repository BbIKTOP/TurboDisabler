//
//  TurboDisablerCPU.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 01.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisabler.hpp"


void TurboDisabler::enableCPUTurbo(void)
{   LOG("enableCPUTurbo: Enabling turbo.\n");
    wrmsr64(MSR_IA32_MISC_ENABLE, rdmsr64(MSR_IA32_MISC_ENABLE) & (~TURBO_MODE_DISABLED_MSR_BIT));
}
void TurboDisabler::disableCPUTurbo(void)
{   LOG("disableCPUTurbo: Disabling turbo.\n");
    wrmsr64(MSR_IA32_MISC_ENABLE, rdmsr64(MSR_IA32_MISC_ENABLE) | TURBO_MODE_DISABLED_MSR_BIT);
}

TurboDisabler::TurboState TurboDisabler::getCPUTurbo(void)
{   uint64_t msr = rdmsr64(MSR_IA32_MISC_ENABLE) & TURBO_MODE_DISABLED_MSR_BIT;
    return (msr == 0 ? turboStateOff : turboStateOn);
}


void TurboDisabler::setCPUTurbo(TurboDisabler::TurboState mode)
{   switch (mode)
    {   case turboStateOn:
            enableCPUTurbo();
            break;
        case turboStateOff:
            disableCPUTurbo();
            break;
        default:
            LOG("setCPUTurbo: unknown mode");
            break;
    }
}



