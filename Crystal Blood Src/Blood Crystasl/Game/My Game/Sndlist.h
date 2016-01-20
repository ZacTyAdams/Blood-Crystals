/// \file sndlist.h
/// \brief Enumerated types for sounds.

#pragma once

/// \brief Game sound enumerated type. 
/// These are the sounds used in actual gameplay. Sounds must be listed here in
/// the same order that they are in the sound settings XML file.

enum GameSoundType{ //sounds used in game engine
  THUMP_SOUND, BOING_SOUND, OW_SOUND, MATCH, GAMEOVER, TITLE, STEPS, TANK, SHOTS, EXPLOSION
};