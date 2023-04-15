#pragma once

#ifndef __SPEECHBUBBLE_H__
#define __SPEECHBUBBLE_H__

#include "Obj.h"
class CSpeechBubble : public CObj
{
public:
	explicit CSpeechBubble();
	virtual ~CSpeechBubble();

public:
	virtual HRESULT Ready_Object() override;
	virtual int Update_Object() override;
	virtual void LateUpdate_Object() override;
	virtual void Render_Object() override;
	virtual void Release_Object() override;
};


#endif