#pragma once
#include <glm/glm.hpp>

#include "Vertex.h"

namespace SolengineV2
{
	// A glyph is a single quad. These are added via SpriteBatch::draw
	class Glyph
	{
	public:
		Glyph() :
			Depth(0) {};
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Colour& colour) :
			Texture(Texture),
			Depth(Depth)
		{
			TopLeft.Col = colour;
			//TopLeft.SetColour(colour);
			TopLeft.SetPosition(destRect.x, destRect.y + destRect.w);
			TopLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

			BottomLeft.Col = colour;
			//BottomLeft.SetColour(colour);
			BottomLeft.SetPosition(destRect.x, destRect.y);
			BottomLeft.SetUV(uvRect.x, uvRect.y);

			BottomRight.Col = colour;
			//BottomRight.SetColour(colour);
			BottomRight.SetPosition(destRect.x + destRect.z, destRect.y);
			BottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

			TopRight.Col = colour;
			//TopRight.SetColour(colour);
			TopRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
			TopRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		}
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Colour& colour, float angle) :
			Texture(Texture),
			Depth(Depth)
		{
			glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

			// Get points centered at origin
			glm::vec2 tl(-halfDims.x, halfDims.y);
			glm::vec2 bl(-halfDims.x, -halfDims.y);
			glm::vec2 br(halfDims.x, -halfDims.y);
			glm::vec2 tr(halfDims.x, halfDims.y);

			// Rotate the points
			tl = rotatePoint(tl, angle) + halfDims;
			bl = rotatePoint(bl, angle) + halfDims;
			br = rotatePoint(br, angle) + halfDims;
			tr = rotatePoint(tr, angle) + halfDims;

			TopLeft.SetColour(colour);
			TopLeft.SetPosition(destRect.x + tl.x, destRect.y + tl.y);
			TopLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

			BottomLeft.SetColour(colour);
			BottomLeft.SetPosition(destRect.x + bl.x, destRect.y + bl.y);
			BottomLeft.SetUV(uvRect.x, uvRect.y);

			BottomRight.SetColour(colour);
			BottomRight.SetPosition(destRect.x + br.x, destRect.y + br.y);
			BottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

			TopRight.SetColour(colour);
			TopRight.SetPosition(destRect.x + tr.x, destRect.y + tr.y);
			TopRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		};

		GLuint Texture;
		float Depth;

		Vertex TopLeft;
		Vertex BottomLeft;
		Vertex TopRight;
		Vertex BottomRight;
	private:
		// Rotates a point about (0,0) by angle
		glm::vec2 rotatePoint(const glm::vec2& pos, float angle)
		{
			return
			{
				pos.x * cos(angle) - pos.y * sin(angle),
				pos.x * sin(angle) + pos.y * cos(angle)
			};
		}
	};
}