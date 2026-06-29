#ifndef GAME_H
#define GAME_H

#include "particle_game.h"
#include "particle_generation.h"
#include "basic_text_renderer.h"
#include "gui_renderer_sw.h"
#include "gui_handler.h"
#include "input_system.h"
#include "mouse.h"
#include "renderer_sw.h"
#include "chunk_lighting.h"
#include "image_manipulation.h"
#include "entity.h"
#include "custom_parser.h"
#include "asset.h"
#include "asset_renderer.h"

int RunParticleGame(ParticleGame* game);

#endif