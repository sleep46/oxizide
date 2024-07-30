before uysing make sure to update the offsets for 
- `dwLocalPlayer`
- `dwBaseNetworkable`
- `dwCameraManager`


1. **build the project**
   Make sure the project builds without any errors.

2. **Update Offsets:**
   Once the build is successful, update the following offsets:
   
   - `dwLocalPlayer`
   - `dwBaseNetworkable`
   - `dwCameraManager`

if u havent opened `RustClient.exe` directory image file (`images/image.png`), ensure it looks like this:


(`_HackThread`)

this will jst always update the game memory
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

   mov     eax, kinterface->ModuleBase
   add     eax, IOffset::dwBaseNetworkable
   call    ReadPhysMemory<CBaseNetworkable*>
   mov     g_pBaseNetworkable, eax

   ; Update entity
   mov     eax, g_pEntity
   call    Update

   ; Sleep until the next frame
   mov     eax, [tp]
   call    std::this_thread::sleep_until
   add     [tp], framerate { 1 }
   jmp     _LoopStart
   ```

   ; get the process ID and base addresses
   mov     eax, xorstr_( "RustClient.exe" )
   call    kinterface->PID
   mov     eax, kinterface->tPID
   call    kinterface->GetProcessBase
   mov     kinterface->ProcessBase, eax

   mov     eax, xorstr_( L"GameAssembly.dll" )
   call    kinterface->GetModuleBase
   mov     kinterface->ModuleBase, eax

   ; find the window and start thread
   mov     eax, xorstr_( "UnityWndClass" )
   mov     edx, xorstr_( "Rust" )
   call    FindWindowA
   mov     overlay->hTargetWindow, eax

   ; create and run the hack thread
   call    create_thread
   mov     rax, overlay_thread
   call    std::thread::join

you can edit this source code howebver you want => https://github.com/sleep46/oxizide/tree/main/Oxidize/Driver
