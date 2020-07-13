#include <IMGUI/imgui_impl_sdl.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <IMGUI/implot.h>
#include <Window.h>

#include "GenerationSystem.h"
#include "GeneSystem.h"
#include "SelectableSystem.h"
#include "VelocitySystem.h"
#include "DataSystem.h"
#include "SurvivalSystem.h"
#include "GUISystem.h"
#include "BeingManager.h"

constexpr const char* const WAVE_NUMBER_TEXT{ "Wave number: %d" };
constexpr const char* PHYSICS_SPEED_TEXT{ "Physics speed" };
constexpr const char* FOOD_PER_GENERATION_TEXT{ "Food per generation" };
constexpr const char* BEING_SPEED_TEXT{ "Being speed: %f" };
constexpr const char* BEING_DIET_TEXT{ "Being diet: %f" };
constexpr const char* BEING_STRENGTH_TEXT{ "Being strength: %f" };
constexpr const char* BEING_STAMINA_TEXT{ "Being stamina: %f" };
constexpr const char* BEING_ENERGY_TEXT{ "Being energy: %i" };
constexpr const char* BEING_FULLNESS_TEXT{ "Being fullness: %i" };
constexpr const char* PLANT_COUNT_TEXT{ "Plants: %i" };
constexpr const char* ANIMAL_COUNT_TEXT{ "Animals: %i" };

constexpr const int FRAMES_PER_UPDATE{ 15 };

using Data = DataSystem::Data;
using BeingType = GeneComponent::BeingType;
using Trait = GeneComponent::Trait;

GUISystem::GUISystem(
	SolengineV2::Window& wndw,
	VelocitySystem& velSys,
	GenerationSystem& genSys,
	DataSystem& dataSys,
	const GeneSystem& geneSys,
	const SelectableSystem& selSys,
	const SurvivalSystem& survSys
) :
	window(wndw),
	velocitySystem(velSys),
	generationSystem(genSys),
	dataSystem(dataSys),
	geneSystem(geneSys),
	selectableSystem(selSys),
	survivalSystem(survSys)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getContext());
	const char* glslVersion = "#version 130";
	ImGui_ImplOpenGL3_Init(glslVersion);
}

void GUISystem::process(BeingManager& beings, SolengineV2::GameState& state)
{
	static uint32_t frameCount = 0;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window.getWindow());

	if (frameCount++ % FRAMES_PER_UPDATE == 0)
	{
		ImGui::NewFrame();

		//bool f = true;
		//ImPlot::ShowDemoWindow(&f);
		{
			if (ImGui::Begin("Plots"))
			{
				int size = dataSystem.getData(Data::SPEED).size();
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
		}

		{
			ImGui::Begin("Simulation Information");
			ImGui::SliderFloat(PHYSICS_SPEED_TEXT, &velocitySystem.getPhysicsSpeed(), 1, 1000);
			ImGui::SliderInt(FOOD_PER_GENERATION_TEXT, &generationSystem.getFoodPerGeneration(), 1, 150);
			ImGui::Text(WAVE_NUMBER_TEXT, generationSystem.getWaveCount());
			ImGui::Text(ANIMAL_COUNT_TEXT, generationSystem.getAnimalCount());
			ImGui::Text(PLANT_COUNT_TEXT, generationSystem.getPlantCount());
			if (ImGui::Button("Reset", ImVec2(225, 0)))
			{
				dataSystem.reset();
				generationSystem.begin(beings);
			}
			ImGui::End();

		}

		{
			ImGui::Begin("Selected Entity Panel");
			const uint32_t selectedHandle = selectableSystem.getCSelectedHandle();
			if (selectedHandle < beings.getSize())
			{
				const GeneComponent& gene = beings.getGeneComponent(selectedHandle);
				ImGui::Text(geneSystem.getBeingType(gene) == BeingType::ANIMAL ? "Animal" : "Plant");
				if (geneSystem.getBeingType(gene) == BeingType::ANIMAL)
				{
					ImGui::Text(BEING_SPEED_TEXT, geneSystem.getTrait(gene, Trait::SPEED));
					ImGui::Text(BEING_DIET_TEXT, geneSystem.getTrait(gene, Trait::DIET));
					ImGui::Text(BEING_STRENGTH_TEXT, geneSystem.getTrait(gene, Trait::STRENGTH));
					ImGui::Text(BEING_STAMINA_TEXT, geneSystem.getTrait(gene, Trait::STAMINA));
					ImGui::Text(BEING_FULLNESS_TEXT, static_cast<int>(0.1f * survivalSystem.getBeingFullness(beings.getSurvivalComponent(selectedHandle)) / geneSystem.getTrait(gene, Trait::HUNGER)));
					ImGui::Text(BEING_ENERGY_TEXT, static_cast<int>(0.1f * survivalSystem.getBeingEnergy(beings.getSurvivalComponent(selectedHandle)) / geneSystem.getTrait(gene, Trait::STAMINA)));
				}
			}
			ImGui::End();
		}

		if (generationSystem.getAnimalCount() == 0)
		{
			ImGui::Begin("There are no more animals alive. The simulation has ended!");
			state = SolengineV2::GameState::PAUSE;

			if (ImGui::Button("Reset", ImVec2(225, 0)))
			{
				state = SolengineV2::GameState::PLAY;
				dataSystem.reset();
				generationSystem.begin(beings);
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
