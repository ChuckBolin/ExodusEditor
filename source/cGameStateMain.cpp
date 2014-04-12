/*
  CGameStateMain.cpp - Controls the user interface and display for the main menu. 

*/
#include "..\include\CGameStateMain.h"
#include "..\include\keystatus.h"

CGameStateMain::CGameStateMain(void)
{
}

CGameStateMain::~CGameStateMain(void)
{
	m_TE.clear();
}

void CGameStateMain::Initialize(){
  m_selection = 0; 
}

void CGameStateMain::Activate(CGameData &data, CConfigData &cfg, CGraphics &con){
 m_bMoveLeft = false;
}

CGameStateObject* CGameStateMain::Update(double timeDifference,  CGameData &data, CConfigData &cfg, CGraphics &con){
  m_event = EVENT_GO_NO_WHERE;

  //CAudioManager *pAudio = CAudioManager::Instance();
  m_mouse.SetHandle(data.m_hWnd);
  con.m_textureRed = 155;//data.m_textureColorRed;
  con.m_textureGreen = 155;//data.m_textureColorGreen;
  con.m_textureBlue = 155;//data.m_textureColorBlue;

  //process keys
  static double timeDiff = 0;
  timeDiff += timeDifference;

  //mouse clicking
  //*********************************************************************************************
  if(::GetActiveWindow() == data.m_hWnd && timeDiff > 0.4 ){//data.m_bLeftMouseDown == true && 
    timeDiff = 0.0;

    if(keyDown('Q') == true)
      m_event = EVENT_GO_CREDITS;
    else if(keyDown('P') == true)
      m_event = EVENT_GO_PLAY1;

    //left arrow clicked
    if(m_mouse.MouseX() > 190 && m_mouse.MouseX() < 223 && m_mouse.MouseY() > 349 && m_mouse.MouseY() < 449){
    }

  }

  for(unsigned int i=0; i< m_TE.size(); i++){
    if (m_TE[i].event == m_event){
        return m_TE[i].p_gso;
    }
  }

  return 0;
}

int CGameStateMain::Render(CGraphics &con,  CGameData &data, CConfigData &cfg){
  CLog *pLog = CLog::Instance();
  static bool bLostDevice = false;

  ::SetCursor(false);

  //test to see if graphic device is lost, if so, deal with it
  //**********************************************************
  HRESULT hr;
  if(FAILED(hr = con.m_d3ddev->TestCooperativeLevel())){
    
    //device is lost...cannot be reset...exit Render() function
    if(hr == D3DERR_DEVICELOST){
      if(bLostDevice == false){
        bLostDevice = true;
        con.DeleteLost();    //delete one time per failure
      }
      return 0;
    }

    //device was lost...but now can be RESET
    else if( hr == D3DERR_DEVICENOTRESET ){
      hr = con.m_d3ddev->Reset(&con.GetDPP());      
      if(SUCCEEDED(hr)){
        con.LoadAssetFile(cfg.GamePlayAssetFile);
        con.LoadAssetFile(cfg.FrameworkAssetFile);     
        con.ReloadLost();
        bLostDevice = false;
      }
      return 0;
    }
  }

  // clear the window to a deep blue
  con.m_d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(106, 126, 67), 1.0f, 0);
  con.m_d3ddev->BeginScene();    // begins the 3D scene
  con.BeginRender();

  GRAPHIC_IMAGE gi;
  //gi = con.GetSprite(1004);
  //con.RenderSprite(gi, 256, con.GetScreenCenterY() - gi.height/2,255, 255, 255);

  con.Print("Main Menu", F_V16, 450, 720, 255, 255, 255, 255);
  con.Print("Press P to Play", F_V16, 10, 50, 255, 255, 255, 255);
  con.Print("Press Q to Quit", F_V16, 10, 70, 255, 255, 255, 255);
  
  //instructions
if(cfg.DisplayDebugInfo == true){
  con.Print("FPS", data.m_FPS, F_A12, 10, 10, 0, 0, 0, 255);
  con.Print("Mouse X", m_mouse.MouseX(), F_A12, 10, 30, 255, 255, 255, 255);
  con.Print("Mouse Y", m_mouse.MouseY(), F_A12, 10, 50, 255, 255, 255, 255);
}
 
  con.EndRender();

  //draw mouse cursor
  //draw mouse
  con.DrawCircle((float)m_mouse.MouseX(), (float)m_mouse.MouseY(), 255, 255, 255, 5);  
  con.DrawRect(m_mouse.MouseX() - 16, m_mouse.MouseY() - 16, m_mouse.MouseX() + 16, m_mouse.MouseY() + 16, 255, 255, 255);

  con.m_d3ddev->EndScene();    // ends the 3D scene
  hr = con.m_d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen

  return 0;
}

void CGameStateMain::ProcessEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
}

void CGameStateMain::Deactivate(CGameData &data, CConfigData &cfg, CGraphics &con){

}
void CGameStateMain::Resume(){
}
void CGameStateMain::Pause(){
}
void CGameStateMain::Save(){
}

void CGameStateMain::AddTransitionEvent(int event, CGameStateObject* p_Next){
  TRANSITION_EVENT te;
  te.event=event;
  te.p_gso = p_Next;

  m_TE.push_back(te);
}