#include "..\include\CObjectManager.h"

CObjectManager::CObjectManager(){
  m_groupID = 0;
}

CObjectManager::~CObjectManager(){

}

void CObjectManager::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  for(size_t i = 0; i < m_pObjects.size(); ++i)
    m_pObjects[i]->Update(timeDifference, data);
}
void CObjectManager::Render(CGraphics &con,  CGameData &data, CConfigData &cfg){
  for(size_t i = 0; i < m_pObjects.size(); ++i)
    m_pObjects[i]->Render(con, data);
}

void CObjectManager::Render(CGraphics &con,  CGameData &data, CConfigData &cfg, int row, int col){
  for(size_t i = 0; i < m_pObjects.size(); ++i){
    if(m_pObjects[i]->m_go.tileRow == row && m_pObjects[i]->m_go.tileCol == col)
      m_pObjects[i]->Render(con, data);
  }
}

bool CObjectManager::LoadObjectDefinitions(CGraphics &con, CConfigData &cfg){
  
  CLog *pLog = CLog::Instance();
  pLog->Log("Loading object definitions");

  m_objectDefinition.clear();
  GAME_OBJECT_DEFINITION gd;

  gd.vecSpriteID.clear();
  int spriteID1;
  int spriteID2;
  int soundID;
  std::string parameter;
  CCSVReader csv;
  int numberSprites;
  int numberSounds;
  csv.LoadFile(cfg.GameObjectFile);
  if(csv.GetTableSize() < 1)
    return false;

  for(size_t i = 0; i < csv.GetTableSize(); ++i){
    //parameter = csv.GetTerm(i, 0);
    if(csv.GetNumberParameters(i) > 12){
    //if(parameter == "tent" && csv.GetNumberParameters(i) > 12){
      //pLog->Log("loaded...", parameter);
      //gd.name = parameter;
      parameter = csv.GetTerm(i, 0);
      gd.id = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 1);
      gd.groupID = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 2);
      gd.rating = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 3);
      gd.footprint = atoi(parameter.c_str());
      
      if(gd.footprint == 0){ //circle
        parameter = csv.GetTerm(i, 4);
        gd.radius = atof(parameter.c_str());
        parameter = csv.GetTerm(i, 5);
        gd.id = 0.0f;
      }
      else{                 //rectangle
        parameter = csv.GetTerm(i, 6);
        gd.widthEW = atof(parameter.c_str());
        parameter = csv.GetTerm(i, 7);
        gd.widthNS = atof(parameter.c_str());
      }
      
      parameter = csv.GetTerm(i, 8);
      numberSprites = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 9);
      numberSounds = atof(parameter.c_str());
      
      gd.vecSpriteID.clear();
      gd.vecSpriteID2.clear();
      if(numberSprites > 0){
        for(int j= 0; j < numberSprites; j += 2){
          parameter = csv.GetTerm(i, 10 + j);
          spriteID1 = atoi(parameter.c_str());
          parameter = csv.GetTerm(i, 10 + j + 1);
          spriteID2 = atoi(parameter.c_str());
          gd.vecSpriteID.push_back(spriteID1);
          gd.vecSpriteID2.push_back(spriteID2);
        }
      }

      gd.vecSoundID.clear();
      if(numberSounds > 0){
        for(int j= 0; j < numberSounds; ++j){
          parameter = csv.GetTerm(i, 10 + (numberSprites * 2) + j);
          soundID = atoi(parameter.c_str());
          gd.vecSoundID.push_back(soundID);
        }
      }
      
      //x and y offset
      parameter = csv.GetTerm(i, 10 + (numberSprites * 2) + numberSounds);
      gd.xOffset = atof(parameter.c_str());
      parameter = csv.GetTerm(i, 10 + (numberSprites * 2) + numberSounds + 1);
      gd.yOffset = atof(parameter.c_str());

      //load name
      parameter = csv.GetTerm(i, 10 + (numberSprites * 2) + numberSounds + 2);
      gd.name = parameter;
      m_objectDefinition.push_back(gd);
    }
  }

  pLog->Log("Number of object definitions loaded", m_objectDefinition.size());
  //for(size_t i = 0; i < m_objectDefinition.size(); ++i){
  //  pLog->Log("&&&&&&&&&&&&&&&&&&&&& ", m_objectDefinition[i].vecSpriteID[0]);
  //}

  return true;
}
void CObjectManager::LoadLevelFile(std::string sFilename, CGraphics &con){
  CLog *pLog = CLog::Instance();
  std::string parameter;
  GAME_OBJECT go;
  GRAPHIC_IMAGE gi;

  CCSVReader csv;
  csv.LoadFile(sFilename);
  if(csv.GetTableSize() < 1)
    return;

  m_pObjects.clear();
  
  for(size_t i = 0; i < csv.GetTableSize(); ++i){
    if(csv.GetNumberParameters(i)== 4){
      parameter = csv.GetTerm(i, 0);
      if(parameter != "tile"){
        
        for(size_t j = 0; j < m_objectDefinition.size(); ++j){
          if(m_objectDefinition[j].name == parameter){
            //pLog->Log("loaded object", parameter);
            parameter = csv.GetTerm(i, 1);
            
            go.positionX = atof(parameter.c_str());
            parameter = csv.GetTerm(i, 2);
            go.positionY = atof(parameter.c_str());
            parameter = csv.GetTerm(i, 3);
            go.positionZ = atof(parameter.c_str());
            m_pObjects.push_back(new CThing);
            m_pObjects[m_pObjects.size() - 1]->m_go.gd = m_objectDefinition[j];
            gi = con.GetSprite(m_objectDefinition[j].vecSpriteID[0]);
            m_pObjects[m_pObjects.size() - 1]->m_go.positionX = go.positionX;
            m_pObjects[m_pObjects.size() - 1]->m_go.positionY = go.positionY;
            m_pObjects[m_pObjects.size() - 1]->m_go.positionZ = go.positionZ;
            
            m_pObjects[m_pObjects.size() - 1]->m_go.tileRow = CalcIsoRow(go.positionX, go.positionY, 0) + 2;
            m_pObjects[m_pObjects.size() - 1]->m_go.tileCol = CalcIsoCol(go.positionX, go.positionY, 0) - 1;
            //pLog->Log("calc iso row", m_pObjects[m_pObjects.size() - 1]->m_go.tileRow);
            //pLog->Log("calc iso col", m_pObjects[m_pObjects.size() - 1]->m_go.tileCol);
            m_pObjects[m_pObjects.size() - 1]->m_go.gd.animTime = 0;
            m_pObjects[m_pObjects.size() - 1]->m_go.gd.frameMax = gi.frameMax;
            m_pObjects[m_pObjects.size() - 1]->m_go.gd.frameCount = (rand() % gi.frameMax) + 1;
            m_pObjects[m_pObjects.size() - 1]->m_go.gd.updateInterval = gi.updateInterval;
/*
            pLog->Log("Loaded object");
            pLog->Log("X", m_pObjects[m_pObjects.size() - 1]->m_go.positionX - 64);
            pLog->Log("Y", m_pObjects[m_pObjects.size() - 1]->m_go.positionY - 32);
            pLog->Log("Z", m_pObjects[m_pObjects.size() - 1]->m_go.positionZ);
            pLog->Log("xOffset", m_pObjects[m_pObjects.size() - 1]->m_go.gd.xOffset);
            pLog->Log("yOffset", m_pObjects[m_pObjects.size() - 1]->m_go.gd.yOffset);
*/

          }
        }
      }
    }
  }

  for(size_t i = 0; i < m_pObjects.size(); ++i)
    pLog->Log(m_pObjects[i]->m_go.gd.id, m_pObjects[i]->m_go.gd.name);

  if(m_pObjects.size() > 1)
  pLog->Log(sFilename, " loaded objects",m_pObjects.size());
}

