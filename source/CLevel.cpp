#include "..\include\CLevel.h"

CLevel::CLevel(){
  //isometric initialization
  m_viewMode = 0; //normal iso
  m_offsetX = (MAX_COLS - 1) * 64;// 3136;
  m_offsetY = m_offsetX/2;
  m_tileID = 0;
  m_isoRow = 0;
  m_isoCol = 0;
  m_tileWidth = 128;
  m_tileHeight = 64;
  m_bDrawGrid = true;
  m_currentRating = 1;
  m_levelScale = 1;
  m_groupIndex = 0;                       //current group index number
  m_groupHighestIndex = 0;                //group index numbers do not need to be successive
  m_groupLowestIndex = 1000;
  m_oscillate = 1;
  m_oscUp = false;
}

CLevel::~CLevel(){

}

void CLevel::Update(float posX, float posY, float mouseX, float mouseY, double timeDifference, CGameData &data){
  float dx = 0;
  float dy = 0;   

  if(m_oscUp == false){
    m_oscillate += timeDifference * 5.0;
    if(m_oscillate > 8)
      m_oscUp = true;
  }
  else{
    m_oscillate -= timeDifference * 5.0;
    if(m_oscillate < 0)
      m_oscUp = false;
  }

  if(data.m_bShowMap == true){
    if(mouseX > 263 && mouseX < 763 && mouseY > 104 && mouseY < 604){
      float col = ((mouseX - 262)/10);
      float row = ((mouseY - 104)/10);
      m_isoRow = row;
      m_isoCol = col;
     }
   }
  else{
    if(m_viewMode == 0){  //drawn to bottom-left
      dx = (mouseX - posX) - m_offsetX;
      dy = (mouseY - posY) - m_offsetY - data.m_windowedYOffset;
      m_yIntercept1 = (float)(dy - dx * 0.5);
      m_yIntercept2 = (float)(m_offsetX + dy + dx * 0.5);
      m_isoRow = (float)((m_yIntercept1 - 32)/64);
      m_isoCol = (float)((m_yIntercept2 - 32)/64) - (MAX_COLS - 1);
    }
    else if(m_viewMode == 1){  //drawn to bottom-right
      dx = (mouseX - posX) - m_offsetX;
      dy = (mouseY - posY) - m_offsetY - data.m_windowedYOffset;
      m_yIntercept1 = (float)(dy - dx * 0.5);
      m_yIntercept2 = (float)(m_offsetX + dy + dx * 0.5);
      m_isoCol = (float)(MAX_COLS - (int)((m_yIntercept1 - 32)/64) - 1);
      m_isoRow = (float)(((m_yIntercept2 - 32)/64) - (MAX_ROWS - 1));
    }
  }

  //used for rendering
  m_posX = posX;
  m_posY = posY;

  //animate sprites if they exist...use left isometric for timing
  for(int i = 0; i < MAX_ROWS; ++i){
    for(int j = 0; j < MAX_COLS; ++j){
      if(m_tiles[j][i].gi.frameMax > 1){
        m_tiles[j][i].gi.animTime += timeDifference;
        if(m_tiles[j][i].gi.animTime > m_tiles[j][i].gi.updateInterval){
          m_tiles[j][i].gi.animTime = 0;
          m_tiles[j][i].gi.frameCount++;
          if(m_tiles[j][i].gi.frameCount > m_tiles[j][i].gi.frameMax - 1)
            m_tiles[j][i].gi.frameCount = 0;
        }
        m_tiles[j][i].gi2.frameCount = m_tiles[j][i].gi.frameCount;//assign gi to gi2 framecount
      }

      //oscillate water tiles = 9
      if(m_tiles[j][i].id == 9){
        m_tiles[j][i].yOffset = m_tiles[j][i].yNormalOffset;
        m_tiles[j][i].yOffset = m_oscillate + m_tiles[j][i].yOffset;

      }
    }
  }
}

