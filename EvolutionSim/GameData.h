#pragma once
#include "Manager.h"
#include "DataPointEntity.h"
#include "GenerationEntity.h"
#include "OverlayConfigurationEntity.h"
#include "SelectionBoxEntity.h"
#include "BackgroundImageEntity.h"
#include "SelectedTrackerEntity.h"
#include "SpatialPartitionEntity.h"
#include "CameraEntity.h"
#include "TextureLibraryEntity.h"
#include "BeingEntity.h"
#include "FoodEntity.h"
#include "ConfigurableSettingsEntity.h"

constexpr int CELL_SIZE{ 40 };

//contains all the entities in our simulation
struct GameData
{
	Manager<BeingEntity>          beingManager;
	Manager<FoodEntity>           foodManager;
	DataPointEntity               dataPoint;
	GenerationEntity              generation;
	OverlayConfigurationEntity    overlayConfiguration;
	SelectionBoxEntity            selectionBox;
	BackgroundImageEntity         backgroundImage;
	SelectedTrackerEntity         selectedTracker;
	SpatialPartitionEntity        spatialPartition;
	CameraEntity                  camera;
	TextureLibraryEntity          textureLibrary;
	ConfigurableSettingsEntity    configurableSettings;
};