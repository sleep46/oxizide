#include "../includes.h"
#include "Menu.h"

void cMenu::RenderMenu( ) 
{
	ImGui::SetNextWindowSize( { 400, 450 } );

	if ( ImGui::Begin( xorstr_( "Oxidize" ), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize ) ) 
	{
		ImGui::TextColored( ImVec4( 0, 1, 1, 1 ), xorstr_( "FPS: %.0f" ), ImGui::GetIO().Framerate );

		ImGui::SeparatorText( xorstr_( "Pointers | Oxidize" ) );
		ImGui::Text( xorstr_("GameAssembly: %p"), kinterface->ModuleBase );
		ImGui::Text( xorstr_("g_pLocalPlayer: %p"), g_pLocalPlayer );
		ImGui::Text( xorstr_("g_pBaseNetworkable: %p"), g_pBaseNetworkable );

		ImGui::SeparatorText( xorstr_( "Entities" ) );
		ImGui::Text( xorstr_("BasePlayer: %d"), g_vecBasePlayer.size() );
		ImGui::Text( xorstr_("BaseEntity: %d"), g_vecBaseEntity.size() );
		ImGui::Text( xorstr_("BaseAnimal: %d"), g_vecBaseAnimal.size() );
		ImGui::Text( xorstr_("ExtraEntities: %d"), g_pEntity->mapOtherEntities.size() );

		ImGui::SeparatorText( xorstr_( "ESP Features | Oxidize" ) );
		ImGui::Checkbox( xorstr_( "Human Box2D ESP" ), &g_pEntity->bBox2D );
		ImGui::Checkbox( xorstr_( "Human Bones ESP" ), &g_pEntity->bBones );

		ImGui::SeparatorText( xorstr_( "Render Distance | Oxidize" ) );
		ImGui::SliderFloat( xorstr_( "##Distance" ), &g_pEntity->fRenderDistance, 1.0f, 1000.0f, xorstr_( "Distance: %.0f" ) );

		if (ImGui::TreeNode( xorstr_("Extra Entities | Oxidize")))
		{
			for (auto& others : g_pEntity->mapOtherEntities)
			{
				ImGui::Checkbox( others.first.c_str(), &others.second );
			}
			ImGui::TreePop();
		}
	}
	ImGui::End( );
}

void cMenu::InitStyle( ) {
	ImGuiIO& io = ImGui::GetIO( );
	m_pFont = io.Fonts->AddFontDefault();
}