#include "..\include\CThing.h"

CThing::CThing(){

}

CThing::~CThing(){

}

void CThing::Update(double timeDifference, CGameData &data){

  if(m_go.gd.frameMax > 1){
    m_go.gd.animTime += timeDifference;
    if(m_go.gd.animTime > m_go.gd.updateInterval){
      m_go.gd.animTime = 0;
      m_go.gd.frameCount++;
      if(m_go.gd.frameCount > m_go.gd.frameMax - 1)
        m_go.gd.frameCount = 0;
    }
    m_go.gd.frameCount = m_go.gd.frameCount;//assign gi to gi2 framecount
  }

}

void CThing::Render(CGraphics &con, CGameData &data){
  CLog *pLog = CLog::Instance();


  if(data.m_viewMode == 0){ //left isometric view

    float px = -m_go.positionX + data.m_posX;
    float py = -m_go.positionY + data.m_posY + m_go.positionZ;

    if(px > 0 && px < 1024 && py > 0 && py < 768){
      GRAPHIC_IMAGE gi = con.GetSprite(m_go.gd.vecSpriteID[0]);
      gi.updateInterval = m_go.gd.updateInterval;
      gi.animTime = m_go.gd.animTime;
      gi.frameCount = m_go.gd.frameCount;
      gi.frameMax = m_go.gd.frameMax;
      con.RenderSprite(gi, px + m_go.gd.xOffset ,py + m_go.gd.yOffset , 255, 255, 255);
    }
  }
}

void CThing::SetObject(GAME_OBJECT game_object){

}

bool CThing::RenderEffects(int &targetX,int &targetY, int &weaponType){
return true;
}

int CThing::GetLayer(){
  return 0;
}

GAME_OBJECT CThing::GetObjectData(){
  GAME_OBJECT go;
  return go;
}