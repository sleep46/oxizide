#include "../includes.h"
#include "kinterface.h"

bool kinterface_t::Initialize( ) {
	return true;
}

bool kinterface_t::SendCMD( void* data, request_codes code ) {
	return true;
}

uintptr_t kinterface_t::GetProcessBase( int PID ) {
	proc_request data { 0 };

	data.pid = PID;
	SendCMD( &data, request_procbase );
	return data.ProcessBase;
}

uintptr_t kinterface_t::GetModuleBase( int PID, LPCWSTR ModName ) {
	proc_request data { 0 };

	data.pid = PID;
	lstrcpyW( data.lpModName, ModName );
	SendCMD( &data, request_modbase );
	return data.ModuleBase;
}

bool kinterface_t::ReadPhysMemory( const int pid, const std::uintptr_t address, void* buffer, const std::size_t size, bool mmcopy, PDWORD_PTR num_bytes ) {
	read_request data { 0 };

	data.pid = pid;
	data.address = address;
	data.buffer = buffer;
	data.mmcopy = mmcopy;
	data.size = size;

	if ( num_bytes )
		*num_bytes = data.ret_size;
	return SendCMD( &data, request_read );
}

bool kinterface_t::WritePhysMemory( const int pid, const std::uintptr_t address, void* buffer, const std::size_t size ) {
	read_request data { 0 };

	data.pid = pid;
	data.address = address;
	data.buffer = buffer;
	data.size = size;

	return SendCMD( &data, request_write );
}

int kinterface_t::GetProcessThreadNumByID( DWORD dwPID )
{
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( hProcessSnap == INVALID_HANDLE_VALUE )
		return 0;

	PROCESSENTRY32 pe32 = { 0 };
	pe32.dwSize = sizeof( pe32 );
	BOOL bRet = ::Process32First( hProcessSnap, &pe32 );;
	while ( bRet )
	{
		if ( pe32.th32ProcessID == dwPID )
		{
			::CloseHandle( hProcessSnap );
			return pe32.cntThreads;
		}
		bRet = ::Process32Next( hProcessSnap, &pe32 );
	}
	return 0;
}

int kinterface_t::PID( const char* name )
{
	DWORD dwThreadCountMax = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );
	Process32First( hSnapshot, &pe32 );
	do
	{
		if ( _tcsicmp( pe32.szExeFile, name ) == 0 )
		{
			DWORD dwTmpThreadCount = GetProcessThreadNumByID( pe32.th32ProcessID );

			if ( dwTmpThreadCount > dwThreadCountMax )
			{
				dwThreadCountMax = dwTmpThreadCount;
				tPID = pe32.th32ProcessID;
			}
		}
	} while ( Process32Next( hSnapshot, &pe32 ) );
	CloseHandle( hSnapshot );
	return tPID;
}