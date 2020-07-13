#pragma once

#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "SpriteComponent.h"
#include "GeneComponent.h"
#include "VelocityComponent.h"
#include "SurivalComponent.h"
#include "MemoryPool.h"
#include "BeingCreateInfo.h"

using Colour = SolengineV2::Colour;

class BeingManager
{
public:
	BeingManager(const uint32_t texture) : textureID(texture) {}

	uint32_t textureID{ 0 };

	uint32_t getSize() const;
	void init(const uint32_t size);
	void create(const BeingCreateInfo& createInfo);
	void deleteBeing(const uint32_t& handle, uint32_t& selectedHandle);
	void resize(const uint32_t start);
	void clear();
	void reset();

	const TransformComponent&  getTransformComponent  (const uint32_t handle) const { return pools.transform.data[handle];  }
	const SurvivalComponent&   getSurvivalComponent   (const uint32_t handle) const { return pools.survival.data[handle];   }
	const VelocityComponent&   getVelocityComponent   (const uint32_t handle) const { return pools.velocity.data[handle];   }
	const GeneComponent&       getGeneComponent       (const uint32_t handle) const { return pools.gene.data[handle];       }
	const SpriteComponent&     getSpriteComponent     (const uint32_t handle) const { return pools.sprite.data[handle];     }

	TransformComponent&        getTransformComponent  (const uint32_t handle) { return const_cast<TransformComponent&>(std::as_const(*this).getTransformComponent(handle)); }
	SurvivalComponent&         getSurvivalComponent   (const uint32_t handle) { return const_cast<SurvivalComponent&> (std::as_const(*this).getSurvivalComponent(handle));  }
	VelocityComponent&         getVelocityComponent   (const uint32_t handle) { return const_cast<VelocityComponent&> (std::as_const(*this).getVelocityComponent(handle));  }
	GeneComponent&             getGeneComponent       (const uint32_t handle) { return const_cast<GeneComponent&>     (std::as_const(*this).getGeneComponent(handle));      }
	SpriteComponent&           getSpriteComponent     (const uint32_t handle) { return const_cast<SpriteComponent&>   (std::as_const(*this).getSpriteComponent(handle));    }

private:
	struct
	{
		MemoryPool<TransformComponent>  transform;
		MemoryPool<SpriteComponent>     sprite;
		MemoryPool<VelocityComponent>   velocity;
		MemoryPool<SurvivalComponent>   survival;
		MemoryPool<GeneComponent>       gene;
	} pools;
};