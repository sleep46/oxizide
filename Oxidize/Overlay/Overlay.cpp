#include "../includes.h"
#include "Overlay.h"

__int64 __stdcall window_procedur( HWND window, unsigned int message, unsigned __int64 w_param, __int64 l_param )noexcept
{
	if ( ImGui_ImplWin32_WndProcHandler( window, message, w_param, l_param ) )
		return true;

	switch ( message )
	{
	case WM_DESTROY:
		PostQuitMessage( 0 );
		ExitProcess( 0 );
		break;

	case WM_SIZE:
		if ( overlay->device != NULL && w_param != SIZE_MINIMIZED )
		{
			overlay->Width = ( UINT ) LOWORD( l_param );
			overlay->Height = ( UINT ) HIWORD( l_param );
		}
		break;
	default:
		return DefWindowProcA( window, message, w_param, l_param );
		break;
	}
}

inline void SetWindowToTarget( )
{
	while ( true )
	{
		if ( overlay->hTargetWindow )
		{
			RtlZeroMemory( &overlay->GameRect, sizeof( overlay->GameRect ) );
			GetWindowRect( overlay->hTargetWindow, &overlay->GameRect );
			overlay->Width = overlay->GameRect.right - overlay->GameRect.left;
			overlay->Height = overlay->GameRect.bottom - overlay->GameRect.top;
			DWORD dwStyle = GetWindowLongA( overlay->hTargetWindow, GWL_STYLE );

			if ( dwStyle & WS_BORDER )
			{
				overlay->GameRect.top += 32;
				overlay->Height -= 39;
			}

			MoveWindow( overlay->Window, overlay->GameRect.left, overlay->GameRect.top, overlay->Width, overlay->Height, true );
		}
		Sleep( 10 );
	}
}

void Overlay::InitWindow( )
{
	create_thread( SetWindowToTarget );

	wnd_class.cbSize = sizeof( WNDCLASSEXA );
	wnd_class.style = 2 | 1;
	wnd_class.lpfnWndProc = window_procedur;
	wnd_class.lpszClassName = xorstr_( "Overwolf" );
	RegisterClassExA( &wnd_class );

	Window = CreateWindowExA(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE,
		wnd_class.lpszClassName,
		nullptr,
		WS_POPUP,
		0,
		0,
		Width,
		Height,
		nullptr,
		nullptr,
		wnd_class.hInstance,
		nullptr
	);

	SetLayeredWindowAttributes( Window, RGB( 0, 0, 0 ), BYTE( 255 ), LWA_ALPHA );
	{
		RECT client_area {};
		GetClientRect( Window, &client_area );

		RECT window_area {};
		GetWindowRect( Window, &window_area );

		POINT diff {};
		ClientToScreen( Window, &diff );

		const MARGINS margins {
			window_area.left + ( diff.x - window_area.left ),
			window_area.top + ( diff.y - window_area.top ),
			window_area.right,
			window_area.bottom
		};

		DwmExtendFrameIntoClientArea( Window, &Margin );
	}
}

void Overlay::InitDX11( )
{
	DXGI_SWAP_CHAIN_DESC sd {};
	sd.BufferDesc.RefreshRate.Numerator = 60U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = Window;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels [ 2 ] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&device_context
	);

	ID3D11Texture2D* buffer { nullptr };
	swap_chain->GetBuffer( 0U, IID_PPV_ARGS( &buffer ) );
	if ( buffer ) {
		device->CreateRenderTargetView( buffer, nullptr, &render_target_view );
		buffer->Release( );
	}
}

void Overlay::run( )
{
	Sleep( 1000 );
	InitWindow( );
	InitDX11( );

	ShowWindow( Window, SW_SHOWDEFAULT );
	UpdateWindow( Window );

	ImGui::CreateContext( );
	ImGuiIO& io = ImGui::GetIO( ); ( void ) io;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	m_pFont = io.Fonts->AddFontFromFileTTF( xorstr_( "C:\\Windows\\Fonts\\Verdana.ttf" ), 16 );

	ImGui_ImplWin32_Init( Window );
	ImGui_ImplDX11_Init( device, device_context );

	Menu->InitStyle( );

	using framerate = std::chrono::duration<int, std::ratio<1, 90>>;
	auto tp = std::chrono::system_clock::now( ) + framerate { 1 };

	while ( runing )
	{
		MSG msg;
		while ( PeekMessageA( &msg, nullptr, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessageA( &msg );

			if ( msg.message == WM_QUIT )
				runing = false;
		}

		if ( !runing )
			break;

		if ( Width != 0 && Height != 0 )
		{
			if ( render_target_view ) {
				render_target_view->Release( );
			}

			swap_chain->ResizeBuffers( 0, Width, Height, DXGI_FORMAT_UNKNOWN, 0 );

			ID3D11Texture2D* pBackBuffer;
			swap_chain->GetBuffer( 0, IID_PPV_ARGS( &pBackBuffer ) );
			device->CreateRenderTargetView( pBackBuffer, nullptr, &render_target_view );
			pBackBuffer->Release( );
		}

		ImGui_ImplDX11_NewFrame( );
		ImGui_ImplWin32_NewFrame( );
		ImGui::NewFrame( );

		g_pEntity->RenderDrawing( );

		if ( GetAsyncKeyState( VK_INSERT ) & 1 ) {
			Menu->ShowMenu = !Menu->ShowMenu;
		}

		if ( Menu->ShowMenu ) {
			SetWindowLongA( overlay->Window, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_NOACTIVATE );
			Menu->RenderMenu( );
		}
		else {
			SetWindowLongA( overlay->Window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_NOACTIVATE );
		}

		ImGui::Render( );
		constexpr float color [ 4 ] { 0.f,0.f,0.f,0.f };
		device_context->OMSetRenderTargets( 1U, &render_target_view, nullptr );
		device_context->ClearRenderTargetView( render_target_view, color );

		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
		swap_chain->Present( 1U, 0U );
		std::this_thread::sleep_until( tp );
		tp += framerate { 1 };
	}

	ImGui_ImplDX11_Shutdown( );
	ImGui_ImplWin32_Shutdown( );

	ImGui::DestroyContext( );

	if ( swap_chain )
		swap_chain->Release( );

	if ( device_context )
		device->Release( );

	if ( device )
		device->Release( );

	if ( render_target_view )
		render_target_view->Release( );

	DestroyWindow( Window );
	UnregisterClassA( wnd_class.lpszClassName, wnd_class.hInstance );
}