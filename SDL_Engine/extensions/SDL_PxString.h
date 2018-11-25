//By lxfly2000

#ifndef __SDL_PxString_H__
#define __SDL_PxString_H__

namespace ASCII_px_6x8 {
	extern const unsigned ch_count;
	extern const int px_count_horizonal, px_count_vertical;

	//绘制单个字符的点阵
	//字符不存在返回 -1, 否则为 0
	extern int DrawPxChar(SDL_Renderer *renderer, unsigned ch, int x, int y, int pxsize);

	//绘制点阵字符串
	//str: 要显示的字符串
	//strsize: 字符串长度（通过 SDL_arraysize 获取）
	//x, y: 文字左上角的座标
	//pxsize: 单个像素的边长
	//color: 填充颜色
	//multiline: 是否允许换行（默认允许）
	//字符不存在返回 -1, 否则为 0
	extern int DrawPxString(SDL_Renderer *renderer, const char *str, int strsize, int x, int y, int pxsize, const SDL_Color color, bool multiline = true);
}

#endif
