//
//  TurboDisablerTimer.cpp
//  TurboDisabler
//
//  Created by Viktor Remennik on 13.04.2023.
//  Copyright © 2023 st.notexi. All rights reserved.
//

#include "TurboDisabler.hpp"


IOTimerEventSource * TurboDisabler::timer = nullptr;


bool TurboDisabler::setTimer(void)
{   timer = IOTimerEventSource::timerEventSource(this, static_cast<IOTimerEventSource::Action>(&TurboDisabler::timerCallbackDispatcher));
    if (timer == nullptr)
    {   LOG("setTimer: cannot create timer");
        return (false);
    }
    else
    {   timer->setTimeoutMS(TIMER_INITIAL_TIOMEOUT);
        getWorkLoop()->addEventSource(timer);
        //        volatile unsigned long lh, ll;
        //        lh = (unsigned long)this / 0x10000;
        //        ll = (unsigned long)this & 0xFFFF;
        //        LOG("setTimer: timer created for %4.4lX%4.4lX\n", lh, ll);
    }

    return (true);
}

void TurboDisabler::deleteTimer(void)
{   if (timer != nullptr)
    {   timer->cancelTimeout();
        getWorkLoop()->removeEventSource(timer);
        timer->release();
        timer = nullptr;
    }
}

void TurboDisabler::timerCallbackDispatcher(OSObject *owner, IOTimerEventSource * sender)
{   TurboDisabler * self = static_cast<TurboDisabler *>(owner);
    //    volatile unsigned long lh, ll;
    //    lh = (unsigned long)self / 0x10000;
    //    ll = (unsigned long)self & 0xFFFF;
    //
    //    LOG("timerCallbackDispatcher: owner = %4.4lX%4.4lX", lh, ll);
    self->timerCallback(sender);
}

void TurboDisabler::timerCallback(IOTimerEventSource * sender)
{   //    volatile unsigned long lh, ll;
    //    lh = (unsigned long)this / 0x10000;
    //    ll = (unsigned long)this & 0xFFFF;
    //    LOG("timerCallback: owner = %4.4lX%4.4lX", lh, ll);

    if (timer != nullptr)
    {   TurboState state = getSavedTurboState();

        if (state == turboStateUnknown)
        {   TurboState nvramState;
            if (readTurboFromNvram(&nvramState) && nvramState != turboStateUnknown)
            {   setSavedTurboState(nvramState);
            }
        }

        if (getSavedTurboState()!=turboStateUnknown) // Known state has been already read, we could proceed
        {   TurboState nvramState;
            if (!readTurboFromNvram(&nvramState) || nvramState == turboStateUnknown)
            {   LOG("timerCallback: failed to read state from NVRAM");
                return;
            }
//            LOG("timerCallback: NVRAM state is %d, current state is %d", nvramState, state);
            if (nvramState != state || nvramState != getSavedTurboState())
            {   LOG("timerCallback: saved state does not match current state, setting new");
                state=getSavedTurboState();
                setCPUTurbo(state);
                setSavedTurboState(state);
                updateTurboProperty(state);
                writeTurboToNvram(state);
            }
        }
        else
        {   LOG("timerCallback: cannot get state");
        }
        timer->setTimeoutMS(TIMER_LOOP_TIMEOUT);
    }
}

