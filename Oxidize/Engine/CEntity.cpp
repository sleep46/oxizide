#include "../includes.h"
#include "CEntity.h"
#include "CBaseEntity.h"
#include "CBasePlayer.h"
#include "CBaseNetworkable.h"
#include "CHumanNPC.h"

uintptr_t camera_manager_offset = NULL;
uintptr_t camera_manager = NULL;
uintptr_t camera_object = NULL;
uintptr_t camera = NULL;

std::map<std::string, CGenRandomColor> m_mapRandomColor;

CEntity::CEntity( void )
{
	bBones = false;
	bBox2D = false;
	fRenderDistance = 100.0f;
	mapOtherEntities.clear();
}

CEntity::~CEntity( void )
{
}

void DrawBone( ImDrawList* drawList, CModel* pModel, BoneList boneFrom, BoneList boneTo, ImU32 col )
{
	auto From = pModel->GetBonePosition(boneFrom);
	auto To = pModel->GetBonePosition( boneTo );

	Vector2 screenFrom;
	Vector2 screenTo;

	if (From.W2S( screenFrom ) && To.W2S( screenTo ))
	{
		ImVec2 vFrom = ImVec2( screenFrom.x, screenFrom.y );
		ImVec2 vTo = ImVec2( screenTo.x, screenTo.y );

		drawList->AddLine( vFrom, vTo, col );
	}
}

void DrawBox2D( CModel* pModel, ImU32 col )
{
	Vector2 tempFeetR, tempFeetL, tempHead;

	auto FootR = pModel->GetBonePosition( r_foot );
	auto FootL = pModel->GetBonePosition( l_foot );
	auto Head = pModel->GetBonePosition( head );

	if (FootR.W2S( tempFeetR ) && FootL.W2S( tempFeetL ) && Head.W2S( tempHead ))
	{
		Vector2 tempFeet = (tempFeetR + tempFeetL) / 2.f;
		float Entity_h = tempHead.y - tempFeet.y;
		float w = Entity_h / 4;
		float Entityxorstr_ = tempFeet.x - w;
		float Entity_y = tempFeet.y;
		float Entity_w = Entity_h / 2;

		g_pRender->AddRect( Vector2( Entityxorstr_, Entity_y ), Vector2( Entity_w, Entity_h ), col, 2.0f );
	}
}

void CEntity::DrawBones( ImDrawList *drawList, CModel* pModel, ImU32 col )
{
	{
		DrawBone( drawList, pModel, BoneList::head, BoneList::neck, col );
		DrawBone( drawList, pModel, BoneList::neck, BoneList::spine4, col );

		DrawBone( drawList, pModel, BoneList::spine4, BoneList::spine3, col );
		DrawBone( drawList, pModel, BoneList::spine3, BoneList::spine2, col );
		DrawBone( drawList, pModel, BoneList::spine2, BoneList::spine1, col );

		DrawBone( drawList, pModel, BoneList::neck, BoneList::l_clavicle, col );
		DrawBone( drawList, pModel, BoneList::l_clavicle, BoneList::l_upperarm, col );
		DrawBone( drawList, pModel, BoneList::l_upperarm, BoneList::l_forearm, col );
		DrawBone( drawList, pModel, BoneList::l_forearm, BoneList::l_hand, col );
		
		DrawBone( drawList, pModel, BoneList::neck, BoneList::r_clavicle, col );
		DrawBone( drawList, pModel, BoneList::r_clavicle, BoneList::r_upperarm, col );
		DrawBone( drawList, pModel, BoneList::r_upperarm, BoneList::r_forearm, col );
		DrawBone( drawList, pModel, BoneList::r_forearm, BoneList::r_hand, col );

		DrawBone( drawList, pModel, BoneList::spine1, BoneList::r_hip, col );
		DrawBone( drawList, pModel, BoneList::r_hip, BoneList::r_knee, col );
		DrawBone( drawList, pModel, BoneList::r_knee, BoneList::r_foot, col );

		DrawBone( drawList, pModel, BoneList::spine1, BoneList::l_hip, col );
		DrawBone( drawList, pModel, BoneList::l_hip, BoneList::l_knee, col );
		DrawBone( drawList, pModel, BoneList::l_knee, BoneList::l_foot, col );
	}
}

