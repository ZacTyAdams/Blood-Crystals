/// \file renderworld.h 
/// \brief Interface for the render world class CRenderWorld.

#pragma once

#include "renderer.h"
#include "gamedefines.h"

/// \brief The render world.
///
/// Render world handles the game-specific rendering tasks, relying on
/// CRenderer to do all of the actual API-specific rendering.

class CRenderWorld: public CRenderer{
public:
  void LoadImages(); ///< Load images.
}; //CRenderWorld