void CLevel::RenderMap(CGraphics &con, CConfigData &cfg){
  LEVEL_TILE tile; 
  float px;
  float py;
  float x;
  float y;
  GRAPHIC_IMAGE gi;
  gi = con.GetSprite(2020);
  x = 263;//cfg.ScreenWidth/2 - gi.width/2;
  y = 50;//50;
  con.RenderSprite(gi, x, y, 255, 255, 255);
  
  for(int i = 0; i < MAX_ROWS; ++i){
    for(int j = 0; j < MAX_COLS; ++j){
      px = x + 10 +(j * 10);
      py = y + 54 + (i * 10);
      tile = m_tiles[i][j];
      tile.gi.sourceX += tile.gi.width/2 - 5;
      tile.gi.width = 10;
      tile.gi.height = 10;
      tile.gi.frameMax = 1;
      con.RenderSprite(tile.gi, px, py, 255, 255, 255);
    }
  }
}

void CLevel::Render(CGraphics &con, CGameData &data){
  LEVEL_TILE tile;  
  float px, py;
  m_levelScale = 1.0;

  if(m_viewMode == 0){
    
    //render visible level
    for(int i = 0; i < MAX_ROWS; ++i){
      for(int j = 0; j < MAX_COLS; ++j){
        /*
        if(m_tiles[j][i].rating <= m_currentRating){
          
          px = (m_posX + m_offsetX) - (m_tileWidth/2)*m_levelScale + ((i * m_tileWidth/2))*m_levelScale - ((j * m_tileWidth/2))*m_levelScale;
          py = (m_posY +  m_offsetY) + (m_tileHeight/2)*m_levelScale + ((j * m_tileHeight/2))*m_levelScale + ((i * m_tileHeight/2))*m_levelScale + m_tiles[j][i].yOffset*m_levelScale;
          if(px > - m_tileWidth * 2 && px < 1024 + m_tileWidth * 2 && py > -m_tileHeight * 2 && py < 768 + m_tileHeight * 2){
            tile = m_tiles[j][i];//.gi.scale;
            tile.gi.scale *= m_levelScale;
            con.RenderSprite(tile.gi, px, py, 255, 255, 255);
          }
        } */     
       if(m_tiles[j][i].rating <= m_currentRating){ 
          px = m_posX + m_offsetX - m_tileWidth/2 + (i * m_tileWidth/2) - (j * m_tileWidth/2);
          py = m_posY +  m_offsetY + m_tileHeight/2 + (j * m_tileHeight/2) + (i * m_tileHeight/2) + m_tiles[j][i].yOffset;
          if(px > - m_tileWidth * 2 && px < 1024 + m_tileWidth * 2 && py > -m_tileHeight * 2 && py < 768 + m_tileHeight * 2){
            con.RenderSprite(m_tiles[j][i].gi, px, py, 255, 255, 255);
          }
        }      
        else{//<<<<<< need softcode
          tile = GetTileDefinition(0);//m_tileDefinition[0];                    
          px = m_posX + m_offsetX - m_tileWidth/2 + (i * m_tileWidth/2) - (j * m_tileWidth/2);
          py = m_posY +  m_offsetY + m_tileHeight/2 + (j * m_tileHeight/2) + (i * m_tileHeight/2) + tile.yOffset;
          if(px > - m_tileWidth * 2 && px < 1024 + m_tileWidth * 2 && py > -m_tileHeight * 2 && py < 768 + m_tileHeight * 2){
            con.RenderSprite(tile.gi, px, py, 255, 255, 255);
          }
        }
      }
    }

    //render sprite for center of 



  }
  else if(m_viewMode == 1){
    for(int i = 0; i < MAX_ROWS; ++i){
      for(int j = MAX_COLS - 1; j > -1; --j){
      
        if(m_tiles[i][j].rating <= m_currentRating){
          //if(m_tiles[i][j].sprite == 105)//water
          //  m_tiles[i][j].gi2.alpha = 150;
          px = m_posX + m_offsetX - ((MAX_COLS) * m_tileWidth/2) + (i * m_tileWidth/2) + (j * m_tileWidth/2);
          py = m_posY +  m_offsetY + ((MAX_ROWS) * m_tileHeight/2) - (j * m_tileHeight/2) + (i * m_tileHeight/2) + m_tiles[i][j].yOffset;
          if(px > - m_tileWidth * 2 && px < 1024 + m_tileWidth * 2 && py > -m_tileHeight * 2 && py < 768 + m_tileHeight * 2){
            con.RenderSprite(m_tiles[i][j].gi2, px, py, 255, 255, 255);
          }
        }  
        else{//<<<<<<<<<< need softcode
          tile = GetTileDefinition(0);//m_tileDefinition[0];                    
          px = m_posX + m_offsetX - ((MAX_COLS) * m_tileWidth/2) + (i * m_tileWidth/2) + (j * m_tileWidth/2);
          py = m_posY +  m_offsetY + ((MAX_ROWS) * m_tileHeight/2) - (j * m_tileHeight/2) + (i * m_tileHeight/2) + tile.yOffset;
          if(px > - m_tileWidth * 2 && px < 1024 + m_tileWidth * 2 && py > -m_tileHeight * 2 && py < 768 + m_tileHeight * 2){
            con.RenderSprite(tile.gi, px, py, 255, 255, 255);
          }
        }
      }
    }
  }

}

