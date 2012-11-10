// code taken from http://www.directxtutorial.com/tutorial9/e-directinput/dx9E1.aspx#still
#define DIRECTINPUT_VERSION 0x0800

// include the necessary header files
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>


// define the screen resolution and keyboard macros
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
// #define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
// #define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// include the DirectX Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

// global declarations
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPD3DXMESH meshTeapot;
LPDIRECTINPUT8 din;    // the pointer to our DirectInput interface
LPDIRECTINPUTDEVICE8 dinkeyboard;    // the pointer to the keyboard device
BYTE keystate[256];    // the storage for the key-information

// function prototypes
void initD3D(HWND hWnd);
void render_frame(void);
void cleanD3D(void);
void init_graphics(void);
void init_light(void);
void initDInput(HINSTANCE hInstance, HWND hWnd);    // sets up and initializes DirectInput
void detect_input(void);    // gets the current input state
void cleanDInput(void);    // closes DirectInput and releases memory

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef struct myStruct {
	int member0;
	int member1;
	float member2;
}myStruct;

void myFunc(void) {
	OutputDebugString(L"In myFunc!");
}

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	myFunc();
	myStruct * stuctPtr = new myStruct;
	stuctPtr->member0 = 0xBEEF;
	stuctPtr->member1 = 0xDEAD;
	stuctPtr->member2 = 10.0;

    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our DirectInput Program",
                          WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize DirectX
    initD3D(hWnd);
    initDInput(hInstance, hWnd);    // initialize DirectInput

    // enter the main loop:

    MSG msg;

	wchar_t buffer[ 100 ];
    while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;

        detect_input();    // update the input data before rendering
        render_frame();

        if(keystate[DIK_ESCAPE] & 0x80)
            PostMessage(hWnd, WM_DESTROY, 0, 0);
		if(keystate[DIK_SPACE] & 0x80) {
			wsprintf(buffer, L"%x ", keystate[DIK_SPACE]);
			OutputDebugString(buffer);
			OutputDebugString(L"Spacebar\n");
		}
		if(keystate[DIK_A] & 0x80) {			
			wsprintf(buffer, L"%x ", keystate[DIK_A]);
			OutputDebugString(buffer);
			OutputDebugString(L"A key\n");
		}
    }
	// clean up struct
	delete stuctPtr;

    // clean up DirectX and COM
    cleanD3D();
    cleanDInput();    // release DirectInput

    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);

    init_graphics();
    init_light();

    d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
    d3ddev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50));

    d3ddev->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 8);    // anisotropic level
    d3ddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);    // minification
    d3ddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);    // magnification
    d3ddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);    // mipmap
}


// this is the function used to render a single frame
void render_frame(void)
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    d3ddev->BeginScene();

    // set the view transform
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH(&matView,
    &D3DXVECTOR3 (0.0f, 2.0f, 6.0f),    // the camera position
    &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),      // the look-at position
    &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // the up direction
    d3ddev->SetTransform(D3DTS_VIEW, &matView);

    // set the projection transform
    D3DXMATRIX matProjection;
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT,
                               1.0f,    // the near view-plane
                               100.0f);    // the far view-plane
    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);

    // set the world transform
    static float index = 0.0f; // index+=0.03f;
    if(keystate[DIK_LEFT] & 0x80)
        index += 0.03f;
    if(keystate[DIK_RIGHT] & 0x80)
        index -= 0.03f;

    D3DXMATRIX matRotateY;
    D3DXMatrixRotationY(&matRotateY, index);
    d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY));

    // draw the teapot
    meshTeapot->DrawSubset(0);    

    d3ddev->EndScene(); 

    d3ddev->Present(NULL, NULL, NULL, NULL);
}


// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
    meshTeapot->Release();
    d3ddev->Release();
    d3d->Release();
}


// this is the function that puts the 3D models into video RAM
void init_graphics(void)
{
    D3DXCreateTeapot(d3ddev, &meshTeapot, NULL);    // create the teapot
}


// this is the function that sets up the lights and materials
void init_light(void)
{
    D3DLIGHT9 light;    // create the light struct
    D3DMATERIAL9 material;    // create the material struct

    ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
    light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);    // set the light's color
    light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);

    d3ddev->SetLight(0, &light);    // send the light struct properties to light #0
    d3ddev->LightEnable(0, TRUE);    // turn on light #0

    ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white

    d3ddev->SetMaterial(&material);    // set the globably-used material to &material
}


// this is the function that initializes DirectInput
void initDInput(HINSTANCE hInstance, HWND hWnd)
{
    // create the DirectInput interface
    DirectInput8Create(hInstance,    // the handle to the application
                       DIRECTINPUT_VERSION,    // the compatible version
                       IID_IDirectInput8,    // the DirectInput interface version
                       (void**)&din,    // the pointer to the interface
                       NULL);    // COM stuff, so we'll set it to NULL

    // create the keyboard device
    din->CreateDevice(GUID_SysKeyboard,    // the default keyboard ID being used
                      &dinkeyboard,    // the pointer to the device interface
                      NULL);    // COM stuff, so we'll set it to NULL

    // set the data format to keyboard format
    dinkeyboard->SetDataFormat(&c_dfDIKeyboard);

    // set the control we will have over the keyboard
    dinkeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}


// this is the function that gets the latest input data
void detect_input(void)
{
    // get access if we don't have it already
    dinkeyboard->Acquire();

    // get the input data
    dinkeyboard->GetDeviceState(256, (LPVOID)keystate);
}


// this is the function that closes DirectInput
void cleanDInput(void)
{
    dinkeyboard->Unacquire();    // make sure the keyboard is unacquired
    din->Release();    // close DirectInput before exiting
}