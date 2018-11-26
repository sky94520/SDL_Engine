#ifndef __SDL_Label_H__
#define __SDL_Label_H__

#include <string>
#include "SEProtocols.h"
#include "SENode.h"

NS_SDL_BEGIN
class Label : public Node, public LabelProtocol
{
protected:
	std::string _text;
public:
	virtual void setString(const std::string&label) = 0;
	virtual std::string getString() const { return _text;}
};
NS_SDL_END
#endif
