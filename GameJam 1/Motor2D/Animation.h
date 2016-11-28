#ifndef _ANIMATION_
#define _ANIMATION_

#include "p2List.h"
#include "j1App.h"
#include "j1Textures.h"
#include "SDL\include\SDL.h"

struct SDL_Texture;

enum AnimTypes {
	Idle, Run, Shoot, Jump, Hit, Explode
};

class Animation {
public:
	p2List<SDL_Rect> frames;
	float speed = 1.0f;
	bool loop = false;
	AnimTypes type;

public:
	Animation() {	}
	Animation(p2List<SDL_Rect>& rects, float speed, AnimTypes type, bool loop = true): speed(speed), loop(loop), type(type){
		for (p2List_item<SDL_Rect>* rect = rects.start; rect != nullptr; rect = rect->next) {
			frames.add(rect->data);
		}
	}
	Animation(Animation& anim): frames(anim.frames), speed(anim.speed), loop(anim.loop), curr_frame(anim.curr_frame), loops(anim.loops){}

	~Animation(){	}

	//return current frame+speed
	SDL_Rect& GetCurrentFrameRect() {
		curr_frame += speed;
		if (curr_frame >= frames.count())
		{
			curr_frame = (loop) ? 0.0f : frames.count() - 1;
			loops++;
		}

		return frames[(int)curr_frame];
	}

	SDL_Rect& GetActualFrameRect() {
		return frames[(int)curr_frame];
	}

	float GetFrameIndex()const {
		return curr_frame;
	}

	SDL_Rect& GetFrame(int frame) {
		return frames[frame];
	}

	void SetInitialFrame(uint frame) {
		curr_frame = (float)frame;
	}

	void SetFrames(SDL_Rect* _frames, uint n_frames) 
	{
		for (int i = 0; i < n_frames; i++)
		{
			frames.add(_frames[i]);
		}
	}

	void SetSpeed(float _speed)
	{
		speed = _speed;
	}
	
	void SetLoop(bool _loop) 
	{
		loop = _loop;
	}

	bool IsFrame(uint frame_index) {
		return (curr_frame == frame_index);
	}

	void AnimForward() {
		curr_frame += speed;
		if (curr_frame >= frames.count()) curr_frame = 0;
	}

	void AnimBack() {
		curr_frame -= speed;
		if (curr_frame < 0)curr_frame = (float)frames.count() - 1;
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		curr_frame = 0;
		loops = 0;
	}

private:
	float curr_frame = 0.0f;
	uint loops = 0;

private:

};

#endif
