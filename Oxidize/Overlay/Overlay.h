#pragma once
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

constexpr D3D_FEATURE_LEVEL levels [ 2 ] = {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_0
};

class Overlay
{
public:
	WNDCLASSEXA wnd_class {};
	DXGI_SWAP_CHAIN_DESC sd {};
	ID3D11Device* device { nullptr };
	ID3D11DeviceContext* device_context { nullptr };
	IDXGISwapChain* swap_chain { nullptr };
	ID3D11RenderTargetView* render_target_view { nullptr };
	D3D_FEATURE_LEVEL level {};
	ID3D11Texture2D* buffer { nullptr };
public:
	MARGINS Margin = { -1 };
	HWND hTargetWindow = NULL;
	HWND Window = NULL;
	RECT GameRect = { NULL };
	MSG Message = { NULL };
	bool runing = true;
	float Width;
	float Height;
public:
	void InitWindow( );
	void InitDX11( );
	void run( );
};
inline Overlay* overlay = new Overlay( );

inline HANDLE create_thread( void* func ) {
	PTP_POOL pool = CreateThreadpool( NULL );
	if ( pool == NULL ) {
		return NULL;
	}

	PTP_WORK work = CreateThreadpoolWork( ( PTP_WORK_CALLBACK ) func, NULL, NULL );
	if ( work == NULL ) {
		CloseThreadpool( pool );
		return NULL;
	}

	SubmitThreadpoolWork( work );

	return work;
}