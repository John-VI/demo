// Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

#pragma once

#include <vector>

#include "clktextureman.h"

namespace clk {

class scrtextbuffer {
public:
	
	std::vector<char> buff;
	void resetScreenSize(int newwidth, int newheight);

private:
	texturehandle font;
	int intwid, inthei;
};

}