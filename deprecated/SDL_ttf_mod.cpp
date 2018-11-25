#include "SDL_ttf_mod.h"
#include "SDL_endian.h"
#include "SDL.h"

void TTF_SetFontSizeMod(TTF_Font *font, int ptsize)  
{  
    FT_Fixed scale;  
    FT_Error error;  
    FT_Face face;  
      
    face = font->face;  
      
    /* Make sure that our font face is scalable (global metrics) */  
    if ( FT_IS_SCALABLE(face) ) {  
  
        /* Set the character size and use default DPI (72) */  
        error = FT_Set_Char_Size( font->face, 0, ptsize * 64, 0, 0 );  
            if( error ) {  
            TTF_SetFTError( "Couldn't set font size", error );  
            TTF_CloseFont( font );  
            return ;  
      }  
  
      /* Get the scalable font metrics for this font */  
      scale = face->size->metrics.y_scale;  
      font->ascent  = FT_CEIL(FT_MulFix(face->ascender, scale));  
      font->descent = FT_CEIL(FT_MulFix(face->descender, scale));  
      font->height  = font->ascent - font->descent + /* baseline */ 1;  
      font->lineskip = FT_CEIL(FT_MulFix(face->height, scale));  
      font->underline_offset = FT_FLOOR(FT_MulFix(face->underline_position, scale));  
      font->underline_height = FT_FLOOR(FT_MulFix(face->underline_thickness, scale));  
        
    } else {  
        /* Non-scalable font case.  ptsize determines which family 
         * or series of fonts to grab from the non-scalable format. 
         * It is not the point size of the font. 
         * */  
        if ( ptsize >= font->face->num_fixed_sizes )  
            ptsize = font->face->num_fixed_sizes - 1;  
        font->font_size_family = ptsize;  
        error = FT_Set_Pixel_Sizes( face,  
                face->available_sizes[ptsize].height,  
                face->available_sizes[ptsize].width );  
        /* With non-scalale fonts, Freetype2 likes to fill many of the 
         * font metrics with the value of 0.  The size of the 
         * non-scalable fonts must be determined differently 
         * or sometimes cannot be determined. 
         * */  
        font->ascent = face->available_sizes[ptsize].height;  
        font->descent = 0;  
        font->height = face->available_sizes[ptsize].height;  
        font->lineskip = FT_CEIL(font->ascent);  
        font->underline_offset = FT_FLOOR(face->underline_position);  
        font->underline_height = FT_FLOOR(face->underline_thickness);  
    }  
  
    if ( font->underline_height < 1 ) {  
        font->underline_height = 1;  
    }  
  
    font->glyph_italics *= font->height;  
      
    Flush_Cache(font); //这个非常重要  
}