void CObjectManager::SaveLevelFile(std::string sFilename){
  
  std::ofstream file(sFilename.c_str(), std::ios::app);
  if(file){
    for(size_t i = 0; i < m_pObjects.size(); ++i){
      file << m_pObjects[i]->m_go.gd.name << ", " 
           << m_pObjects[i]->m_go.positionX << ", " 
           << m_pObjects[i]->m_go.positionY << ", " 
           << m_pObjects[i]->m_go.positionZ << std::endl;
    }
  }
  file.close();
}

int CObjectManager::CalcIsoRow(float x, float y, int viewMode){//verified
  if(viewMode == 0){//left isometric
    return (int)( ((y + 1613)/32 - (x + 3134)/64)/-2);
  }
  else if(viewMode == 1){//right isometric
    return (int)(((x + 3134)/-64 + (y+1613)/-32)/2);
  }
  return 0;
}

int CObjectManager::CalcIsoCol(float x, float y, int viewMode){//verified
  if(viewMode == 0){//left isometric
    return (int)(((x + 3134)/-64 + (y + 1613)/-32)/2);
  }
  else if(viewMode == 1){//right isometric
    return (int)(50 - ((y + 1613)/32 - (x + 3134)/64)/-2);
    
  }
  return 0;
}


void CObjectManager::AddObject(float x, float y, float z, CGraphics &con){
  CLog *pLog = CLog::Instance();
  float width = 0;
  float height = 0;
  GRAPHIC_IMAGE gi;
  for(size_t i = 0; i < m_groupObjects.size();++i){
    if(m_groupObjects[i].id == m_objectID){//i == m_objectID){//
      m_pObjects.push_back(new CThing);
      m_pObjects[m_pObjects.size() - 1]->m_go.gd = m_groupObjects[i];
      gi = con.GetSprite(m_groupObjects[i].vecSpriteID[0]);
      m_pObjects[m_pObjects.size() - 1]->m_go.gd.animTime = 0;
      m_pObjects[m_pObjects.size() - 1]->m_go.gd.frameCount = gi.frameCount;
      m_pObjects[m_pObjects.size() - 1]->m_go.gd.frameMax = gi.frameMax;
      m_pObjects[m_pObjects.size() - 1]->m_go.gd.updateInterval = gi.updateInterval;
      width = gi.width;
      height = gi.height;
      m_pObjects[m_pObjects.size() - 1]->m_go.positionX = x + width/2;
      m_pObjects[m_pObjects.size() - 1]->m_go.positionY = y + height/2;
      m_pObjects[m_pObjects.size() - 1]->m_go.positionZ = z;       
    }
  }
}