void CLevel::ClearTiles(){
  LEVEL_TILE tile;
  tile.rating = 0;
  tile.yNormalOffset = 0;
  tile.yOffset = 0;
  tile.id = -1;

  for(int i = 0; i <  MAX_ROWS; ++i){
    for(int j = 0; j < MAX_COLS; ++j){
      m_tiles[i][j] = tile;
    }
  }

}

void CLevel::AddTile(int brushsize){
  LEVEL_TILE tile;
  int nMax = 0;

  if(brushsize == 1){
    if(m_isoRow >= 0 && m_isoRow < MAX_ROWS && m_isoCol >= 0 && m_isoCol < MAX_COLS){
      if(m_tileID >= 0 && m_tileID < m_groupTiles.size()){

        m_groupTiles[m_tileID].yNormalOffset = m_groupTiles[m_tileID].yOffset;

        m_tiles[(int)m_isoRow][(int)m_isoCol] = m_groupTiles[m_tileID];
        
        //if framecount == -1 then randomize the start order
        if(m_tiles[(int)m_isoRow][(int)m_isoCol].gi.frameCount == - 1){
          nMax = m_tiles[(int)m_isoRow][(int)m_isoCol].gi.frameMax;
          if(nMax > 0){
            m_tiles[(int)m_isoRow][(int)m_isoCol].gi.frameCount = (rand() % nMax) + 1;  
          }
        }

        //if framecount == -1 then randomize the start order
        if(m_tiles[(int)m_isoRow][(int)m_isoCol].gi2.frameCount == - 1){
          nMax = m_tiles[(int)m_isoRow][(int)m_isoCol].gi2.frameMax;
          if(nMax > 0){
            m_tiles[(int)m_isoRow][(int)m_isoCol].gi2.frameCount = (rand() % nMax) + 1;  
          }
        }

      }
    }
  }
  else if(brushsize == 2){
   if(m_isoRow >= 1 && m_isoRow < MAX_ROWS - 1 && m_isoCol >= 1 && m_isoCol < MAX_COLS - 1){
      if(m_tileID >= 0 && m_tileID < m_groupTiles.size()){
        m_groupTiles[m_tileID].yNormalOffset = m_groupTiles[m_tileID].yOffset;

        m_tiles[(int)m_isoRow - 1][(int)m_isoCol - 1] = m_groupTiles[m_tileID];
        m_tiles[(int)m_isoRow - 1][(int)m_isoCol] = m_groupTiles[m_tileID];
        m_tiles[(int)m_isoRow - 1][(int)m_isoCol + 1] = m_groupTiles[m_tileID];
        m_tiles[(int)m_isoRow][(int)m_isoCol - 1] = m_groupTiles[m_tileID];
        m_tiles[(int)m_isoRow][(int)m_isoCol] = m_groupTiles[m_tileID];
        m_tiles[(int)m_isoRow][(int)m_isoCol + 1] = m_groupTiles[m_tileID];
        m_tiles[(int)m_isoRow + 1][(int)m_isoCol - 1] = m_groupTiles[m_tileID];
        m_tiles[(int)m_isoRow + 1][(int)m_isoCol] = m_groupTiles[m_tileID];
        m_tiles[(int)m_isoRow + 1][(int)m_isoCol + 1] = m_groupTiles[m_tileID];
        
      }
    }
  }
}

