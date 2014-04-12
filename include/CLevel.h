#ifndef CLEVEL_H
#define CLEVEL_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "CGraphics.h"
#include "CGameData.h"
#include "CCSVReader.h"
#include "CConfigData.h"
#include "CLog.h"

class CGraphics;
struct GRAPHIC_IMAGE;

struct LEVEL_TILE{
  int id;
  std::string name;
  float yOffset;
  float yNormalOffset;
  int rating;         //1 - 5
  int groupID;
  int sprite;
  int sprite2;
  GRAPHIC_IMAGE gi;   //isometric sprites
  GRAPHIC_IMAGE gi2;  //reverse isometric sprites
};

class CLevel{
public:
  CLevel();
  ~CLevel();
  void Update(float posX, float posY, float mouseX, float mouseY, double timeDifference, CGameData &data);
  void Render(CGraphics &con, CGameData &data);
  void Render(CGraphics &con, CGameData &data, int row, int col);
  void ClearTiles();
  void AddTile(int brushsize);
  void AddTileViaMap(int mx, int my, int brushsize);
  void RemoveTile();
  void RemoveTileViaMap(int mx, int my, int brushsize);
  float GetOffsetX(){return m_offsetX;}
  float GetOffsetY(){return m_offsetY;}
  int GetViewMode(){return m_viewMode;}
  void SetViewMode(int mode){m_viewMode = mode;}
  size_t GetDrawLevel(){return m_tileID;}
  bool IsDrawGrid(){return m_bDrawGrid;}
  void ToggleGrid(){m_bDrawGrid = !m_bDrawGrid;}
  void IncTileID();
  void DecTileID();
  void SetTileID(size_t id);
  void DrawGrid(CGraphics &con);
  bool LoadTileDefinitions(CGraphics &con, CConfigData &cfg);
  std::string GetTileName(size_t i);
  std::string GetCurrentTileName();
  int GetIsoRow(){return (int)m_isoRow;}
  int GetIsoCol(){return (int)m_isoCol;}
  int GetTileSprite(int index);
  size_t GetTileDefSize(){return m_tileDefinition.size();}
  bool SaveLevelFile(std::string filename);
  bool LoadLevelFile(std::string filename);
  int GetTileRating(size_t index);
  float GetTileYOffset(size_t index);
  float GetTileYOffset(float x, float y);
  void SetLevelScale(float scale){m_levelScale = scale;}
  void SetCurrentRating(int rating);
  void RenderMap(CGraphics &con, CConfigData &cfg); 
  LEVEL_TILE GetTileDefinition(size_t id);

  static const int MAX_ROWS = 50;
  static const int MAX_COLS = 50;
  static const int TILE_WIDTH = 128;
  static const int TILE_HEIGHT = 64;

  //conversion helper functions
  int CalcIsoRow(float x, float y, int viewMode);
  int CalcIsoCol(float x, float y, int viewMode);
  float GetIsoX(int col, int row, int viewMode);
  float GetIsoY(int col, int row, int viewMode);

  //supports temporary grouping of tiles v018
  void SetGroupNumber(int num);           //sets the group number, clears vec, loads new group
  size_t GetGroupSize(){return m_groupTiles.size();} //ret number of tiles in this group
  int GetGroupTileRating(size_t index);      //ret rating of tile
  float GetGroupTileYOffset(size_t index);   //ret yOffset of tile
  int GetGroupTileSprite(size_t index);
  void SetGroupTileID(size_t id);

private:
  int m_viewMode;
  float m_offsetX;
  float m_offsetY;
  float m_yIntercept1;
  float m_yIntercept2;
  float m_yIntercept1a;
  float m_yIntercept2a;
  float m_isoRow;
  float m_isoCol;
  float m_tileWidth;
  float m_tileHeight;
  int m_currentRating;
  size_t m_tileID;
  bool m_bDrawGrid;
  float m_posX; //used for rendering only
  float m_posY;

  //water levle
  float m_oscillate;
  float m_oscUp;

  //row,col of center screen
  float m_isoRowCenter;
  float m_isoColCenter;

  //revision to world system
  LEVEL_TILE m_tiles[MAX_ROWS][MAX_COLS];
  std::vector<LEVEL_TILE> m_tileDefinition; //stores tile data from file
  float m_levelScale;

  //supports temporary grouping of tiles
  //********************************************************
  std::vector<LEVEL_TILE> m_groupTiles;   //stores select grouping of files
  int m_groupIndex;                       //current group index number
  int m_groupHighestIndex;                //group index numbers do not need to be successive
  int m_groupLowestIndex;

};

#endif