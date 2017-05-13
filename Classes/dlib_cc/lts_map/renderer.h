#ifndef DLIB_CC_LTS_MAP_RENDERER_H
#define DLIB_CC_LTS_MAP_RENDERER_H

#include "../../../cocos2d/cocos/cocos2d.h"
#include "../fast_shift_table/fast_shift_table.h"
#include "buffer.h"
#include <string>
#include <unordered_map>

namespace dlib_cc
{
namespace lts_map_unit
{

class Renderer
{
public:
	Renderer();
	Renderer(const Buffer& buffer);
	Renderer(const Buffer& buffer, cocos2d::Sprite* def_val);
	int RenderBuffer(const Buffer& buffer);

private:
	dlib_cc::fast_shift_table<cocos2d::Sprite*> sprite_table_;
	cocos2d::Vec2 origin_;
	cocos2d::Size texture_size_;
	cocos2d::Layer* canvas_;
	std::string texture_atlas_src_;
	// key = texture id, value = position
	std::unordered_map<int, cocos2d::Vec2> texture_position_map_;
	bool added_sprites_to_canvas_;

	void Init(cocos2d::Sprite* def_val);
	void FillSpriteTable(cocos2d::Sprite* def_val);
};

} /* namespace lts_map_unit */
} /* namespace dlib_cc */

#endif
