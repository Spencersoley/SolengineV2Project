#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <algorithm>

#include "Glyph.h"
#include "Vertex.h"

namespace SolengineV2
{
	// Determines how we should sort the glyphs
	enum class GlyphSortType { NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE };

	// Each render batch is used for a single draw call
	class RenderBatch
	{
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) :
			offset(Offset),
			numVertices(NumVertices),
			texture(Texture) {}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	// The SpriteBatch class is a more efficient way of drawing sprites
	class SpriteBatch
	{
	public:
		SpriteBatch() : vbo(0), vao(0)
		{
			createVertexArray();
		};

		void Dispose()
		{
			if (vao != 0)
			{
				glDeleteVertexArrays(1, &vao);
				vao = 0;
			}

			if (vbo != 0)
			{
				glDeleteBuffers(1, &vbo);
				vbo = 0;
			}
		}

		// Begins the spritebatch
		void Begin()
		{
			renderBatches.clear();

			// Makes glpyhs.size() == 0, however it does not free internal memory.
			// So when we later call emplace_back it doesn't need to internally call new.
			glyphs.clear();
		}

		// Adds a glyph to the spritebatch
		void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Colour colour)
		{
			glyphs.emplace_back(destRect, uvRect, texture, depth, colour);
		}
		// Adds a glyph to the spritebatch with angle rotation
		void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Colour colour, float angle)
		{
			glyphs.emplace_back(destRect, uvRect, texture, depth, colour, angle);
		}
		// Adds a glyph to the spritebatch with direction rotation
		void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Colour colour, const glm::vec2& dir)
		{
			const glm::vec2 right(1.0f, 0.0f);
			float angle = acos(glm::dot(right, dir));
			if (dir.y < 0.0f) angle = -angle;

			glyphs.emplace_back(destRect, uvRect, texture, depth, colour, angle);
		}
	
		// Ends the spritebatch
		void End()
		{
			// Set up all pointers for fast sorting
			glyphPointers.resize(glyphs.size());
			for (size_t i = 0; i < glyphs.size(); i++)
			{
				glyphPointers[i] = &glyphs[i];
			}
			createRenderBatches();
		}

		// Renders the entire SpriteBatch to the screen
		void RenderSpriteBatch()
		{
			// Bind our VAO. This sets up the opengl state we need, including the 
			// vertex attribute pointers and it binds the VBO
			glBindVertexArray(vao);

			for (size_t i = 0; i < renderBatches.size(); i++) 
			{
				glBindTexture(GL_TEXTURE_2D, renderBatches[i].texture);
				glDrawArrays(GL_TRIANGLES, renderBatches[i].offset, renderBatches[i].numVertices);
			}

			glBindVertexArray(0);
		}
	private:
		GLuint vbo;
		GLuint vao;

		std::vector<Glyph*> glyphPointers; ///< This is for sorting
		std::vector<Glyph> glyphs; ///< These are the actual glyphs
		std::vector<RenderBatch> renderBatches;

		// Creates all the needed RenderBatches
		void createRenderBatches()
		{
			// This will store all the vertices that we need to upload
			std::vector <Vertex> vertices;
			// Resize the buffer to the exact size we need so we can treat
			// it like an array
			vertices.resize(glyphPointers.size() * 6);

			if (glyphPointers.empty()) 
			{
				return;
			}

			int offset = 0; // current offset
			int cv = 0; // current vertex

			//Add the first batch
			renderBatches.emplace_back(offset, 6, glyphPointers[0]->Texture);
			vertices[cv++] = glyphPointers[0]->TopLeft;
			vertices[cv++] = glyphPointers[0]->BottomLeft;
			vertices[cv++] = glyphPointers[0]->BottomRight;
			vertices[cv++] = glyphPointers[0]->BottomRight;
			vertices[cv++] = glyphPointers[0]->TopRight;
			vertices[cv++] = glyphPointers[0]->TopLeft;
			offset += 6;

			//Add all the rest of the glyphs
			for (size_t cg = 1; cg < glyphPointers.size(); cg++)
			{
				// Check if this glyph can be part of the current batch
				if (glyphPointers[cg]->Texture != glyphPointers[cg - 1]->Texture)
				{
					// Make a new batch
					renderBatches.emplace_back(offset, 6, glyphPointers[cg]->Texture);
				}
				else 
				{
					// If its part of the current batch, just increase numVertices
					renderBatches.back().numVertices += 6;
				}
				vertices[cv++] = glyphPointers[cg]->TopLeft;
				vertices[cv++] = glyphPointers[cg]->BottomLeft;
				vertices[cv++] = glyphPointers[cg]->BottomRight;
				vertices[cv++] = glyphPointers[cg]->BottomRight;
				vertices[cv++] = glyphPointers[cg]->TopRight;
				vertices[cv++] = glyphPointers[cg]->TopLeft;
				offset += 6;
			}
			// Bind our VBO
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			// Orphan the buffer (for speed)
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
			// Upload the data
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

			// Unbind the VBO
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		// Generates our VAO and VBO
		void createVertexArray()
		{
			// Generate the VAO if it isn't already generated
			if (vao == 0)
			{
				glGenVertexArrays(1, &vao);
			}
			// Bind the VAO. All subsequent opengl calls will modify it's state.
			glBindVertexArray(vao);
			//G enerate the VBO if it isn't already generated
			if (vbo == 0)
			{
				glGenBuffers(1, &vbo);
			}
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			//Tell opengl what attribute arrays we need
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			//This is the position attribute pointer
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Pos));
			//This is the color attribute pointer
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Col));
			//This is the UV attribute pointer
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
			glBindVertexArray(0);
		}
	};
}