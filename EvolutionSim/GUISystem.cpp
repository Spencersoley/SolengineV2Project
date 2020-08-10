#include "BeingManager.h"
#include "GenerationManager.h"
#include "DataPointManager.h"
#include "SelectedTracker.h"
#include "OverlayConfig.h"
#include "TextureLibrary.h"

#include "TextureLoaderSystemImplementation.h"

#include "GUISystemImplementation.h"
#include "DataSystemImplementation.h"
#include "GenerationSystemImplementation.h"
#include "GeneSystemImplementation.h"
#include "OverlaySystemImplementation.h"
#include "SelectableSystemImplementation.h"
#include "SurvivalSystemImplementation.h"
#include "VelocitySystemImplementation.h"

#include "GeneEnum.h"

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

using BeingType = Gene::BeingType;
using Trait = Gene::Trait;

void GUISystem::init(SolengineV2::Window& wndw)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsCustom();

	ImGui_ImplSDL2_InitForOpenGL(wndw.getWindow(), wndw.getContext());
	const char* glslVersion = "#version 130";
	ImGui_ImplOpenGL3_Init(glslVersion);
};

void GUISystem::update(BeingManager& beings, DataPointManager& data, GenerationManager& generation, OverlayConfig& overlayConfig, SelectedTracker& selected, TextureLibrary& textureLibrary, SolengineV2::Window& window, SolengineV2::GameState& state, unsigned int deltaTime)
{
	static unsigned int microsecondsFromLastUpdate = 0;
	microsecondsFromLastUpdate += deltaTime;

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
				ImGui::SliderFloat(PHYSICS_SPEED_TEXT, &Generation::System::getPhysicsSpeedRef(generation.component), 1, 1000);
				ImGui::SliderInt(FOOD_PER_GENERATION_TEXT, &Generation::System::getFoodPerGenerationRef(generation.component), 1, 150);
				ImGui::Text(WAVE_NUMBER_TEXT, Generation::System::getWaveCount(generation.component));
				ImGui::Text(ANIMAL_COUNT_TEXT, Generation::System::getAnimalCount(generation.component));
				ImGui::Text(PLANT_COUNT_TEXT, Generation::System::getPlantCount(generation.component));
				if (ImGui::Button("Reset", ImVec2(225, 0)))
				{
					resetGame(state, beings, generation, data, selected, textureLibrary);
				}
			}
			else if (menu == Menu::HELP)
			{
				ImGui::BeginChild("GameRender");
				ImVec2 guiWindowSize = ImGui::GetWindowSize();

				ImGui::SameLine();
				ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::BEING_1, textureLibrary), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
				ImGui::SameLine();
				ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::MEAT, textureLibrary), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
				ImGui::SameLine();
				ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::PLANT, textureLibrary), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));

				if (ImGui::CollapsingHeader("Controls"))
				{
					ImGui::Dummy(ImVec2(60.0f, 20.0f));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::KEYS, textureLibrary), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::CAMERA, textureLibrary), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));

					ImGui::Dummy(ImVec2(60.0f, 20.0f));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::KEYS_2, textureLibrary), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::ZOOM, textureLibrary), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));


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
			int size = Data::System::getData(data.speed).size();
			if (size > dataSizeMilestone)
			{
				//resize data
				Data::System::reserveAdditional(data.speed, 1000);
				Data::System::reserveAdditional(data.stamina, 1000);
				Data::System::reserveAdditional(data.strength, 1000);
				Data::System::reserveAdditional(data.diet, 1000);
				Data::System::reserveAdditional(data.animalPopulation, 1000);
				Data::System::reserveAdditional(data.plantPopulation, 1000);

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
				ImPlot::PlotLine("Speed", x.data(), Data::System::getData(data.speed).data(), size, 0);
				ImPlot::PlotLine("Strength", x.data(), Data::System::getData(data.strength).data(), size, 0);
				ImPlot::PlotLine("Diet", x.data(), Data::System::getData(data.diet).data(), size, 0);
				ImPlot::PlotLine("Stamina", x.data(), Data::System::getData(data.stamina).data(), size, 0);

				ImPlot::EndPlot();
			}
			else if (ImPlot::SetNextPlotLimitsY(0.0f, Data::System::getData(data.beingMaximum).back(), ImGuiCond_Always); ImGui::CollapsingHeader("Population history"))
			{
				ImPlot::SetColormap(ImPlotColormap_Default, 5);

				ImPlot::BeginPlot("Population history", "Generations", "Population");
				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
				ImPlot::PlotLine("Animal", x.data(), Data::System::getData(data.animalPopulation).data(), size, 0);
				ImPlot::PlotLine("Plant", x.data(), Data::System::getData(data.plantPopulation).data(), size, 0);
				ImPlot::EndPlot();
			}
			ImGui::End();
		}

		// SELECTED BEING
		if (ImGui::Begin("Selected Being Panel"))
		{
			if (const Handle selectedHandle = Selectable::System::getHandle(selected.component); selectedHandle < beings.getSize())
			{
				const GeneComponent& gene = beings.pool[selectedHandle].gene;
				ImGui::Text(Gene::System::getBeingType(gene) == BeingType::ANIMAL ? "Animal" : "Plant");
				if (Gene::System::getBeingType(gene) == BeingType::ANIMAL)
				{
					ImGui::Text(BEING_SPEED_TEXT, Gene::System::getTrait(gene, Trait::SPEED));
					ImGui::Text(BEING_DIET_TEXT, Gene::System::getTrait(gene, Trait::DIET));
					ImGui::Text(BEING_STRENGTH_TEXT, Gene::System::getTrait(gene, Trait::STRENGTH));
					ImGui::Text(BEING_STAMINA_TEXT, Gene::System::getTrait(gene, Trait::STAMINA));
					ImGui::Text(BEING_FULLNESS_TEXT, static_cast<int>(0.1f * Survival::System::getFullness(beings.pool[selectedHandle].survival) / Gene::System::getTrait(gene, Trait::HUNGER)));
					ImGui::Text(BEING_ENERGY_TEXT, static_cast<int>(0.1f * Survival::System::getEnergy(beings.pool[selectedHandle].survival) / Gene::System::getTrait(gene, Trait::STAMINA)));
				}
			}
			ImGui::End();
		}

		/// OVERLAY KEY ///
		if (ImGui::Begin("Overlay"))
		{
			OverlayMode overlay = Overlay::System::getOverlay(overlayConfig.component);
			ImVec2 guiWindowSize = ImGui::GetWindowSize();

			if (ImGui::Button("Default", ImVec2(100, 25)))
			{
				Overlay::System::setOverlay(overlayConfig.component, OverlayMode::DEFAULT);
				Overlay::System::updateOverlay(beings, overlayConfig);
			}
			if (ImGui::Button("Strength", ImVec2(100, 25)))
			{
				Overlay::System::setOverlay(overlayConfig.component, OverlayMode::STRENGTH);
				Overlay::System::updateOverlay(beings, overlayConfig);
			}
			if (ImGui::Button("Stamina", ImVec2(100, 25)))
			{
				Overlay::System::setOverlay(overlayConfig.component, OverlayMode::STAMINA);
				Overlay::System::updateOverlay(beings, overlayConfig);
			}
			if (ImGui::Button("Speed", ImVec2(100, 25)))
			{
				Overlay::System::setOverlay(overlayConfig.component, OverlayMode::SPEED);
				Overlay::System::updateOverlay(beings, overlayConfig);
			}
			if (ImGui::Button("Diet", ImVec2(100, 25)))
			{
				Overlay::System::setOverlay(overlayConfig.component, OverlayMode::DIET);
				Overlay::System::updateOverlay(beings, overlayConfig);
			}


			if (overlay == OverlayMode::DEFAULT)
			{

			}
			else if (overlay == OverlayMode::STAMINA)
			{
				ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::STAMINA_GRADIENT, textureLibrary), guiWindowSize, ImVec2(0, -1), ImVec2(1, 0));

			}
			else if (overlay == OverlayMode::SPEED)
			{
				ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::SPEED_GRADIENT, textureLibrary), guiWindowSize, ImVec2(0, -1), ImVec2(1, 0));

			}
			else if (overlay == OverlayMode::STRENGTH)
			{
				ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::STRENGTH_GRADIENT, textureLibrary), guiWindowSize, ImVec2(0, -1), ImVec2(1, 0));

			}
			else if (overlay == OverlayMode::DIET)
			{
				ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibrary::Texture::DIET_GRADIENT, textureLibrary), guiWindowSize, ImVec2(0, -1), ImVec2(1, 0));

			}
			ImGui::End();
		}

		/// RESET MENU ///
		if (Generation::System::getAnimalCount(generation.component) == 0)
		{
			ImGui::Begin("There are no more animals alive. The simulation has ended!");
			state = SolengineV2::GameState::PAUSE;

			if (ImGui::Button("Reset", ImVec2(225, 0)))
			{
				resetGame(state, beings, generation, data, selected, textureLibrary);
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

inline void GUISystem::resetGame(SolengineV2::GameState& state, BeingManager& beings, GenerationManager& generation, DataPointManager& data, SelectedTracker& selected, TextureLibrary& textureLibrary)
{
	state = SolengineV2::GameState::PLAY;
	Data::System::clear(data.speed);
	Data::System::clear(data.stamina);
	Data::System::clear(data.strength);
	Data::System::clear(data.diet);
	Data::System::clear(data.animalPopulation);
	Data::System::clear(data.beingMaximum);
	Data::System::clear(data.plantPopulation);

	Generation::System::begin(beings, generation, selected, textureLibrary);
}