void CLevel::AddTileViaMap(int mx, int my, int brushsize){
  int col = (int)((mx - 272)/10);
  int row = (int)((my - 104)/10);
  
  LEVEL_TILE tile;
  int nMax = 0;

  if(brushsize == 1 && row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLS){
    if(m_tileID >= 0 && m_tileID < m_tileDefinition.size()){     
      //m_tiles[row][col] = m_tileDefinition[m_tileID];
      m_groupTiles[m_tileID].yNormalOffset = m_groupTiles[m_tileID].yOffset;
      m_tiles[row][col] = m_groupTiles[m_tileID];

      //if framecount == -1 then randomize the start order
      if(m_tiles[row][col].gi.frameCount == - 1){
        nMax = m_tiles[row][col].gi.frameMax;
        if(nMax > 0){
          m_tiles[row][col].gi.frameCount = (rand() % nMax) + 1;  
        }
      }    

      //if framecount == -1 then randomize the start order
      if(m_tiles[row][col].gi2.frameCount == - 1){
        nMax = m_tiles[row][col].gi2.frameMax;
        if(nMax > 0){
          m_tiles[row][col].gi2.frameCount = (rand() % nMax) + 1;  
        }
      }  

    }
  }

  else if(brushsize == 2 && row >= 1 && row < MAX_ROWS - 1 && col >= 1 && col < MAX_COLS - 1){
    if(m_tileID >= 0 && m_tileID < m_tileDefinition.size()){    
      m_groupTiles[m_tileID].yNormalOffset = m_groupTiles[m_tileID].yOffset;
      m_tiles[row - 1][col - 1] = m_groupTiles[m_tileID];
      m_tiles[row - 1][col] = m_groupTiles[m_tileID];
      m_tiles[row - 1][col + 1] = m_groupTiles[m_tileID];
      m_tiles[row][col - 1] = m_groupTiles[m_tileID];
      m_tiles[row][col] = m_groupTiles[m_tileID];
      m_tiles[row][col + 1] = m_groupTiles[m_tileID];
      m_tiles[row + 1][col - 1] = m_groupTiles[m_tileID];
      m_tiles[row + 1][col] = m_groupTiles[m_tileID];
      m_tiles[row + 1][col + 1] = m_groupTiles[m_tileID];
    }
  }

  m_isoRow = (float)row;
  m_isoCol = (float)col;
}

void CLevel::RemoveTile(){
  LEVEL_TILE tile;
  tile.id = -1;
  tile.sprite = 0;
  tile.sprite2 = 0;
  tile.rating = 0;

  if(m_isoRow >= 0 && m_isoRow < MAX_ROWS && m_isoCol >= 0 && m_isoCol < MAX_COLS)
    m_tiles[(int)m_isoRow][(int)m_isoCol] = tile;
}

void CLevel::RemoveTileViaMap(int mx, int my, int brushsize){
  int col = (int)((mx - 272)/10);
  int row = (int)((my - 104)/10);
  
  LEVEL_TILE tile;
  tile.id = -1;
  tile.sprite = 0;
  tile.sprite2 = 0;
  tile.rating = 0;

  if(brushsize == 1 && row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLS){
    m_tiles[row][col] = tile;
  }
  else if(brushsize == 2 && row >= 1 && row < MAX_ROWS - 1 && col >= 1 && col < MAX_COLS - 1){
    m_tiles[row - 1][col - 1] = tile;
    m_tiles[row - 1][col] = tile;
    m_tiles[row - 1][col + 1] = tile;
    m_tiles[row][col - 1] = tile;
    m_tiles[row][col] = tile;
    m_tiles[row][col + 1] = tile;
    m_tiles[row + 1][col - 1] = tile;
    m_tiles[row + 1][col] = tile;
    m_tiles[row + 1][col + 1] = tile;
  }
  m_isoRow = (float)row;
  m_isoCol = (float)col;
}

void CLevel::IncTileID(){
  m_tileID++;
  if(m_tileID > m_tileDefinition.size() - 1)
    m_tileID = m_tileDefinition.size() - 1;
}

