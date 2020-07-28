#pragma once

class BeingManager;
class GeneSystem;
class SpriteSystem;
class SurvivalSystem;

class OverlaySystem
{
public:
	OverlaySystem(
		SpriteSystem& spriteSys,
		const GeneSystem& geneSys,
		const SurvivalSystem& survSys
	) :
		spriteSystem(spriteSys),
		geneSystem(geneSys),
		survivalSystem(survSys)
	{}

	enum class Overlay { DEFAULT, STRENGTH, SPEED, STAMINA, DIET, HUNGER  };

	void setOverlay(Overlay overlay);
	void updateOverlay(BeingManager& beings) const;

	Overlay getOverlay() const;

private:
	Overlay currentOverlay{ Overlay::DEFAULT };

	SpriteSystem& spriteSystem;
	const GeneSystem& geneSystem;
	const SurvivalSystem& survivalSystem;
};