void CEntity::EntityList( void )
{
	auto ClientEntities = g_pBaseNetworkable->GetEntityRealm()->GetChildren();
	auto pGetValue = ClientEntities->GetValues();

	if (pGetValue)
	{
		std::vector<_stBasePlayer>	vecBasePlayer{};
		std::vector<_stBaseAnimal>	vecBaseAnimal{};
		std::vector<_stBaseEntity>	vecBaseEntity{};

		auto pLocalBasePlayer = g_pLocalPlayer->GetBasePlayer();
		auto vPos = pLocalBasePlayer->GetModel()->GetPosition();

		for (int i = 0; i < pGetValue->GetSize(); i++)
		{
			auto pEntity = (CBaseEntity*)pGetValue->GetArray( i );
			if (!pEntity) continue;

			CStringA pClassName = pEntity->GetClassNameA();

			bool bPlayer = pClassName.CompareNoCase( xorstr_( "BasePlayer" ) ) == 0;
			bool bBandit = pClassName.CompareNoCase( xorstr_( "BanditGuard" ) ) == 0;
			bool bScientist = pClassName.CompareNoCase( xorstr_( "ScientistNPC" ) ) == 0;
			bool bDwellerTL = pClassName.CompareNoCase( xorstr_( "TunnelDweller" ) ) == 0;
			bool bDwellerUW = pClassName.CompareNoCase( xorstr_( "UnderwaterDweller" ) ) == 0;

			bool bAnimalNPC = 
				pClassName.CompareNoCase( xorstr_( "Boar" ) ) == 0 ||
				pClassName.CompareNoCase( xorstr_( "Stag" ) ) == 0;
				pClassName.CompareNoCase( xorstr_( "Dog" ) ) == 0;
				pClassName.CompareNoCase( xorstr_( "Wolf" ) ) == 0;

			auto color = m_mapRandomColor[ pClassName.GetString() ];

			DWORD dwFlags1 = pEntity->GetValue<DWORD>( 0x24 );
			DWORD dwFlags2 = pEntity->GetValue<DWORD>( 0x2C );

			if (bPlayer || bBandit || bScientist || bDwellerTL || bDwellerUW)
			{
				_stBasePlayer cPlayer;

				auto pPlayer = pEntity->ToPlayer();
				if (!pPlayer) continue;

				auto pPlayerModel = pPlayer->GetModel();
				if (!pPlayerModel) continue;

				if (pLocalBasePlayer == pPlayer)
					continue;

				cPlayer.cColors = color.color;
				cPlayer.pBasePlayer = pPlayer;
				cPlayer.dwTraitFlags = dwFlags2;
				cPlayer.pPlayerModel = pPlayerModel;
				cPlayer.pModel = pEntity->GetModel();
				cPlayer.IsHumanNPC = pPlayerModel->IsNPC();

				cPlayer.isDead = pPlayer->wasDead();
				cPlayer.isSleeping = pPlayer->wasSleeping();
				cPlayer.isLocal = pPlayer == pLocalBasePlayer;
				cPlayer.lifeState = pPlayer->GetLifeState();
				cPlayer.faction = pPlayer->GetFaction();

				cPlayer.position = pPlayerModel->GetPosition();
				cPlayer.health = pPlayer->GetHealth();
				cPlayer.maxHealth = pPlayer->GetMaxHealth();
				cPlayer.distance = Math::Calc3D_Dist( cPlayer.position, vPos );

				if (bPlayer)
					StrCpy( cPlayer.szName, pPlayer->displayName()->GetString() );
				if (bBandit)
					StrCpy( cPlayer.szName, xorstr_( "[NPC] Bandit" ) );
				if (bScientist)
					StrCpy( cPlayer.szName, xorstr_( "[NPC] Scientist" ) );
				if (bDwellerUW)
					StrCpy( cPlayer.szName, xorstr_( "[NPC] Underwater Dweller" ) );
				if (bDwellerTL)
					StrCpy( cPlayer.szName, xorstr_( "[NPC] Tunnel Dweller" ) );

				if (bPlayer == FALSE)
				{
					if (cPlayer.distance > fRenderDistance)
					{
						continue;
					}
				}

				vecBasePlayer.push_back( cPlayer );
			}
			else if (bAnimalNPC)
			{
				auto pAnimal = (CBaseAnimalNPC*)pEntity->ToBaseNPC();
				if (!pAnimal) continue;

				auto pModel = pEntity->GetModel();
				if (!pModel) continue;

				_stBaseAnimal cAnimal {};

				cAnimal.dwTraitFlags = dwFlags2;
				cAnimal.cColors = color.color;
				cAnimal.pModel = pModel;
				cAnimal.distance = Math::Calc3D_Dist( cAnimal.position, vPos );
				cAnimal.position = pModel->GetBonePosition( BoneList::head );
				pAnimal->GetValue( &cAnimal.cAiStatistics, 0x2A0, sizeof( AiStatistics ) );

				vecBaseAnimal.push_back( cAnimal );
			}
			else
			{
				_stBaseEntity cEntity;
				auto pModel = pEntity->GetModel();
				if (!pModel) continue;

				auto pPlayer = (CBasePlayer*)pEntity;

				auto headPos = pModel->GetBonePosition( BoneList::head );
				float fDistance = Math::Calc3D_Dist( vPos, headPos );

				if (fDistance <= fRenderDistance)
				{
					cEntity.dwTraitFlags = dwFlags2;
					cEntity.cColors = color.color;
					cEntity.distance = fDistance;
					cEntity.pModel = pModel;

					DWORD faction = pPlayer->GetFaction();

					CStringA Fac;
					Fac.Format( xorstr_( "[%X][%d][%d] %s" ), faction, dwFlags1, dwFlags2, pClassName );
					StrCpy( cEntity.szClassName, Fac );

					vecBaseEntity.push_back( cEntity );
				}
			}
		}

		g_vecBaseEntity.swap( vecBaseEntity );
		g_vecBaseAnimal.swap( vecBaseAnimal );
		g_vecBasePlayer.swap( vecBasePlayer );
	}
}

