/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "chop.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
#include "../random.h"
#include "../timer.h"
#include "../animation.h"

fixed_t fadewhite, fadeblack, fadespeed, fadeextra, fadeextra2, fadeextra3, fadeblack2, fadepurp, fadepurp2;
fixed_t purpout1, purpout2, purpout3;
boolean fademode;

//too many fucking variables
fixed_t fadered, fadered2, redspeed;
boolean redmode;

//Week 4 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0; //Wall left
	Gfx_Tex tex_back0p; //Wall left
	Gfx_Tex tex_back1; //Wall right
	Gfx_Tex tex_back1p; //Wall right

	Gfx_Tex tex_floor; //Floor
	Gfx_Tex tex_floorp; //Floor

	Gfx_Tex tex_junk; //random objects
	Gfx_Tex tex_junkp; //random objects

	Gfx_Tex tex_fireplace; //fireplace
	Gfx_Tex tex_fireplacep; //random objects
	//fire
	u8 fire_frame, fire_tex_id;
	Animatable fire_animatable;
	
	//fire purple
	u8 firep_frame, firep_tex_id;
	Animatable firep_animatable;

	//gabe newell
	u8 steam_frame, steam_tex_id;
	Animatable steam_animatable;

	//wick
	u8 wick_frame, wick_tex_id;
	Animatable wick_animatable;
	
	//wick purple
	u8 wickp_frame, wickp_tex_id;
	Animatable wickp_animatable;
	
	
	
} Back_Chop;

static const CharFrame fire_frame[4] = {
	{0, {169,   0,  59, 51}, {0,  0}},
	{0, {105,  79,  55, 51}, {-1, 0}},
	{0, {160,  79,  59, 64}, {0, 12}},
	{0, {105, 143,  57, 59}, {-1, 8}},
};

static const Animation fire_anim[1] = {
	{2, (const u8[]){0, 1, 2, 3, ASCR_REPEAT}},
};

void Chop_Fire_Draw(Back_Chop *this, fixed_t x, fixed_t y)
{
	//Draw animated object
	const CharFrame *cframe = &fire_frame[this->fire_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_junk, &src, &dst, stage.camera.bzoom);
}

void Chop_Fire_SetFrame(void *user, u8 frame)
{
	Back_Chop *this = (Back_Chop*)user;
	
	//Check if this is a new frame
	if (frame != this->fire_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &fire_frame[this->fire_frame = frame];
	}
}

static const CharFrame firep_frame[4] = {
	{0, {169,   0,  59, 51}, {0,  0}},
	{0, {105,  79,  55, 51}, {-1, 0}},
	{0, {160,  79,  59, 64}, {0, 12}},
	{0, {105, 143,  57, 59}, {-1, 8}},
};

static const Animation firep_anim[1] = {
	{2, (const u8[]){0, 1, 2, 3, ASCR_REPEAT}},
};

void Chop_Firep_Draw(Back_Chop *this, fixed_t x, fixed_t y)
{
	//Draw animated object
	const CharFrame *cframe = &firep_frame[this->firep_frame];
				
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
				
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_junkp, &src, &dst, stage.camera.bzoom);
}
			
void Chop_Firep_SetFrame(void *user, u8 frame)
{
	Back_Chop *this = (Back_Chop*)user;
	
	//Check if this is a new frame
	if (frame != this->firep_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &firep_frame[this->firep_frame = frame];
	}
}

static const CharFrame steam_frame[4] = {
	{0, {162, 143,  15,  29}, {0,   0}},
	{0, {177, 143,  12,  35}, {-1,  6}},
	{0, {189, 143,  13,  35}, {-6,  6}},
	{0, {202, 143,  14,  32}, {-3,  3}},
};

static const Animation steam_anim[1] = {
	{4, (const u8[]){0, 1, 2, 3, ASCR_REPEAT}},
};

