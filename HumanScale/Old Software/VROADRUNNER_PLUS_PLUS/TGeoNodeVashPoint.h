// TGeoNodeVashPoint.h: TGeoNodeVashPoint クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGEONODEVASHPOINT_H__B826ADDB_46F5_4716_90B1_C18E64A2DE3D__INCLUDED_)
#define AFX_TGEONODEVASHPOINT_H__B826ADDB_46F5_4716_90B1_C18E64A2DE3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeoNode.h"

class TGeoNodeVashPoint : public TGeoNode  
{
public:
	bool isDraw;
public:
	virtual void Draw();
	TGeoNodeVashPoint();
	virtual ~TGeoNodeVashPoint();

};

#endif // !defined(AFX_TGEONODEVASHPOINT_H__B826ADDB_46F5_4716_90B1_C18E64A2DE3D__INCLUDED_)
