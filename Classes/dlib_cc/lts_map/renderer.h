#ifndef DLIB_CC_LTS_MAP_RENDERER_H
#define DLIB_CC_LTS_MAP_RENDERER_H

#include "../../../cocos2d/cocos/cocos2d.h"
#include "../fast_shift_table/fast_shift_table.h"
#include "buffer.h"

namespace dlib_cc
{
	namespace lts_map_unit
	{
		class Renderer
		{
		public:
			Renderer();
			Renderer(int width, int height);
			Renderer(int width, int height, cocos2d::Sprite* def_val);
			void Resize(int width, int height);
			int RenderBuffer(const Buffer& buffer);

		private:
			bool auto_resize_;
			dlib_cc::fast_shift_table<cocos2d::Sprite*> sprites_;

		};
	} /* namespace lts_map_unit */
} /* namespace dlib_cc */

#endif
