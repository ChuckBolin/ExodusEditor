#ifndef CTHING_H
#define CTHING_H

#include <vector>
#include <string>
#include "CObject.h"
#include "CGraphics.h"
#include "CGameData.h"
#include "CLog.h"

class CThing : public CObject{
public:
  CThing();
  ~CThing();
  void Update(double timeDifference, CGameData &data);
  void Render(CGraphics &con, CGameData &data);
  void SetObject(GAME_OBJECT game_object);
  bool RenderEffects(int &targetX,int &targetY, int &weaponType);
  int GetLayer();
  GAME_OBJECT GetObjectData();

private:
};


#endif;