void CEntity::Update( void )
{
	if (g_pLocalPlayer && g_pBaseNetworkable)
	{
		OnGameEnter();
		Initialize();
	}
	else
	{
		OnGameLeave();
	}
}

void CEntity::Settings( void )
{
}

void CEntity::OnGameEnter( void )
{
	RECT rect;

	if (camera_manager_offset == NULL)
		camera_manager_offset = ReadPhysMemory<uintptr_t>( kinterface->ModuleBase + IOffset::dwCameraManager );//"Releasing render texture that is set as Camera.targetTexture!"
	if (camera_manager == NULL)
		camera_manager = ReadPhysMemory<uintptr_t>( camera_manager_offset + 0xB8 );
	if (camera == NULL)
		camera = ReadPhysMemory<uintptr_t>( camera_manager );
	if (camera_object == NULL)
		camera_object = ReadPhysMemory<uintptr_t>( camera + 0x10 );

	if (IGame::ViewMatrix == NULL)
		IGame::ViewMatrix = new Matrix4x4();
	if (IGame::ViewMatrix)
		ReadPhysMemory( camera_object + 0x2E4, IGame::ViewMatrix, sizeof( Matrix4x4 ) );

	if (GetWindowRect( overlay->hTargetWindow, &rect ))
	{
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		IGame::vRustWindow = Vector2( width, height );
	}
}

void CEntity::OnGameLeave( void )
{
	camera = NULL;
	camera_object = NULL;
	camera_manager = NULL;
	camera_manager_offset = NULL;

	if (g_vecBaseEntity.size())
		g_vecBaseEntity.clear();
	if (g_vecBasePlayer.size())
		g_vecBasePlayer.clear();
	if (mapOtherEntities.size())
		mapOtherEntities.clear();
}

void CEntity::RenderDrawing( void )
{
	Vector2 screen;

	auto drawList = ImGui::GetBackgroundDrawList();
	if (!drawList) return;

	g_pRender->Init_DrawList( drawList );

	for (auto &player : g_vecBasePlayer)
	{
		auto color = player.cColors;

		player.position = player.pModel->GetBonePosition( head );

		if (player.position.W2S( screen ))
		{
			g_pRender->AddText( screen, TextAlign_Center, ImColor( color.r, color.g, color.b ), player.szName );
		}

		if (bBones)
			DrawBones( drawList, player.pModel, ImColor( color.r, color.g, color.b ) );
		if (bBox2D)
			DrawBox2D( player.pModel, ImColor( color.r, color.g, color.b ) );
	}

	for (auto& entity : g_vecBaseEntity)
	{
		auto color = entity.cColors;

		entity.position = entity.pModel->GetBonePosition( head );

		if (entity.position.W2S( screen ))
		{
			ImVec2 vPos = ImVec2( screen.x, screen.y );

			if (entity.pModel)
			{
				if (bBones)
					DrawBones( drawList, entity.pModel, ImColor( color.r, color.g, color.b ) );
				if (bBox2D)
					DrawBox2D( entity.pModel, ImColor( color.r, color.g, color.b ) );
			}

			g_pRender->AddText( screen, TextAlign_Center, ImColor( color.r, color.g, color.b ), xorstr_( "[%.0f] %s" ), entity.distance, entity.szClassName );
		}
	}

	for (auto& animal : g_vecBaseAnimal)
	{
		auto color = animal.cColors;

		animal.position = animal.pModel->GetBonePosition( head );

		if (animal.position.W2S( screen ))
		{
			g_pRender->AddText( screen, TextAlign_Center, ImColor( color.r, color.g, color.b ), xorstr_( "[%.0f] %s" ), animal.distance, animal.szClassName );
		}
	}
}

void CEntity::Initialize( void )
{
	EntityList();
}