void CLevel::DecTileID(){
  m_tileID--;
  if(m_tileID < 0)
    m_tileID = 0;
}

void CLevel::SetTileID(size_t id){
  if(id < 0 || id > m_tileDefinition.size() - 1)
    return;

  m_tileID = id;
  
}

void CLevel::DrawGrid(CGraphics &con){
  
  if(m_bDrawGrid == true){
    float offsetX  = m_offsetX;
    float offsetY = m_offsetY; 
    float x1 = 0, y1 = 0;
    float x2 = 0, y2 = 0;
    float x3 = 0, y3 = 0;
    float x4 = 0, y4 = 0;
    for(int j = 0; j < MAX_ROWS; ++j){
      offsetX -= TILE_WIDTH/2;
      offsetY += TILE_HEIGHT/2;
      for(int i = 0; i < MAX_COLS; ++i){
    
        x1 = m_posX + offsetX + (i * TILE_WIDTH/2) + TILE_WIDTH/2;
        y1 = m_posY + offsetY + (i * TILE_HEIGHT/2);      
        x2 = m_posX + offsetX + (i * TILE_WIDTH/2) + TILE_WIDTH;
        y2 = m_posY + offsetY + (i * TILE_HEIGHT/2) + TILE_HEIGHT/2;      
        x3 = m_posX + offsetX + (i * TILE_WIDTH/2) + TILE_WIDTH/2;
        y3 = m_posY + offsetY + (i * TILE_HEIGHT/2) + TILE_HEIGHT;      
        //x4 = m_posX + offsetX + (i * TILE_WIDTH/2);
        //y4 = m_posY + offsetY + (i * TILE_HEIGHT/2) + TILE_HEIGHT/2;
        
        /*  
        con.DrawLine(x1, y1, x2, y2, 255, 0, 0);//red grid
        con.DrawLine(x2, y2, x3, y3, 255, 0, 0);
        con.DrawLine(x3, y3, x4, y4, 255, 0, 0);
        con.DrawLine(x4, y4, x1, y1, 255, 0, 0);
        */ 
        
        con.DrawLine(x1, y1, x2, y2, 30, 30, 30);//gray grid
        con.DrawLine(x2 , y2 , x3, y3, 30, 30, 30);
        //con.DrawLine(x3, y3, x4, y4, 120, 120, 120);
        //con.DrawLine(x4, y4, x1, y1, 120, 120, 120);
      }
    }
  }
}

//loads tiles.dat
bool CLevel::LoadTileDefinitions(CGraphics &con, CConfigData &cfg){

  CLog *pLog = CLog::Instance();
  pLog->Log("Loading tile definitions");

  ClearTiles();
  LEVEL_TILE tile;
  std::string parameter;
  CCSVReader csv;
  csv.LoadFile(cfg.TilesFile);
  if(csv.GetTableSize() < 1)
    return false;

  for(size_t i = 0; i < csv.GetTableSize(); ++i){
    if(csv.GetNumberParameters(i)== 7 ){
      parameter = csv.GetTerm(i, 0);
      tile.id = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 1);
      tile.sprite = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 2);
      tile.sprite2 = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 3);
      tile.rating = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 4);
      tile.yOffset = (float)(atof(parameter.c_str()));
      parameter = csv.GetTerm(i, 5);
      tile.groupID = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 6);
      tile.name = parameter;
      tile.gi = con.GetSprite(tile.sprite);
      tile.gi2 = con.GetSprite(tile.sprite2);
      m_tileDefinition.push_back(tile);
    }
  }

  pLog->Log("Number of tiles loaded", m_tileDefinition.size());
  SetGroupNumber(0);
  return true;
}

bool CLevel::SaveLevelFile(std::string filename){
  
  std::ofstream file(filename.c_str(), std::ios::out);
  if(file){
    for(int i = 0; i <  MAX_ROWS; ++i){
      for(int j = 0; j < MAX_COLS; ++j){
        if(m_tiles[i][j].id > -1)
          file << "tile, " << m_tiles[i][j].id << ", " << i << ", "  << j << std::endl;
      }
    }   
  }
  file.close();
  return true;
}

