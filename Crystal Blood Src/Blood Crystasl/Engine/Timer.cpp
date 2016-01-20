/// \file timer.cpp
/// \brief Code for timer class CTimer.

#include "timer.h"

CTimer::CTimer(): 
m_nStartTime(0),  m_nLevelStartTime(0), m_nLevelFinishTime(0), m_bLevelTimerOn(FALSE){};

/// Start the timer.

void CTimer::start(){ //start the timer
  m_nStartTime = timeGetTime();
} //start

/// Get current time.
/// \return Current time since start in milliseconds.

int CTimer::time(){ //return the time
  return m_nCurrentTime;
} //time

/// The elapsed function is a useful function for measuring repeating time intervals.
/// Given the start and duration times, this function returns TRUE if the interval
/// is over, and has the side-effect of resetting the start time when that happens,
/// thus setting things up for the next interval.
/// \param start start of time interval
/// \param interval duration of time interval
/// \return TRUE if time interval is over

int CTimer::elapsed(int &start, int interval){
  if(m_nCurrentTime >= start + interval){ //if interval is over
    start = m_nCurrentTime; return TRUE; //reset the start and succeed
  }
  else return FALSE; //otherwise, fail
} //elapsed

/// This is the signal that a new animation frame has begun,
/// so that the timer can return the same time value for the
/// duration of this frame.

void CTimer::beginframe(){
  m_nCurrentTime = timeGetTime() - m_nStartTime;
} //beginframe

/// \return Level start time.

int CTimer::GetLevelStartTime(){
  return m_nLevelStartTime;
} //GetLevelStartTime

/// \return Level elapsed time.

int CTimer::GetLevelElapsedTime(){
  return m_bLevelTimerOn?
    timeGetTime() - m_nLevelStartTime: 
    m_nLevelFinishTime - m_nLevelStartTime;
} //GetLevelElapsedTime

void CTimer::StartLevelTimer(){
  m_bLevelTimerOn = TRUE;
  m_nLevelStartTime = timeGetTime();
} //StartLevelTimer

void CTimer::StopLevelTimer(){
  m_bLevelTimerOn = FALSE;
  m_nLevelFinishTime = timeGetTime();
} //StopLevelTimer