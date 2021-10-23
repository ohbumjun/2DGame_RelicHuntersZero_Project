#pragma once
#include "UIWindow.h"
class CUISelect :
    public CUIWindow
{
	friend class CScene;

private:
	CUISelect();
	virtual ~CUISelect();
private:
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private:
	void StartClick();
	void ExitClick();
};

