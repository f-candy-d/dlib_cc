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
{
	Init(Sprite::create(), nullptr);
}

Renderer::Renderer(const Buffer& buffer)
:sprite_table_(buffer.width(), buffer.height())
,origin_(0.0, 0.0)
,texture_size_(0.0, 0.0)
,canvas_(nullptr)
,texture_atlas_src_()
,texture_position_map_()
{
	Init(Sprite::create(), nullptr);
}

Renderer::Renderer(const Buffer& buffer, Sprite* def_val)
:sprite_table_(buffer.width(), buffer.height())
,origin_(0.0, 0.0)
,texture_size_(0.0, 0.0)
,canvas_(nullptr)
,texture_atlas_src_()
,texture_position_map_()
{
	Init(def_val, nullptr);
}

Renderer::Renderer(const Buffer& buffer, Sprite* def_val, Layer* canvas)
:sprite_table_(buffer.width(), buffer.height())
,origin_(0.0, 0.0)
,texture_size_(0.0, 0.0)
,canvas_(nullptr)
,texture_atlas_src_()
,texture_position_map_()
{
	Init(def_val, canvas);
}

int Renderer::RenderBuffer(const Buffer& buffer)
{
	int rendered = 0;
	return rendered;
}

void Renderer::SetCanvas(cocos2d::Layer* canvas)
{
	RemoveSpritesFromCanvas();
	canvas_ = canvas;
	AddSpritesToCanvas();
}

void Renderer::Init(Sprite* def_val, Layer* canvas)
{
	// NOTE : DO NOT change the order of the following methods!
	FillSpriteTable(def_val);
	SetCanvas(canvas);
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

void Renderer::AddSpritesToCanvas()
{
	if(canvas_ != nullptr)
	{
		for(auto& sp : sprite_table_)
		{
			canvas_->addChild(sp);
		}
	}
}

void Renderer::RemoveSpritesFromCanvas()
{
	if(canvas_ != nullptr)
	{
		for(auto& sp : sprite_table_)
		{
			canvas_->removeChild(sp, true);
		}
	}
}

} /* namespace lts */
} /* namespace dlib_cc */
