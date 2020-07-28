#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <IMGUI/implot.h>
#include <numeric>
#include <Window.h>

#include "GameState.h"

#include "BeingManager.h"
#include "DataSystem.h"
#include "GenerationSystem.h"
#include "GeneSystem.h"
#include "GUISystem.h"
#include "OverlaySystem.h"
#include "SelectableSystem.h"
#include "SurvivalSystem.h"
#include "TextureSystem.h"
#include "VelocitySystem.h"

constexpr const char* WAVE_NUMBER_TEXT{ "Wave number: %d" };
constexpr const char* PHYSICS_SPEED_TEXT{ "Physics speed" };
constexpr const char* FOOD_PER_GENERATION_TEXT{ "Land fertility" };
constexpr const char* BEING_SPEED_TEXT{ "Being speed: %f" };
constexpr const char* BEING_DIET_TEXT{ "Being diet: %f" };
constexpr const char* BEING_STRENGTH_TEXT{ "Being strength: %f" };
constexpr const char* BEING_STAMINA_TEXT{ "Being stamina: %f" };
constexpr const char* BEING_ENERGY_TEXT{ "Being energy: %i" };
constexpr const char* BEING_FULLNESS_TEXT{ "Being fullness: %i" };
constexpr const char* PLANT_COUNT_TEXT{ "Plants: %i" };
constexpr const char* ANIMAL_COUNT_TEXT{ "Animals: %i" };

constexpr const unsigned int MICROSECONDS_PER_UPDATE = 50000;

using Data = DataSystem::Data;
using BeingType = GeneComponent::BeingType;
using Trait = GeneComponent::Trait;
using Handle = unsigned int;

GUISystem::GUISystem(
	SolengineV2::Window& wndw,
	const TextureSystem& textureSys,
	VelocitySystem& velSys,
	GenerationSystem& genSys,
	DataSystem& dataSys,
	OverlaySystem& overlaySys,
	const GeneSystem& geneSys,
	const SelectableSystem& selSys,
	const SurvivalSystem& survSys
) :
	window(wndw),
	textureSystem(textureSys),
	velocitySystem(velSys),
	generationSystem(genSys),
	dataSystem(dataSys),
	overlaySystem(overlaySys),
	geneSystem(geneSys),
	selectableSystem(selSys),
	survivalSystem(survSys)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO();

	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsCustom();

	ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getContext());
	const char* glslVersion = "#version 130";
	ImGui_ImplOpenGL3_Init(glslVersion);
}

