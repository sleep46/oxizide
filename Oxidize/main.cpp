#include "includes.h"

void _HackThread( void )
{
	using framerate = std::chrono::duration<int, std::ratio<1, 30>>;
	auto tp = std::chrono::system_clock::now( ) + framerate { 1 };

	while ( true )
	{
		g_pLocalPlayer = ReadPhysMemory<CLocalPlayer*>( kinterface->ModuleBase + IOffset::dwLocalPlayer );
		g_pBaseNetworkable = ReadPhysMemory<CBaseNetworkable*>( kinterface->ModuleBase + IOffset::dwBaseNetworkable );

		g_pEntity->Update( );

		std::this_thread::sleep_until( tp );
		tp += framerate { 1 };
	}
}

int main( ) {
	Sleep( 1000 );

	if ( kinterface->Initialize( ) == FALSE ) {
		ExitProcess( 0 );
		__fastfail( 0 );
	}

	kinterface->PID( xorstr_( "RustClient.exe" ) ); /* RustDedicated.exe */ 
	/* kinterface->PID( xorstr_( "RustDedicated.exe" ) );  */
	kinterface->ProcessBase = kinterface->GetProcessBase( kinterface->tPID );
	kinterface->ModuleBase = kinterface->GetModuleBase( kinterface->tPID, xorstr_( L"GameAssembly.dll" ) );

	printf( xorstr_( "kinterface->tPID: %d\n" ), kinterface->tPID );
	printf( xorstr_( "kinterface->ProcessBase: %llx\n" ), kinterface->ProcessBase );
	printf( xorstr_( "kinterface->ModuleBase: %llx\n" ), kinterface->ModuleBase );

	overlay->hTargetWindow = FindWindowA( xorstr_( "UnityWndClass" ), xorstr_( "Rust" ) );

	const auto overlay_thread { [ & ]( )->void {
		overlay->run( );
	} };

	create_thread( _HackThread );

	std::thread( overlay_thread ).join( );
}