#include "GameData.h"
#include "TextureLoaderSystemSpecialization.h"
#include "DataSystemSpecialization.h"
#include "FoodSystemSpecialization.h"
#include "GenerationSystemSpecialization.h"
#include "GeneSystemSpecialization.h"
#include "GUISystemSpecialization.h"
#include "OverlaySystemSpecialization.h"
#include "PartitionSystemSpecialization.h"
#include "SelectableSystemSpecialization.h"
#include "AggressionTraitSystemSpecialization.h"
#include "DietTraitSystemSpecialization.h"
#include "HungerTraitSystemSpecialization.h"
#include "HealthTraitSystemSpecialization.h"
#include "IntelligenceTraitSystemSpecialization.h"
#include "SpeedTraitSystemSpecialization.h"
#include "StaminaTraitSystemSpecialization.h"
#include "StrengthTraitSystemSpecialization.h"
#include "Math.h"

constexpr const char* WAVE_NUMBER_TEXT{ "Wave number: %d" };
constexpr const char* PHYSICS_SPEED_TEXT{ "Physics speed" };
constexpr const char* FOOD_PER_GENERATION_TEXT{ "Land fertility" };
constexpr const char* BEING_AGGRESSION_TEXT{ "Being aggression: %f" };
constexpr const char* BEING_DIET_TEXT{ "Being diet: %f" };
constexpr const char* BEING_HEALTH_TEXT{ "Being health: %f" };
constexpr const char* BEING_INTELLIGENCE_TEXT{ "Being intelligence: %f" };
constexpr const char* BEING_SPEED_TEXT{ "Being speed: %f" };
constexpr const char* BEING_STAMINA_TEXT{ "Being stamina: %f" };
constexpr const char* BEING_STRENGTH_TEXT{ "Being strength: %f" };
constexpr const char* BEING_ENERGY_TEXT{ "Current energy: %.2f" };
constexpr const char* BEING_FULLNESS_TEXT{ "Current fullness: %.2f" };
constexpr const char* BEING_CURRENT_HEALTH_TEXT{ "Current health: %.2f" };
constexpr const char* PLANT_COUNT_TEXT{ "Plants: %i" };
constexpr const char* ANIMAL_COUNT_TEXT{ "Animals: %i" };

constexpr std::array<const char*, static_cast<int>(OverlayMode::N_OF_MODES)> overlayStrArr
{
	"Default",
	"Aggression",
	"Diet",
	"Health",
	"Hunger",
	"Intelligence",
	"Speed",
	"Stamina",
	"Strength",
};

constexpr std::array<OverlayMode, static_cast<int>(OverlayMode::N_OF_MODES)> overlayEnumArr
{
	OverlayMode::DEFAULT,
	OverlayMode::AGGRESSION,
	OverlayMode::DIET,
	OverlayMode::HEALTH,
	OverlayMode::HUNGER,
	OverlayMode::INTELLIGENCE,
	OverlayMode::SPEED,
	OverlayMode::STAMINA,
	OverlayMode::STRENGTH,
};

constexpr const unsigned int MICROSECONDS_PER_UPDATE = 100000;

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

