#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <sstream>
#include <TlHelp32.h>
#include <thread>
#include <vector>
#include <array>
#include <math.h>
#include <winternl.h>
#include <cmath>
#include <map>
#include <dwmapi.h>
#include <atlstr.h>
#include <d3d11.h>
#include <d3dx11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"

inline ImFont* m_pFont;

#include "Utils/Colors.h"
#include "Utils/XorStr.h"
#include "Utils/LazyImporter.h"

#include "Driver/kinterface.h"

#include "Overlay/Overlay.h"
#include "Overlay/Render/CRender.h"

#include "Engine/CBasePlayer.h"
#include "Engine/CBaseNetworkable.h"
#include "Engine/CEntity.h"

#include "Menu/Menu.h"