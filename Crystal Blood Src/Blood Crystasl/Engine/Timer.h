/// \file timer.h
/// \brief Interface for the timer class CTimer.

#pragma once

#include "Defines.h"

/// \brief The timer.
///
/// The timer allows you to manage game events by duration, rather than
/// on a frame-by-frame basis. This simple version of the timer is based on
/// the Windows API function timeGetTime, which is notoriously inaccurate
/// but perfectly adequate for a simple game demo.

class CTimer{ //game timer class
  private:
    int m_nStartTime; ///< Time that timer was started.
    int m_nCurrentTime; ///< Current time.
    int m_nLevelStartTime; ///< Time the current level started.
    int m_nLevelFinishTime; ///< Time the current level finished.
    BOOL m_bLevelTimerOn; ///< Whether the current level is being timed.
  public:
    CTimer(); ///< Constructor.
    void start(); ///< Start the timer.
    int time(); ///< Return the time in ms.
    BOOL elapsed(int &start ,int interval); ///< Has interval ms elapsed since start?
    void beginframe(); ///< Freeze the time returned for the duration of this frame.
    int GetLevelStartTime(); ///< Get time that the level started.
    int GetLevelElapsedTime(); ///< Get elapsed time since the level started.
    void StartLevelTimer(); ///< Start the level timer.
    void StopLevelTimer(); ///< Stop the level timer.
}; //CTimer