void GUISystem::update(SolengineV2::Window& window, SolengineV2::GameState& state, const std::chrono::microseconds& deltaTime, GameData& gameData)
{
	static unsigned int microsecondsFromLastUpdate = 0;
	microsecondsFromLastUpdate += static_cast<float>(deltaTime.count());

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window.getWindow());

	if (microsecondsFromLastUpdate > MICROSECONDS_PER_UPDATE)
	{
		ImGui::NewFrame();

		//bool f = true;
		//ImPlot::ShowDemoWindow(&f);

		/// CONTOL PANEL ///
		if (ImGui::Begin("Control Panel"))
		{
			ImGui::SameLine();
			if (ImGui::Button("Info", ImVec2(100, 25)))
			{
				menu = Menu::INFO;
			}
			ImGui::SameLine();
			if (ImGui::Button("Settings", ImVec2(100, 25)))
			{
				menu = Menu::SETTINGS;
			}
	

			if (menu == Menu::SETTINGS)
			{
				ImGui::SliderFloat(PHYSICS_SPEED_TEXT, &gameData.configurableSettings.physicsSpeed.get0to1(), 0.0f, 1.0f);
				ImGui::SliderFloat(FOOD_PER_GENERATION_TEXT, &gameData.configurableSettings.foodPerGeneration.get0to1(), 0.0f, 1.0f);
				float arenaDiameter = gameData.configurableSettings.arenaDiameter.get0to1();
				ImGui::SliderFloat("Arena Diamater", &gameData.configurableSettings.arenaDiameter.get0to1(), 0.0f, 1.0f);
				if (arenaDiameter != gameData.configurableSettings.arenaDiameter.get0to1())
				{
					PartitionComponent& partitionComponent = gameData.spatialPartition.partition;
					size_t cellCount = Math::getNOfCellsPerRow(gameData.configurableSettings.arenaDiameter.get(), CELL_SIZE);
					Partition::System::setAnimalGrid(partitionComponent, Grid(cellCount, Row(cellCount, Cell{})));
					Partition::System::setFoodGrid(partitionComponent, Grid(cellCount, Row(cellCount, Cell{})));
				}
				ImGui::SliderFloat("Combat rate", &gameData.configurableSettings.combatRateMultiplier.get0to1(), 0.0f, 1.0f);
				ImGui::SliderFloat("Strength combat bonus", &gameData.configurableSettings.strengthCombatMultiplier.get0to1(), 0.0f, 1.0f);
				ImGui::SliderFloat("Food consumption rate", &gameData.configurableSettings.foodRateMultiplier.get0to1(), 0.0f, 1.0f);
				ImGui::SliderFloat("Poison damage rate", &gameData.configurableSettings.poisonDamageRateMultiplier.get0to1(), 0.0f, 1.0f);
				ImGui::SliderFloat("Toxic deterioration rate", &gameData.configurableSettings.toxicDeteriorationRateMultiplier.get0to1(), 0.0f, 1.0f);
				ImGui::SliderFloat("Default plant saturation", &gameData.configurableSettings.plantSaturation.get0to1(), 0.0f, 1.0f);
				ImGui::SliderFloat("Energy depletion rate", &gameData.configurableSettings.staminaDepletionRate.get0to1(), 0.0f, 1.0f);
	
			}
			else if (menu == Menu::INFO)
			{
				ImGui::Text(WAVE_NUMBER_TEXT, Generation::System::getWaveCount(gameData.generation.component));
				ImGui::Text(ANIMAL_COUNT_TEXT, Generation::System::getAnimalCount(gameData.generation.component));
				ImGui::Text(PLANT_COUNT_TEXT, Generation::System::getPlantCount(gameData.generation.component));
				if (ImGui::Button("Reset", ImVec2(225, 0)))
				{
					resetGame(state);
				}
				ImGui::CheckboxFlags("Bounded", (unsigned int*)&gameData.configurableSettings.bounded.get(), ImGuiConfigFlags_NavEnableKeyboard);

				ImGui::Text("Overlay mode: ");
				ImGui::SameLine();
				if (ImGui::BeginCombo("##combo", overlayStrArr[static_cast<int>(Overlay::System::getOverlayMode(gameData.overlayConfiguration.component))])) // The second parameter is the label previewed before opening the combo.
				{
					for (const auto mode : overlayEnumArr)
					{
						bool isCurrentMode = (Overlay::System::getOverlayMode(gameData.overlayConfiguration.component) == mode);
						if (ImGui::Selectable(overlayStrArr[static_cast<int>(mode)], isCurrentMode))
						{
							Overlay::System::setOverlay(gameData.overlayConfiguration.component, mode);
							Overlay::System::updateOverlay(gameData);
							if (isCurrentMode) ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				ImVec2 guiWindowSize = ImGui::GetWindowSize();
				ImVec2 imageSize = ImVec2(guiWindowSize.x / 3.0f, guiWindowSize.y / 16.0f);
				ImGui::Text("Overlay key: ");
				ImGui::SameLine();
				ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::GRADIENT, gameData), imageSize, ImVec2(0, -1), ImVec2(1, 0));

				if (ImGui::CollapsingHeader("Controls"))
				{
					ImGui::Dummy(ImVec2(60.0f, 20.0f));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::KEYS, gameData), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::CAMERA, gameData), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));

					ImGui::Dummy(ImVec2(60.0f, 20.0f));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::KEYS_2, gameData), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
					ImGui::SameLine();
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::ZOOM, gameData), ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4), ImVec2(0, -1), ImVec2(1, 0));
				}
			}

			ImGui::End();
		}

		/// DATA PLOTS ///
		if (ImGui::Begin("Plots"))
		{
			std::vector<float>& x = Data::System::getData(gameData.dataPoint.waveCount);
			size_t size = x.size();
			ImPlot::SetNextPlotLimitsY(0.0f, 1.0f, ImGuiCond_Always);
			if (ImPlot::SetNextPlotLimitsX(0.0f, std::max(static_cast<int>(x.back() * 1.2f), 15), ImGuiCond_Always); ImGui::CollapsingHeader("Trait history"))
			{
				ImPlot::SetColormap(ImPlotColormap_Pink, 1);
				ImPlot::BeginPlot("Trait history", "Generations", "Genes");
				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
				ImPlot::PlotLine("Stamina", x.data(), Data::System::getData(gameData.dataPoint.stamina).data(), size, 0);
				ImPlot::PlotLine("Aggression", x.data(), Data::System::getData(gameData.dataPoint.aggression).data(), size, 0);
				ImPlot::PlotLine("Strength", x.data(), Data::System::getData(gameData.dataPoint.strength).data(), size, 0);
				ImPlot::PlotLine("Hunger", x.data(), Data::System::getData(gameData.dataPoint.hunger).data(), size, 0);
				ImPlot::PlotLine("Diet", x.data(), Data::System::getData(gameData.dataPoint.diet).data(), size, 0);
				ImPlot::PlotLine("Health", x.data(), Data::System::getData(gameData.dataPoint.health).data(), size, 0);
				ImPlot::PlotLine("Intelligence", x.data(), Data::System::getData(gameData.dataPoint.intelligence).data(), size, 0);
				ImPlot::PlotLine("Speed", x.data(), Data::System::getData(gameData.dataPoint.speed).data(), size, 0);

				ImPlot::EndPlot();
			}
			else if (ImPlot::SetNextPlotLimitsY(0.0f, Data::System::getCurrentPoint(gameData.dataPoint.beingMaximum), ImGuiCond_Always); ImGui::CollapsingHeader("Population history"))
			{
				ImPlot::SetColormap(ImPlotColormap_Default, 1);
				ImPlot::BeginPlot("Population history", "Generations", "Population");
				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
				ImPlot::PlotLine("Animal", x.data(), Data::System::getData(gameData.dataPoint.animalPopulation).data(), size, 0);
				ImPlot::PlotLine("Plant", x.data(), Data::System::getData(gameData.dataPoint.plantPopulation).data(), size, 0);
				ImPlot::EndPlot();
			}
			else if (ImPlot::SetNextPlotLimitsY(0.0f, Data::System::getCurrentPoint(gameData.dataPoint.deathMaximum), ImGuiCond_Always); ImGui::CollapsingHeader("Cause of death"))
			{
				ImPlot::SetColormap(ImPlotColormap_Default, 1);
				ImPlot::BeginPlot("Population history", "Generations", "Population");
				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
				ImPlot::PlotLine("Poison", x.data(), Data::System::getData(gameData.dataPoint.causeOfDeathPoison).data(), size, 0);
				ImPlot::PlotLine("Predator", x.data(), Data::System::getData(gameData.dataPoint.causeOfDeathPredator).data(), size, 0);
				ImPlot::PlotLine("Starvation", x.data(), Data::System::getData(gameData.dataPoint.causeOfDeathStarvation).data(), size, 0);
				ImPlot::EndPlot();
			}

			ImGui::End();
		}

		// SELECTED BEING
		if (ImGui::Begin("Selected Being Panel"))
		{
			if (Handle animalHandle = Selectable::System::getSelectedAnimalHandle(gameData.selectedTracker.component); animalHandle < gameData.beingManager.getSize())
			{
				const GeneComponent& gene = gameData.beingManager.pool[animalHandle].gene;
				ImGui::Text("Animal");
				ImGui::Text("Stats:");
				const HungerTraitComponent& hungerTrait = Gene::System::getHungerTrait(gene);
				ImGui::Text(BEING_FULLNESS_TEXT, HungerTrait::System::getFullness(hungerTrait) / HungerTrait::System::getMaxFullness(hungerTrait));
				const StaminaTraitComponent& staminaTrait = Gene::System::getStaminaTrait(gene);
				ImGui::Text(BEING_ENERGY_TEXT, StaminaTrait::System::getEnergy(staminaTrait) / StaminaTrait::System::getMaxEnergy(staminaTrait));
				const HealthTraitComponent& healthTrait = Gene::System::getHealthTrait(gene);
				ImGui::Text(BEING_CURRENT_HEALTH_TEXT, HealthTrait::System::getCurrentHealth(healthTrait) / HealthTrait::System::getHealthWhenFull(healthTrait));
				ImGui::Text("Traits:");
				ImGui::Text(BEING_AGGRESSION_TEXT, AggressionTrait::System::getTraitValue(Gene::System::getAggressionTrait(gene)));
				ImGui::Text(BEING_DIET_TEXT, DietTrait::System::getTraitValue(Gene::System::getDietTrait(gene)));
				ImGui::Text(BEING_HEALTH_TEXT, HealthTrait::System::getTraitValue(healthTrait));
				ImGui::Text(BEING_INTELLIGENCE_TEXT, IntelligenceTrait::System::getTraitValue(Gene::System::getIntelligenceTrait(gene)));
				ImGui::Text(BEING_SPEED_TEXT, SpeedTrait::System::getTraitValue(Gene::System::getSpeedTrait(gene)));
				ImGui::Text(BEING_STAMINA_TEXT, StaminaTrait::System::getTraitValue(staminaTrait));
				ImGui::Text(BEING_STRENGTH_TEXT, StrengthTrait::System::getTraitValue(Gene::System::getStrengthTrait(gene)));
				const auto& hist = Gene::System::getFamilyHistory(gene);
				ImGui::Text("History: %i, %i, %i", hist.data[0], hist.data[1], hist.data[2]);
			}
			else if (Handle foodHandle = Selectable::System::getFoodHandle(gameData.selectedTracker.component); foodHandle < gameData.foodManager.getSize())
			{
				const FoodComponent& food = gameData.foodManager.pool[foodHandle].food;
				if (Food::System::getFoodType(food) == FoodType::MEAT)
				{
					ImVec2 guiWindowSize = ImGui::GetWindowSize();
					ImVec2 imageSize = ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4);
					ImVec2 imagePos = ImVec2((guiWindowSize.x - imageSize.x) * 0.5f, imageSize.y * 0.5f);
					ImGui::SetCursorPos(imagePos);
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::MEAT, gameData), imageSize, ImVec2(0, -1), ImVec2(1, 0), ImVec4(1, 0, 0, 1));
					ImGui::Text("Meat");
				}
				else
				{
					ImVec2 guiWindowSize = ImGui::GetWindowSize();
					ImVec2 imageSize = ImVec2(guiWindowSize.x / 4, guiWindowSize.y / 4);
					ImVec2 imagePos = ImVec2((guiWindowSize.x - imageSize.x) * 0.5f, imageSize.y * 0.5f);
					ImGui::SetCursorPos(imagePos);
					ImGui::Image((ImTextureID)TextureLoader::System::getTextureID(TextureLibraryEntity::Texture::PLANT, gameData), imageSize, ImVec2(0, -1), ImVec2(1, 0), ImVec4(0, 1, 0, 1));
					ImGui::Text("Plant");
				}

				ImGui::Text("Food saturation: %.2f", Food::System::getSaturation(food));
				if (Food::System::getIsToxic(food))
				{
					ImGui::Text("Toxic");
				}
				else
				{
					ImGui::Text("Not toxic");
				}
			}
			ImGui::End();
		}

		/// RESET MENU ///
		if (gameData.beingManager.pool.size() == 0)
		{
			ImGui::Begin("There are no more animals alive. The simulation has ended!");
			state = SolengineV2::GameState::PAUSE;

			if (ImGui::Button("Reset", ImVec2(225, 0)))
			{
				resetGame(state);
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

inline void GUISystem::resetGame(SolengineV2::GameState& state)
{
	state = SolengineV2::GameState::BEGIN;
}