void CObjectManager::AddObject(float x, float y, float z, int row, int col, CGraphics &con){
  CLog *pLog = CLog::Instance();
 
  float width = 0;
  float height = 0;
  GRAPHIC_IMAGE gi;
  for(size_t i = 0; i < m_groupObjects.size();++i){
    if(m_groupObjects[i].id == m_objectID){//i == m_objectID){//
      m_pObjects.push_back(new CThing);
      m_pObjects[m_pObjects.size() - 1]->m_go.gd = m_groupObjects[i];
      gi = con.GetSprite(m_groupObjects[i].vecSpriteID[0]);
      m_pObjects[m_pObjects.size() - 1]->m_go.gd.animTime = 0;
      m_pObjects[m_pObjects.size() - 1]->m_go.gd.frameCount = gi.frameCount;
      m_pObjects[m_pObjects.size() - 1]->m_go.gd.frameMax = gi.frameMax;
      m_pObjects[m_pObjects.size() - 1]->m_go.gd.updateInterval = gi.updateInterval;
      width = gi.width;
      height = gi.height;
      m_pObjects[m_pObjects.size() - 1]->m_go.positionX = x + width/2;
      m_pObjects[m_pObjects.size() - 1]->m_go.positionY = y + height/2;
      m_pObjects[m_pObjects.size() - 1]->m_go.positionZ = z;            
      //m_pObjects[m_pObjects.size() - 1]->m_go.gd.yOffset = -100;//z - height;
      m_pObjects[m_pObjects.size() - 1]->m_go.tileRow = row;
      m_pObjects[m_pObjects.size() - 1]->m_go.tileCol = col;
      pLog->Log("Added object");
      pLog->Log("X", m_pObjects[m_pObjects.size() - 1]->m_go.positionX);
      pLog->Log("Y", m_pObjects[m_pObjects.size() - 1]->m_go.positionY);
      pLog->Log("Z", m_pObjects[m_pObjects.size() - 1]->m_go.positionZ);
      pLog->Log("xOffset", m_pObjects[m_pObjects.size() - 1]->m_go.gd.xOffset);
      pLog->Log("yOffset", m_pObjects[m_pObjects.size() - 1]->m_go.gd.yOffset);
    }
  }
}

void CObjectManager::ClearObjects(){
  m_pObjects.clear();
}


  //group objects
  //std::vector<GAME_OBJECT_DEFINITION> m_objectDefinition;
  //std::vector<GAME_OBJECT_DEFINITION> m_groupObjects;
size_t CObjectManager::GetGroupNumber(){
  return m_groupID;
}

void CObjectManager::SetGroupNumber(size_t id){
  m_groupID = id;    
  m_groupObjects.clear();
  
  for(size_t i = 0; i < m_objectDefinition.size();++i){
    if(m_objectDefinition[i].groupID == id)
      m_groupObjects.push_back(m_objectDefinition[i]);
  }
 
  if(m_groupObjects.size() < 1)
    m_objectID = -1;
  else
    m_objectID = 0;
}

int CObjectManager::GetGroupObjectSprite(size_t index){
  if(index < 0 || index > m_groupObjects.size() - 1)
    return 1;

  return m_groupObjects[index].vecSpriteID[0];
}


void CObjectManager::SetGroupTileID(size_t id){
  if(id < 0 || id > m_groupObjects.size() - 1)
    return;
  //for(size_t i = 0; i < m_groupObjects.size();++i){
    

  //}
  
  m_objectID = m_groupObjects[id].id;  
}

void CObjectManager::RemoveObject(float x, float y){
  for(size_t i = 0; i < m_pObjects.size(); ++i){
    if(m_pObjects[i]->m_go.positionX > x - 100 && m_pObjects[i]->m_go.positionX < x + 100 &&
      m_pObjects[i]->m_go.positionY > y - 100 && m_pObjects[i]->m_go.positionY < y + 100){
        m_pObjects.erase(m_pObjects.begin() + i);
        return; 
    }
  }
}