bool CLevel::LoadLevelFile(std::string filename){
  CLog *pLog = CLog::Instance();
  int nMax = 0;
  LEVEL_TILE tile;
  std::string parameter;
  int row = -1;
  int col = -1;
  size_t id = -1;
  CCSVReader csv;
  csv.LoadFile(filename);
  if(csv.GetTableSize() < 1)
    return false;

  ClearTiles();
  for(size_t i = 0; i < csv.GetTableSize(); ++i){
    if(csv.GetNumberParameters(i)== 4 && csv.GetTerm(i, 0) == "tile"){
      parameter = csv.GetTerm(i, 1);
      id = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 2);
      row = atoi(parameter.c_str());
      parameter = csv.GetTerm(i, 3);
      col = atoi(parameter.c_str());
      for(size_t j = 0; j < m_tileDefinition.size(); ++ j){
        if(id == m_tileDefinition[j].id){
          //if(id < m_tileDefinition.size()  && row > -1 && row < 50 && col > -1 && col < 50){
          if(row > -1 && row < 50 && col > -1 && col < 50){
          
            tile = GetTileDefinition(id);
            tile.yNormalOffset = tile.yOffset;

            //if framecount == -1 then randomize the start order
            if(tile.gi.frameCount == - 1){
              nMax = tile.gi.frameMax;
              if(nMax > 0){
                tile.gi.frameCount = (rand() % nMax) + 1;  
              }
            }    

            //if framecount == -1 then randomize the start order
            if(tile.gi2.frameCount == - 1){
              nMax = tile.gi2.frameMax;
              if(nMax > 0){
                tile.gi2.frameCount = (rand() % nMax) + 1;  
              }
            }    

            m_tiles[row][col] = tile;
          }
        }
      }
    }
  }

  return true;
}

std::string CLevel::GetTileName(size_t id){
  if(id < 0)
    return "";

  LEVEL_TILE tile;
  tile.name = "";
  for(size_t i = 0; i < m_tileDefinition.size(); ++i){
    if(m_tileDefinition[i].id == id){
      return m_tileDefinition[i].name;
    }
  }
  
  return tile.name;
}

std::string CLevel::GetCurrentTileName(){
  return m_tileDefinition[m_tileID].name;
}

int CLevel::GetTileSprite(int index){
  if(index < 0)// || index > m_tileDefinition.size() - 1)
    return -1;

  LEVEL_TILE tile;
  tile.sprite = -1;
  for(size_t i = 0; i < m_tileDefinition.size(); ++i){
    if(m_tileDefinition[i].id == index){
      return m_tileDefinition[i].sprite;
    }
  }

  return tile.sprite;
}

int CLevel::GetTileRating(size_t index){
  if(index < 0)// || index > m_tileDefinition.size() - 1)
    return -1;

  LEVEL_TILE tile;
  tile.rating = -1;
  for(size_t i = 0; i < m_tileDefinition.size(); ++i){
    if(m_tileDefinition[i].id == index){
      return m_tileDefinition[i].rating;
    }
  }

  return tile.rating;
}

float CLevel::GetTileYOffset(size_t index){
  if(index < 0)// || index > m_tileDefinition.size() - 1)
    return -1;

  LEVEL_TILE tile;
  tile.yOffset = 0;
  for(size_t i = 0; i < m_tileDefinition.size(); ++i){
    if(m_tileDefinition[i].id == index){
      return m_tileDefinition[i].yOffset;
    }
  }

  return tile.yOffset;
}

float CLevel::GetTileYOffset(float x, float y){
  int row = CalcIsoRow(x, y, 0);
  int col = CalcIsoCol(x, y, 0);
  if(row < 0 || row > 49 || col < 0 || col > 49)
    return 0;
  LEVEL_TILE tile = m_tiles[row][col];  
  return tile.yOffset;
}




void CLevel::SetCurrentRating(int rating){
  if(rating < 1 || rating > 5)
    return;
  m_currentRating = rating;
}


//conversion helper functions
//********************************************************
int CLevel::CalcIsoRow(float x, float y, int viewMode){//verified
  if(viewMode == 0){//left isometric
    return (int)( ((y + 1613)/32 - (x + 3134)/64)/-2);
  }
  else if(viewMode == 1){//right isometric
    return (int)(((x + 3134)/-64 + (y+1613)/-32)/2);
  }
  return 0;
}