void GUISystem::update(BeingManager& beings, SolengineV2::GameState& state, unsigned int dt)
{
	static unsigned int microsecondsFromLastUpdate = 0;
	microsecondsFromLastUpdate += dt;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window.getWindow());

	if (microsecondsFromLastUpdate > MICROSECONDS_PER_UPDATE)
	{
		microsecondsFromLastUpdate = 0;
		ImGui::NewFrame();

		//bool f = true;
		//ImPlot::ShowDemoWindow(&f);

		/// CONTOL PANEL ///
		if (ImGui::Begin("Control Panel"))
		{
			ImGui::SameLine();
			if (ImGui::Button("Settings", ImVec2(100, 25)))
			{
				menu = Menu::SETTINGS;
			}
			ImGui::SameLine();
			if (ImGui::Button("Help", ImVec2(100, 25)))
			{
				menu = Menu::HELP;
			}

			if (menu == Menu::SETTINGS)
			{
				ImGui::SliderFloat(PHYSICS_SPEED_TEXT, &velocitySystem.getPhysicsSpeed(), 1, 1000);
				ImGui::SliderInt(FOOD_PER_GENERATION_TEXT, &generationSystem.getFoodPerGeneration(), 1, 150);
				ImGui::Text(WAVE_NUMBER_TEXT, generationSystem.getWaveCount());
				ImGui::Text(ANIMAL_COUNT_TEXT, generationSystem.getAnimalCount());
				ImGui::Text(PLANT_COUNT_TEXT, generationSystem.getPlantCount());
				if (ImGui::Button("Reset", ImVec2(225, 0)))
				{
					resetGame(state, beings);
				}
			}
			else if (menu == Menu::HELP)
			{
				ImGui::BeginChild("GameRender");
				ImVec2 guiWindowSize = ImGui::GetWindowSize();

				ImGui::SameLine();
				ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::BEING_1), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
				ImGui::SameLine();
				ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::MEAT), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
				ImGui::SameLine();
				ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::PLANT), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));

				if (ImGui::CollapsingHeader("Controls"))
				{
					ImGui::Dummy(ImVec2(60.0f, 20.0f));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::KEYS), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::CAMERA), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));

					ImGui::Dummy(ImVec2(60.0f, 20.0f));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::KEYS_2), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::ZOOM), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));


					ImGui::EndChild();
				}


				if (ImGui::CollapsingHeader("About"))
				{

				}

				ImGui::EndChild();

			}
			else if (menu == Menu::OVERLAY)
			{

			}
			ImGui::End();
		}

		/// DATA PLOTS ///
		if (ImGui::Begin("Plots"))
		{
			static int dataSizeMilestone = 1000;
			int size = dataSystem.getData(Data::SPEED).size();
			if (size > dataSizeMilestone)
			{
				//resize data
				dataSystem.reserveAdditional(Data::SPEED, 1000);
				dataSystem.reserveAdditional(Data::STAMINA, 1000);
				dataSystem.reserveAdditional(Data::STRENGTH, 1000);
				dataSystem.reserveAdditional(Data::DIET, 1000);
				dataSystem.reserveAdditional(Data::ANIMAL_POPULATION, 1000);
				dataSystem.reserveAdditional(Data::PLANT_POPULATION, 1000);

				dataSizeMilestone += 1000;
			}

			//set capacity for data in chunks of 1000 waves
			std::vector<float> x(size);
			std::iota(x.begin(), x.end(), 0);
			ImPlot::SetNextPlotLimitsY(0.0f, 1.0f, ImGuiCond_Always);
			if (ImPlot::SetNextPlotLimitsX(0.0f, std::max(2 * size, 15), ImGuiCond_Always); ImGui::CollapsingHeader("Trait history"))
			{
				ImPlot::SetColormap(ImPlotColormap_Plasma, 5);

				ImPlot::BeginPlot("Trait history", "Generations", "Genes");
				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
				ImPlot::PlotLine("Speed", x.data(), dataSystem.getData(Data::SPEED).data(), size, 0);
				ImPlot::PlotLine("Strength", x.data(), dataSystem.getData(Data::STRENGTH).data(), size, 0);
				ImPlot::PlotLine("Diet", x.data(), dataSystem.getData(Data::DIET).data(), size, 0);
				ImPlot::PlotLine("Stamina", x.data(), dataSystem.getData(Data::STAMINA).data(), size, 0);

				ImPlot::EndPlot();
			}
			else if (ImPlot::SetNextPlotLimitsY(0.0f, dataSystem.getData(Data::BEING_MAXIMUM).back(), ImGuiCond_Always); ImGui::CollapsingHeader("Population history"))
			{
				ImPlot::SetColormap(ImPlotColormap_Default, 5);

				ImPlot::BeginPlot("Population history", "Generations", "Population");
				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
				ImPlot::PlotLine("Animal", x.data(), dataSystem.getData(Data::ANIMAL_POPULATION).data(), size, 0);
				ImPlot::PlotLine("Plant", x.data(), dataSystem.getData(Data::PLANT_POPULATION).data(), size, 0);
				ImPlot::EndPlot();
			}
			ImGui::End();
		}

		// SELECTED BEING
		if (ImGui::Begin("Selected Being Panel"))
		{
			if (const Handle selectedHandle = selectableSystem.getCSelectedHandle(); selectedHandle < beings.getSize())
			{
				const GeneComponent& gene = beings.pool[selectedHandle].gene;
				ImGui::Text(geneSystem.getBeingType(gene) == BeingType::ANIMAL ? "Animal" : "Plant");
				if (geneSystem.getBeingType(gene) == BeingType::ANIMAL)
				{
					ImGui::Text(BEING_SPEED_TEXT, geneSystem.getTrait(gene, Trait::SPEED));
					ImGui::Text(BEING_DIET_TEXT, geneSystem.getTrait(gene, Trait::DIET));
					ImGui::Text(BEING_STRENGTH_TEXT, geneSystem.getTrait(gene, Trait::STRENGTH));
					ImGui::Text(BEING_STAMINA_TEXT, geneSystem.getTrait(gene, Trait::STAMINA));
					ImGui::Text(BEING_FULLNESS_TEXT, static_cast<int>(0.1f * survivalSystem.getBeingFullness(beings.pool[selectedHandle].survival) / geneSystem.getTrait(gene, Trait::HUNGER)));
					ImGui::Text(BEING_ENERGY_TEXT, static_cast<int>(0.1f * survivalSystem.getBeingEnergy(beings.pool[selectedHandle].survival) / geneSystem.getTrait(gene, Trait::STAMINA)));
				}
			}
			ImGui::End();
		}

		/// OVERLAY KEY ///
		if (ImGui::Begin("Overlay"))
		{

			OverlaySystem::Overlay overlay = overlaySystem.getOverlay();
			ImVec2 guiWindowSize = ImGui::GetWindowSize();

			if (ImGui::Button("Default", ImVec2(100, 25)))
			{
				overlaySystem.setOverlay(OverlaySystem::Overlay::DEFAULT);
				overlaySystem.updateOverlay(beings);
			}
			if (ImGui::Button("Strength", ImVec2(100, 25)))
			{
				overlaySystem.setOverlay(OverlaySystem::Overlay::STRENGTH);
				overlaySystem.updateOverlay(beings);
			}
			if (ImGui::Button("Stamina", ImVec2(100, 25)))
			{
				overlaySystem.setOverlay(OverlaySystem::Overlay::STAMINA);
				overlaySystem.updateOverlay(beings);
			}
			if (ImGui::Button("Speed", ImVec2(100, 25)))
			{
				overlaySystem.setOverlay(OverlaySystem::Overlay::SPEED);
				overlaySystem.updateOverlay(beings);
			}
			if (ImGui::Button("Diet", ImVec2(100, 25)))
			{
				overlaySystem.setOverlay(OverlaySystem::Overlay::DIET);
				overlaySystem.updateOverlay(beings);
			}


			if (overlay == OverlaySystem::Overlay::DEFAULT)
			{

			}
			else if (overlay == OverlaySystem::Overlay::STAMINA)
			{
				ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::STAMINA_GRADIENT), guiWindowSize, ImVec2(0, -1), ImVec2(1, 0));

			}
			else if (overlay == OverlaySystem::Overlay::SPEED)
			{
				ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::SPEED_GRADIENT), guiWindowSize, ImVec2(0, -1), ImVec2(1, 0));

			}
			else if (overlay == OverlaySystem::Overlay::STRENGTH)
			{
				ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::STRENGTH_GRADIENT), guiWindowSize, ImVec2(0, -1), ImVec2(1, 0));

			}
			else if (overlay == OverlaySystem::Overlay::DIET)
			{
				ImGui::Image((ImTextureID)textureSystem.getTextureID(TextureSystem::Component::DIET_GRADIENT), guiWindowSize, ImVec2(0, -1), ImVec2(1, 0));

			}
			ImGui::End();
		}

		/// RESET MENU ///
		if (generationSystem.getAnimalCount() == 0)
		{
			ImGui::Begin("There are no more animals alive. The simulation has ended!");
			state = SolengineV2::GameState::PAUSE;

			if (ImGui::Button("Reset", ImVec2(225, 0)))
			{
				resetGame(state, beings);
			}

			if (ImGui::Button("Exit", ImVec2(225, 0)))
			{
				state = SolengineV2::GameState::EXIT;
			}
			ImGui::End();
		}

		ImGui::Render();
	}
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUISystem::resetGame(SolengineV2::GameState& state, BeingManager& beings)
{
	state = SolengineV2::GameState::PLAY;
	dataSystem.reset();
	generationSystem.begin(beings);
}