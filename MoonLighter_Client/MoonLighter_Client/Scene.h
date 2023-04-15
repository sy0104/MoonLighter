#pragma once
class CScene abstract
{
public:
	explicit CScene();
	virtual ~CScene();
public:
	virtual void Ready_Scene()		PURE;
	virtual void Update_Scene()		PURE;
	virtual void LateUpdate_Scene() PURE;
	virtual void Render_Scene()		PURE;
	virtual void Release_Scene()	PURE;
};

