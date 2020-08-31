#pragma once

constexpr float PHYSICS_SPEED_INITIAL{ 0.01f };
constexpr float PHYSICS_SPEED_MAXIMUM{ 2000.0f };
constexpr float PHYSICS_SPEED_MINIMUM{ 0.0f };

constexpr float FOOD_PER_GENERATION_INITIAL{ 0.5f };
constexpr float FOOD_PER_GENERATION_MIN{ 5.0f };
constexpr float FOOD_PER_GENERATION_MAX{ 150.0f };

constexpr float ARENA_DIAMETER_INITIAL{ 0.5f };
constexpr float ARENA_DIAMETER_MIN{ 1000.0f };
constexpr float ARENA_DIAMETER_MAX{ 3000.0f };

constexpr float FOOD_RATE_INITIAL{ 0.5f };
constexpr float FOOD_RATE_MIN{ 0.0f };
constexpr float FOOD_RATE_MAX{ 0.00001f };

constexpr float COMBAT_RATE_INITIAL{ 0.5f };
constexpr float COMBAT_RATE_MIN{ 0.01f };
constexpr float COMBAT_RATE_MAX{ 0.02f };

constexpr float POISON_HEALTH_DAMAGE_INITIAL{ 0.5f };
constexpr float POISON_HEALTH_DAMAGE_MIN{ 0.0f };
constexpr float POISON_HEALTH_DAMAGE_MAX{ 0.002f };

constexpr float TOXIC_DETERIORATION_INITIAL{ 0.5f };
constexpr float TOXIC_DETERIORATION_MIN{ 0.0f };
constexpr float TOXIC_DETERIORATION_MAX{ 400.0f };

constexpr float PLANT_SATURATION_INITIAL{ 0.5f };
constexpr float PLANT_SATURATION_MIN{ 100.0f };
constexpr float PLANT_SATURATION_MAX{ 700.0f };

constexpr float STAMINA_DEPLETION_RATE_INITIAL{ 0.5f };
constexpr float STAMINA_DEPLETION_RATE_MIN{ 0.00000004f };
constexpr float STAMINA_DEPLETION_RATE_MAX{ 0.0000004f };

constexpr float STRENGTH_MULTIPLIER_INITIAL{ 0.5f };
constexpr float STRENGTH_COMBAT_MULTIPLIER_MIN{ 0.0005f };
constexpr float STRENGTH_COMBAT_MULTIPLIER_MAX{ 0.001f };

struct ConfigurableComponent
{
	float val{}; // 0 to 1
	float min{}; // true min
	float max{}; // true max

	float& get0to1() { return val; }
	const float get() const { return min + (val * (max - min)); }
};

struct ConfigurableBoolComponent
{
	bool val{ false };
	bool& get() { return val; }
};

struct ConfigurableSettingsEntity
{
	ConfigurableBoolComponent bounded{ true };
	ConfigurableComponent physicsSpeed{ PHYSICS_SPEED_INITIAL, PHYSICS_SPEED_MINIMUM, PHYSICS_SPEED_MAXIMUM };
	ConfigurableComponent foodPerGeneration{ FOOD_PER_GENERATION_INITIAL, FOOD_PER_GENERATION_MIN, FOOD_PER_GENERATION_MAX };
	ConfigurableComponent arenaDiameter{ ARENA_DIAMETER_INITIAL, ARENA_DIAMETER_MIN, ARENA_DIAMETER_MAX };
	ConfigurableComponent foodRateMultiplier{ FOOD_RATE_INITIAL, FOOD_RATE_MIN, FOOD_RATE_MAX };
	ConfigurableComponent combatRateMultiplier{ COMBAT_RATE_INITIAL, COMBAT_RATE_MIN, COMBAT_RATE_MAX };
	ConfigurableComponent poisonDamageRateMultiplier{ POISON_HEALTH_DAMAGE_INITIAL, POISON_HEALTH_DAMAGE_MIN, POISON_HEALTH_DAMAGE_MAX };
	ConfigurableComponent toxicDeteriorationRateMultiplier{ TOXIC_DETERIORATION_INITIAL, TOXIC_DETERIORATION_MIN, TOXIC_DETERIORATION_MAX };
	ConfigurableComponent plantSaturation{ PLANT_SATURATION_INITIAL, PLANT_SATURATION_MIN, PLANT_SATURATION_MAX };
	ConfigurableComponent staminaDepletionRate{ STAMINA_DEPLETION_RATE_INITIAL, STAMINA_DEPLETION_RATE_MIN, STAMINA_DEPLETION_RATE_MAX };
	ConfigurableComponent strengthCombatMultiplier{ STRENGTH_MULTIPLIER_INITIAL, STRENGTH_COMBAT_MULTIPLIER_MIN, STRENGTH_COMBAT_MULTIPLIER_MAX };
};