void Chop_Steam_Draw(Back_Chop *this, fixed_t x, fixed_t y)
{
	//Draw animated object
	const CharFrame *cframe = &steam_frame[this->steam_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	//Stage_DrawTex(&this->tex_junk, &src, &dst, stage.camera.bzoom);
	Stage_BlendTex(&this->tex_junk, &src, &dst, stage.camera.bzoom, 0);
}

void Chop_Steam_SetFrame(void *user, u8 frame)
{
	Back_Chop *this = (Back_Chop*)user;
	
	//Check if this is a new frame
	if (frame != this->steam_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &steam_frame[this->steam_frame = frame];
	}
}

static const CharFrame wick_frame[4] = {
	{0, {  0, 175,  4,  8}, {0,  0}},
	{0, {  4, 175,  4,  9}, {0,  1}},
	{0, {  8, 175,  4,  7}, {0, -1}},
	{0, { 12, 175,  5,  7}, {0, -1}},
};

static const Animation wick_anim[1] = {
	{2, (const u8[]){0, 1, 2, 3, ASCR_REPEAT}},
};

void Chop_Wick_Draw(Back_Chop *this, fixed_t x, fixed_t y)
{
	//Draw animated object
	const CharFrame *cframe = &wick_frame[this->wick_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_junk, &src, &dst, stage.camera.bzoom);
}

void Chop_Wick_SetFrame(void *user, u8 frame)
{
	Back_Chop *this = (Back_Chop*)user;
	
	//Check if this is a new frame
	if (frame != this->wick_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &fire_frame[this->wick_frame = frame];
	}
}

static const CharFrame wickp_frame[4] = {
	{0, {  0, 175,  4,  8}, {0,  0}},
	{0, {  4, 175,  4,  9}, {0,  1}},
	{0, {  8, 175,  4,  7}, {0, -1}},
	{0, { 12, 175,  5,  7}, {0, -1}},
};

static const Animation wickp_anim[1] = {
	{2, (const u8[]){0, 1, 2, 3, ASCR_REPEAT}},
};

void Chop_Wickp_Draw(Back_Chop *this, fixed_t x, fixed_t y)
{
	//Draw animated object
	const CharFrame *cframe = &wickp_frame[this->wickp_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_junkp, &src, &dst, stage.camera.bzoom);
}

void Chop_Wickp_SetFrame(void *user, u8 frame)
{
	Back_Chop *this = (Back_Chop*)user;
	
	//Check if this is a new frame
	if (frame != this->wickp_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &fire_frame[this->wickp_frame = frame];
	}
}

void Back_Chop_DrawBG(StageBack *back)
{
	Back_Chop *this = (Back_Chop*)back;
	

	
	fixed_t fx, fy;

	Animatable_Animate(&this->fire_animatable, (void*)this, Chop_Fire_SetFrame);
	Animatable_Animate(&this->firep_animatable, (void*)this, Chop_Firep_SetFrame);
	Animatable_Animate(&this->steam_animatable, (void*)this, Chop_Steam_SetFrame);
	Animatable_Animate(&this->wick_animatable, (void*)this, Chop_Wick_SetFrame);
	Animatable_Animate(&this->wickp_animatable, (void*)this, Chop_Wickp_SetFrame);
	
	
	
	//Draw room
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT halll_src = {0, 0, 255, 256};
	RECT_FIXED halll_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(256,1)
	};

	RECT hallr_src = {0, 0, 256, 256};
	RECT_FIXED hallr_dst = {
		FIXED_DEC(90 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(256,1)
	};

	RECT floorl_src = {0, 0, 255, 125};
	RECT_FIXED floorl_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(76,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};

	RECT floorr_src = {0, 125, 255, 125};
	RECT_FIXED floorr_dst = {
		FIXED_DEC(91 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(74,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};

	RECT chair_src = {0, 0, 105, 175};
	RECT_FIXED chair_dst = {
		FIXED_DEC(-107 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-52,1) - fy,
		FIXED_DEC(105 + SCREEN_WIDEOADD,1),
		FIXED_DEC(175,1)
	};

	RECT table_src = {105, 0, 64, 79};
	RECT_FIXED table_dst = {
		FIXED_DEC(245 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(42,1) - fy,
		FIXED_DEC(64 + SCREEN_WIDEOADD,1),
		FIXED_DEC(79,1)
	};

	RECT fireplace_src = {0, 0, 213, 256};
	RECT_FIXED fireplace_dst = {
		FIXED_DEC(8 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-145,1) - fy,
		FIXED_DEC(213 + SCREEN_WIDEOADD,1),
		FIXED_DEC(256,1)
	};
	
	RECT halllp_src = {0, 0, 255, 256};
	RECT_FIXED halllp_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(256,1)
	};

	RECT hallrp_src = {0, 0, 256, 256};
	RECT_FIXED hallrp_dst = {
		FIXED_DEC(90 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(256,1)
	};
	
	RECT floorlp_src = {0, 0, 255, 125};
	RECT_FIXED floorlp_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(76,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};

	RECT floorrp_src = {0, 125, 255, 125};
	RECT_FIXED floorrp_dst = {
		FIXED_DEC(91 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(74,1) - fy,
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(128,1)
	};

	RECT chairp_src = {0, 0, 105, 175};
	RECT_FIXED chairp_dst = {
		FIXED_DEC(-107 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-52,1) - fy,
		FIXED_DEC(105 + SCREEN_WIDEOADD,1),
		FIXED_DEC(175,1)
	};

	RECT tablep_src = {105, 0, 64, 79};
	RECT_FIXED tablep_dst = {
		FIXED_DEC(245 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(42,1) - fy,
		FIXED_DEC(64 + SCREEN_WIDEOADD,1),
		FIXED_DEC(79,1)
	};

	RECT fireplacep_src = {0, 0, 213, 256};
	RECT_FIXED fireplacep_dst = {
		FIXED_DEC(8 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-145,1) - fy,
		FIXED_DEC(213 + SCREEN_WIDEOADD,1),
		FIXED_DEC(256,1)
			
	};

	if (stage.stage_id == StageId_1_5)
	{
		//bft cutscene shit
		if (fadeblack > 0 && fademode == 0)
		{
			if (fadeblack >= 81858)
			{
				//This sucks
				static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
				u8 flash_col = fadeblack >> FIXED_SHIFT;
				u8 flash_col2 = fadeextra >> FIXED_SHIFT;
				Gfx_BlendRect(&flash, flash_col, flash_col, flash_col, 2);
			}
			else
			{
   	 		static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
				u8 flash_col = fadeblack >> FIXED_SHIFT;
				u8 flash_col2 = fadeextra >> FIXED_SHIFT;
				Gfx_BlendRect(&flash, flash_col, flash_col, flash_col, 2);
				fadeblack += FIXED_MUL(fadespeed, timer_dt*3);  
			}

	
		}

		if (fadeblack2 > 0 && fademode == 1)
		{
   	 	static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			u8 flash_col = fadeblack2 >> FIXED_SHIFT;
			u8 flash_col2 = fadeextra >> FIXED_SHIFT;
			Gfx_BlendRect(&flash, flash_col, flash_col, flash_col, 2);
			fadeblack2 -= FIXED_MUL(fadespeed, timer_dt*3); 
	
		}
	}

	//wilter purple
	if (stage.stage_id == StageId_1_2)
	{
		if (fadepurp > 0 && fademode == 0)
		{	
			//purple animation
			Chop_Firep_Draw(this, FIXED_DEC(85,1) - fx, FIXED_DEC(31,1) - fy);
			Chop_Steam_Draw(this, FIXED_DEC(271,1) - fx, FIXED_DEC(15,1) - fy);
			
			Chop_Wickp_Draw(this, FIXED_DEC(41,1) - fx, FIXED_DEC(-26,1) - fy);
			Chop_Wickp_Draw(this, FIXED_DEC(55,1) - fx, FIXED_DEC(-21,1) - fy);
			Chop_Wickp_Draw(this, FIXED_DEC(184,1) - fx, FIXED_DEC(-26,1) - fy);
			
			//purple background
			Stage_DrawTex(&this->tex_fireplacep, &fireplacep_src, &fireplacep_dst, stage.camera.bzoom);
			Stage_DrawTex(&this->tex_junkp, &tablep_src, &tablep_dst, stage.camera.bzoom);
			Stage_DrawTex(&this->tex_junkp, &chairp_src, &chairp_dst, stage.camera.bzoom);
			Stage_DrawTex(&this->tex_floorp, &floorlp_src, &floorlp_dst, stage.camera.bzoom);
			Stage_DrawTex(&this->tex_floorp, &floorrp_src, &floorrp_dst, stage.camera.bzoom);
			Stage_DrawTex(&this->tex_back0p, &halllp_src, &halllp_dst, stage.camera.bzoom);
			Stage_DrawTex(&this->tex_back1p, &hallrp_src, &hallrp_dst, stage.camera.bzoom);

	
		}

		if (purpout1 > FIXED_DEC(1,1) && fademode == 1|| purpout2 > FIXED_DEC(1,1) && fademode == 1)
		{
			static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			u8 flash_col = purpout1 >> FIXED_SHIFT;
			u8 flash_col2 = purpout2 >> FIXED_SHIFT;
			u8 flash_col3 = purpout3 >> FIXED_SHIFT;
			Gfx_BlendRect(&flash, flash_col, flash_col2, flash_col3, 2);
			if (purpout1 > FIXED_DEC(1,1))
				purpout1 -= FIXED_MUL(fadespeed, timer_dt*3);    
			if (purpout2 > FIXED_DEC(1,1))
				purpout2 -= FIXED_MUL(fadespeed, timer_dt*3);  
			if (purpout3 > FIXED_DEC(0,1))
				purpout3 -= FIXED_MUL(fadespeed, timer_dt*3);  
			//try to prevent weird colors from flashing
			if (purpout1 < 0)
				purpout1 = 0;
			if (purpout2 < 0)
				purpout2 = 0;
			if (purpout3 < 0)
				purpout3 = 0;
		}
	}


	Chop_Fire_Draw(this, FIXED_DEC(85,1) - fx, FIXED_DEC(31,1) - fy);
	Chop_Steam_Draw(this, FIXED_DEC(271,1) - fx, FIXED_DEC(15,1) - fy);
	
	Chop_Wick_Draw(this, FIXED_DEC(41,1) - fx, FIXED_DEC(-26,1) - fy);
	Chop_Wick_Draw(this, FIXED_DEC(55,1) - fx, FIXED_DEC(-21,1) - fy);
	Chop_Wick_Draw(this, FIXED_DEC(184,1) - fx, FIXED_DEC(-26,1) - fy);
	
	Stage_DrawTex(&this->tex_fireplace, &fireplace_src, &fireplace_dst, stage.camera.bzoom);
	Stage_DrawTex(&this->tex_junk, &table_src, &table_dst, stage.camera.bzoom);
	Stage_DrawTex(&this->tex_junk, &chair_src, &chair_dst, stage.camera.bzoom);
	Stage_DrawTex(&this->tex_floor, &floorl_src, &floorl_dst, stage.camera.bzoom);
	Stage_DrawTex(&this->tex_floor, &floorr_src, &floorr_dst, stage.camera.bzoom);
	Stage_DrawTex(&this->tex_back0, &halll_src, &halll_dst, stage.camera.bzoom);
	Stage_DrawTex(&this->tex_back1, &hallr_src, &hallr_dst, stage.camera.bzoom);

	if (stage.stage_id == StageId_1_2)
	{
		switch (stage.song_step)
		{
			case 383:
			{
				fademode = 0;
				fadepurp = 1;
				fadespeed = FIXED_DEC(81,1);
				break;
			}
			case 511:
			{
				purpout1 = fadepurp;
				purpout2 = fadeextra2;
				purpout3 = fadeextra3;
				fademode = 1;
				break;
			}
			case 703:
			{
				fademode = 0;
				fadepurp = 1;
				fadeextra2 = 0;
				fadeextra3 = 0;
				break;
			}
			case 960:
			{
				redspeed = FIXED_DEC(8,1);
				fadered = FIXED_DEC(1,1);
				break;
			}
			case 1023:
			{
				purpout1 = fadepurp;
				purpout2 = fadeextra2;
				purpout3 = fadeextra3;
				fademode = 1;
				break;
			}
			case 1024:
			{
				redspeed = FIXED_DEC(20,1);
				redmode = 1;
				break;
			}
		}
	}

	if (stage.stage_id == StageId_1_5)
	{
		switch (stage.song_step)
		{
			case 10:
			{
				fadeblack = 1;
				fadeextra = FIXED_DEC(0,1);
				fadespeed = FIXED_DEC(46,10);
				break;
			}
			case 103:
			{
				fademode = 1;
				fadeblack2 = fadeblack;
				fadespeed = FIXED_DEC(80,1);
				break;
			}
		}
	}


}

void Back_Chop_DrawRed(StageBack *back)
{
	Back_Chop *this = (Back_Chop*)back;

	if (stage.stage_id == StageId_1_2)
	{
		if (fadered > 0 && redmode == 0)
		{
			if (fadered >= FIXED_DEC(255,1))
			{
				//This sucks
				static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
				u8 flash_col = fadered >> FIXED_SHIFT;
				u8 flash_col2 = fadered2 >> FIXED_SHIFT;
				Gfx_BlendRect(&flash, flash_col, 0, flash_col2, 1);
			}
			else
			{
				static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
				u8 flash_col = fadered >> FIXED_SHIFT;
				u8 flash_col2 = fadered2 >> FIXED_SHIFT;
				Gfx_BlendRect(&flash, flash_col, 0, flash_col2, 1);
				if (fadered < FIXED_DEC(255,1))
					fadered += FIXED_MUL(redspeed, timer_dt*3); 
				if (fadered2 < FIXED_DEC(55,1))
					fadered2 += FIXED_MUL(redspeed, timer_dt*3);
			}
		}

		if (fadered > 0 && redmode == 1)
		{
			static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
			u8 flash_col = fadered >> FIXED_SHIFT;
			u8 flash_col2 = fadered2 >> FIXED_SHIFT;
			Gfx_BlendRect(&flash, flash_col, 0, flash_col2, 1);
			if (fadered > FIXED_DEC(0,1))
				fadered -= FIXED_MUL(redspeed, timer_dt*3);
			if (fadered2 > FIXED_DEC(1,1)) //no more blue flashbangs please
				fadered2 -= FIXED_MUL(redspeed, timer_dt*3);
		}
	}
}

void Back_Chop_Free(StageBack *back)
{
	Back_Chop *this = (Back_Chop*)back;
	
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Chop_New(void)
{
	//Allocate background structure
	Back_Chop *this = (Back_Chop*)Mem_Alloc(sizeof(Back_Chop));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = Back_Chop_DrawRed;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Chop_DrawBG;
	this->back.free = Back_Chop_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\PSYCHE\\COOL.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_floor, Archive_Find(arc_back, "floor.tim"), 0);
	Gfx_LoadTex(&this->tex_junk, Archive_Find(arc_back, "junk.tim"), 0);
	Gfx_LoadTex(&this->tex_fireplace, Archive_Find(arc_back, "fplace.tim"), 0);
	Gfx_LoadTex(&this->tex_back0p, Archive_Find(arc_back, "back0p.tim"), 0);
	Gfx_LoadTex(&this->tex_back1p, Archive_Find(arc_back, "back1p.tim"), 0);
	Gfx_LoadTex(&this->tex_floorp, Archive_Find(arc_back, "floorp.tim"), 0);
	Gfx_LoadTex(&this->tex_junkp, Archive_Find(arc_back, "junkp.tim"), 0);
	Gfx_LoadTex(&this->tex_fireplacep, Archive_Find(arc_back, "fplacep.tim"), 0);
	Mem_Free(arc_back);

	Gfx_SetClear(0, 0, 0);

	Animatable_Init(&this->fire_animatable, fire_anim);
	Animatable_SetAnim(&this->fire_animatable, 0);
	
	Animatable_Init(&this->firep_animatable, firep_anim);
	Animatable_SetAnim(&this->firep_animatable, 0);

	Animatable_Init(&this->steam_animatable, steam_anim);
	Animatable_SetAnim(&this->steam_animatable, 0);

	Animatable_Init(&this->wick_animatable, wick_anim);
	Animatable_SetAnim(&this->wick_animatable, 0);
	
	Animatable_Init(&this->wickp_animatable, wickp_anim);
	Animatable_SetAnim(&this->wickp_animatable, 0);

	fademode = 0;
	fadeblack = 0;
	fadeblack2 = 0;

	fadepurp = 0;
	fadeextra2 = 0;
	fadeextra3= 0;

	purpout1 = 0;
	purpout2 = 0;
	purpout3 = 0;

	fadered = 0;
	fadered2 = 0;
	redmode = 0;
	redspeed = 0;
	
	
	
	return (StageBack*)this;
}
