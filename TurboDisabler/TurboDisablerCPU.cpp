//
//  TurboDisablerCPU.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 01.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisabler.hpp"


void TurboDisabler::enableTurbo(void)
{   LOG("TurboDisabler: enabling turbo.\n");
    wrmsr64(MSR_IA32_MISC_ENABLE, rdmsr64(MSR_IA32_MISC_ENABLE) & (~TURBO_MODE_DISABLED_MSR_BIT));
    currentTurboState = true;
    updateTurboProperty();
}
void TurboDisabler::disableTurbo(void)
{   LOG("TurboDisabler: disabling turbo.\n");
    wrmsr64(MSR_IA32_MISC_ENABLE, rdmsr64(MSR_IA32_MISC_ENABLE) | TURBO_MODE_DISABLED_MSR_BIT);
    currentTurboState = false;
    updateTurboProperty();
}

bool TurboDisabler::getTurbo(void)
{   currentTurboState = !(rdmsr64(MSR_IA32_MISC_ENABLE) & TURBO_MODE_DISABLED_MSR_BIT);
    updateTurboProperty();
    return (currentTurboState);
}


void TurboDisabler::setTurbo()
{   if (currentTurboState)
    {   enableTurbo();
    }
    else
    {   disableTurbo();
    }
}
void TurboDisabler::setTurbo(bool mode)
{   currentTurboState = mode;
    setTurbo();
}



