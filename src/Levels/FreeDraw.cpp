// ----------------------------------------------------------------------------
// File: FreeDraw.cpp
// Project: CS200 Assignment 1
// Purpose: 
// Author: Thomas Komair
// Copyright DigiPen Institute of Technology, 2015. All rights reserved
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// INCLUDES
#include <AEEngine.h>
#include "..\Engine\Rasterizer\Rasterizer.h"
#include "Common.h"

namespace FreeDraw {
	// ----------------------------------------------------------------------------
	// GLOBALS AND DEFINITIONS
	std::vector<std::pair<AEVec2, AEVec2>> gLineArray;
	std::vector<std::pair<AEVec2, AEVec2>> gCircleArray;
	unsigned int	gCurrentLinePoint = 0;
	unsigned int	gCurrentCirclePoint = 0;
	// ----------------------------------------------------------------------------
	// GAMESTATEFUNCTIONS
	void Init()
	{
		gLineArray.clear();
		gCurrentLinePoint = 0;
	}
	void Update()
	{
		KeyboardInput();
		bool isGuiActive = ShowFrameworkMenu();

		// current the mouse position to frame buffer space (origin at topleft)
		AEVec2 mouseVP = AEGetMouseData().position + AEVec2((float)AEGetSysWindowWidth() / 2.0f, (float)AEGetSysWindowHeight() / 2.0f);

		// clear all primitives
		if (AEInputKeyTriggered('C'))
		{
			gLineArray.clear();
			gCurrentLinePoint = 0;
		}
		if (!isGuiActive) {
			// new line
			if (AEInputMouseTriggered(AE_MOUSE_LEFT))
			{
				if (gCurrentLinePoint == 0 || AEInputKeyPressed(AE_KEY_CTRL)) {
					gLineArray.push_back(std::pair<AEVec2, AEVec2>(mouseVP, mouseVP));
					gCurrentLinePoint++;
				}
				else gCurrentLinePoint = 0;
			}

			if (AEInputMouseTriggered(AE_MOUSE_RIGHT))
			{
				if (gCurrentCirclePoint == 0 || AEInputKeyPressed(AE_KEY_SHIFT)) {
					gCircleArray.push_back(std::pair<AEVec2, AEVec2>(mouseVP, mouseVP));
					gCurrentCirclePoint++;
				}
				else gCurrentCirclePoint = 0;
			}

			// follow mouse
			if (gCurrentLinePoint)
			{
				gLineArray.back().second = mouseVP;
			}

			if (gCurrentCirclePoint)
			{
				gCircleArray.back().second = mouseVP;
			}


		}
	}
	void Render()
	{
		Rasterizer::FrameBuffer::Clear(Rasterizer::Color(1.0f, 1.0f, 1.0f, 1.0f));

		// render all the lines
		for (unsigned int i = 0; i < gLineArray.size(); ++i)
		{
			// get line endpoints
			AEVec2& p0 = gLineArray[i].first;
			AEVec2& p1 = gLineArray[i].second;

			// Draw the line
			Rasterizer::DrawLine(p0, p1, Rasterizer::Color());
		}

		for (unsigned int i = 0; i < gCircleArray.size(); ++i)
		{
			// get circle endpoints
			AEVec2& p0 = gCircleArray[i].first;
			AEVec2& p1 = gCircleArray[i].second;

			float r = p0.Distance(p1) / 2.0f;
			// Draw the circle
			Rasterizer::DrawCircle(p0, r, Rasterizer::Color());
		}

		Rasterizer::FrameBuffer::Present();
	}
}