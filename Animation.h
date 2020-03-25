#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL_rect.h"
#define MAX_FRAMES 100
class Animation
{
public:
	bool loop = true;
	bool flow = false;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES]; //array of sdl_rect to store each sprite of the animation


private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;
	enum FLOW {
		forwards,
		backwards
	} direction = forwards;

public:
	Animation()
	{}
	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame) { // the colon is used to inherit or in this case to initialize variables before the constructor is called
		SDL_memcpy(&frames, anim.frames, sizeof(frames));//copies the info in anim.frames to local variable frames
	}
	void PushBack(const SDL_Rect& rect) //stores the rect into frames array
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{

		switch (direction)
		{
		case FLOW::forwards:
		{
			current_frame += speed;
			if (current_frame >= last_frame)
			{
				current_frame = (loop || flow) ? 0.0f : last_frame - 1;
				direction = flow ? FLOW::backwards : FLOW::forwards;
				loops++;
			}
		}
		break;
		case FLOW::backwards:
		{
			current_frame -= speed;
			if (current_frame <= 0.0f)
			{
				current_frame = 0.0f;
				direction = FLOW::forwards;
				loops++;
			}
		}
		break;
		}

		return frames[(int)current_frame];
	}
	bool Finished() const
	{
		return loops > 0;
	}
	void Reset()
	{
		loops = 0;
		current_frame = 0.0f;
	}


};

#endif