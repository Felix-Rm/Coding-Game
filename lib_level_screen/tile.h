
#pragma once

#include "../lib_window/drawable.h"

class Tile : public Drawable
{
private:
    /* data */
public:
    Tile(bool is_drivable, int tex_id);
};