int CLevel::CalcIsoCol(float x, float y, int viewMode){//verified
  if(viewMode == 0){//left isometric
    return (int)(((x + 3134)/-64 + (y + 1613)/-32)/2);
  }
  else if(viewMode == 1){//right isometric
    return (int)(50 - ((y + 1613)/32 - (x + 3134)/64)/-2);
    
  }
  return 0;
}
float CLevel::GetIsoX(int col, int row, int viewMode){
  if(viewMode == 0){//left isometric
    return (float)(-3134 + (row * 64) - (col * 64));
  }
  else if(viewMode == 1){//right isometric
    return (float)(1 - (row * 64) - (col * 64));
  }
  return 0;
}
float CLevel::GetIsoY(int col, int row, int viewMode){
  if(viewMode == 0){//left isometric
    return (float)(-1613 - (col * 32) - (row * 32));
  }
  else if(viewMode == 1){//right isometric
    return (float)(-3187 + (row * 32) - (col * 64));
  }
  return 0;
}

//supports temporary grouping of tiles

void CLevel::SetGroupNumber(int num){
  m_groupIndex = num;

  m_groupTiles.clear();
  for(size_t i = 0; i < m_tileDefinition.size();++i){
    if(m_tileDefinition[i].groupID == num)
      m_groupTiles.push_back(m_tileDefinition[i]);
  }
 
  if(m_groupTiles.size() < 1)
    m_tileID = -1;
  else
    m_tileID = 0;
}

void CLevel::SetGroupTileID(size_t id){
  if(id < 0 || id > m_groupTiles.size() - 1)
    return;
  m_tileID = id;  
}

int CLevel::GetGroupTileSprite(size_t index){
  if(index < 0 || index > m_groupTiles.size() - 1)
    return -1;
  return m_groupTiles[index].sprite;
}

int CLevel::GetGroupTileRating(size_t index){
  if(index < 0 || index > m_groupTiles.size() - 1)
    return -1;
  return m_groupTiles[index].rating;
}

float CLevel::GetGroupTileYOffset(size_t index){
  if(index < 0 || index > m_groupTiles.size() - 1)
    return -1;
  return m_groupTiles[index].yOffset;
}

LEVEL_TILE CLevel::GetTileDefinition(size_t id){
  LEVEL_TILE tile;
  tile.id = -1;
  for(size_t i = 0; i < m_tileDefinition.size(); ++i){
    if(m_tileDefinition[i].id == id){
      return m_tileDefinition[i];
    }
  }
  return tile;
}




void CLevel::Render(CGraphics &con, CGameData &data, int row, int col){
  LEVEL_TILE tile;  
  float px, py;
  m_levelScale = 1.0;

  //j = col, i = row
  //if(m_viewMode == 0){
    
  if(m_tiles[col][row].rating <= m_currentRating){ 
    px = m_posX + m_offsetX - m_tileWidth/2 + (row * m_tileWidth/2) - (col * m_tileWidth/2);
    py = m_posY +  m_offsetY + m_tileHeight/2 + (col * m_tileHeight/2) + (row * m_tileHeight/2) + m_tiles[col][row].yOffset;
    if(px > - m_tileWidth * 2 && px < 1024 + m_tileWidth * 2 && py > -m_tileHeight * 2 && py < 768 + m_tileHeight * 2){
      con.RenderSprite(m_tiles[col][row].gi, px, py, 255, 255, 255);
    }
  }      
  else{//<<<<<< need softcode
    tile = GetTileDefinition(0);//m_tileDefinition[0];                    
    px = m_posX + m_offsetX - m_tileWidth/2 + (row * m_tileWidth/2) - (col * m_tileWidth/2);
    py = m_posY +  m_offsetY + m_tileHeight/2 + (col * m_tileHeight/2) + (row * m_tileHeight/2) + tile.yOffset;
    if(px > - m_tileWidth * 2 && px < 1024 + m_tileWidth * 2 && py > -m_tileHeight * 2 && py < 768 + m_tileHeight * 2){
      con.RenderSprite(tile.gi, px, py, 255, 255, 255);
    }
  }


}