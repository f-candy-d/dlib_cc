/**********************************************************************************
 *
 * dlib_cc::lts_map_unit::Renderer class
 *
 **********************************************************************************/
#include "./renderer.h"

using cocos2d::Sprite;
using cocos2d::Layer;

namespace dlib_cc
{
namespace lts_map_unit
{

Renderer::Renderer()
:sprite_table_(0, 0)
,origin_(0.0, 0.0)
,texture_size_(0.0, 0.0)
,canvas_(nullptr)
,texture_atlas_src_()
,texture_position_map_()
,added_sprites_to_canvas_(false)
{
	Init(Sprite::create());
}

Renderer::Renderer(const Buffer& buffer)
:sprite_table_(buffer.width(), buffer.height())
,origin_(0.0, 0.0)
,texture_size_(0.0, 0.0)
,canvas_(nullptr)
,texture_atlas_src_()
,texture_position_map_()
,added_sprites_to_canvas_(false)
{
	Init(Sprite::create());
}

Renderer::Renderer(const Buffer& buffer, Sprite* def_val)
:sprite_table_(buffer.width(), buffer.height())
,origin_(0.0, 0.0)
,texture_size_(0.0, 0.0)
,canvas_(nullptr)
,texture_atlas_src_()
,texture_position_map_()
,added_sprites_to_canvas_(false)
{
	Init(def_val);
}

int Renderer::RenderBuffer(const Buffer& buffer)
{
	int rendered = 0;
	return rendered;
}

void Renderer::Init(Sprite* def_val)
{
	FillSpriteTable(def_val);
}

void Renderer::FillSpriteTable(Sprite* def_val)
{
	assert(def_val);

	for(auto& sp : sprite_table_)
	{
		sp = Sprite::create();
		sp->retain();
		sp->setPosition(def_val->getPosition());
		sp->setAnchorPoint(def_val->getAnchorPoint());
	}
}

void AddSpritesToCanvas()
{
	assert(canvas);

	if(!added_sprites_to_canvas_)
	{
		for(auto& sp : sprite_table_)
		{
			canvas->addChild(sp);
		}

		added_sprites_to_canvas_ = true;
	}
}

} /* namespace lts */
} /* namespace dlib_cc */
