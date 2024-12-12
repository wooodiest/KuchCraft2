///
/// @file Renderer.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "Graphics/Renderer.h"
#include "Graphics/Data/ShaderLibrary.h"

#include "Core/Application.h"
#include "Core/Config.h"

#include <glad/glad.h>

#ifdef  INCLUDE_IMGUI
	#include <imgui.h>
	#include <misc/cpp/imgui_stdlib.h>
#endif

namespace KuchCraft {

#pragma region Lifecycle 
	void Renderer::Init()
	{
		/// Check if logging for OpenGL is enabled.
		if (ApplicationConfig::GetRendererData().Logs)
		{
			/// Set up an OpenGL debug message callback function to handle log messages.
			glDebugMessageCallback( [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
					/// Handle the message severity levels accordingly.
					switch (severity)
					{
						case GL_DEBUG_SEVERITY_HIGH:
							Log::Error("[OpenGL] : {0}", (char*)message);
							break;

						case GL_DEBUG_SEVERITY_MEDIUM:
						case GL_DEBUG_SEVERITY_LOW:
							Log::Warn("[OpenGL] : {0}", (char*)message);
							break;

				   /// case GL_DEBUG_SEVERITY_NOTIFICATION:
						/// Log::Info("[OpenGL] : {0}", (char*)message);
						/// break;
					}
				},
				nullptr
			);
			/// Enable OpenGL debug output and ensure that OpenGL debug messages are synchronous.
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}

		/// Adds dynamic substitutions for shaders (constants and configurations)
		AddSubstitutions();

		/// Create uniform buffers
		s_Data.CameraDataUniformBuffer.Create(sizeof(CameraDataUniformBuffer));

		/// Initializes resources
		InitQuads2D();
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::OnEvent(Event& e)
	{

	}

	void Renderer::BeginFrame()
	{
		/// tmp
		auto [width, height] = Application::GetWindow().GetSize();
		glViewport(0, 0, width, height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/// Clear data
		s_Stats.Reset();
		s_Quad2DData.Vertices.clear();
	}

	void Renderer::EndFrame()
	{
		s_Stats.fpsTracker.AddValue(1.0f / Application::GetWindow().GetRawDeltaTime());
	}

	void Renderer::BeginWorld(Camera* camera)
	{
		Camera* currentCamera = camera;

		/// Updates the uniform buffer
		CameraDataUniformBuffer cameraBuffer;
		cameraBuffer.ViewProjection  = currentCamera->GetViewProjection();
		cameraBuffer.OrthoProjection = glm::ortho(0.0f, (float)Application::GetWindow().GetWidth(), 0.0f , (float)Application::GetWindow().GetHeigth());
		s_Data.CameraDataUniformBuffer.SetData(&cameraBuffer, sizeof(cameraBuffer));
	}

	void Renderer::EndWorld()
	{
		EnableDepthTesting();
		RenderQuads2D();
	}

	void Renderer::OnImGuiRender()
	{
#ifdef  INCLUDE_IMGUI
		
		if (ImGui::CollapsingHeader("Statistics##Renderer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			s_Stats.fpsTracker      .RenderImGui("Fps");
			s_Stats.drawCallsTracker.RenderImGui("Draw calls");
			s_Stats.verticesTracker .RenderImGui("Vertices");
		}

		if (ImGui::CollapsingHeader("Shaders", ImGuiTreeNodeFlags_DefaultOpen) && !s_Data.ShaderLibrary.GetShaders().empty())
		{
			if (ImGui::Button("Recompile all", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
				ReCompileShaders();

			constexpr float shader_list_height = 150.0f;
			static int selected = -1;
			static std::string selectedShader = "NONE";
	
			ImGui::SeparatorText("ShaderList");
			ImGui::BeginChild("ShaderList", ImVec2(0.0f, shader_list_height), true);

			int index = 0;
			for (const auto& [name, shader] : s_Data.ShaderLibrary.GetShaders())
			{
				bool isSelected = (selected == index);
				if (ImGui::Selectable(name.c_str(), isSelected))
				{
					selected = index;
					selectedShader = name;
				}
				
				if (isSelected) 
					ImGui::SetItemDefaultFocus();

				index++;
			}

			ImGui::EndChild();

			/// Displays details of the selected shader.
			if (selected != -1)
			{
				ImGui::SeparatorText("Shader info");

				auto shader = s_Data.ShaderLibrary.Get(selectedShader);
				ImGui::Text("Name: %s", shader->GetName());
				ImGui::Text("Path: %s", shader->GetPath().string().c_str());
				ImGui::Text("RendererID: %d", shader->GetRendererID());

				if (ImGui::Button("Recompile", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					ReCompileShader(shader->GetName());

				ImGui::SeparatorText("Uniforms");

				enum class UniformType { Int, Float, Float2, Float3, Float4, Mat3, Mat4, Color3, Color4 };
				static std::string uniformName;
				static UniformType selectedType = UniformType::Color4;
				static int intVal               = 0;
				static float floatVal           = 0.0f;
				static glm::vec2 float2Val      = glm::vec2(0.0f);
				static glm::vec3 float3Val      = glm::vec3(0.0f);
				static glm::vec4 float4Val      = glm::vec4(0.0f);
				static glm::mat3 mat3Val        = glm::mat3(1.0f);
				static glm::mat4 mat4Val        = glm::mat4(1.0f);
				static glm::vec3 color3Val		= glm::vec3(1.0f);
				static glm::vec4 color4Val		= glm::vec4(1.0f);

				auto UniformTypeToString = [](UniformType type) -> std::string {
					switch (type)
					{
							case UniformType::Int:    return "Int";	
							case UniformType::Float:  return "Float";
							case UniformType::Float2: return "Float2";
							case UniformType::Float3: return "Float3";
							case UniformType::Float4: return "Float4";
							case UniformType::Mat3:   return "Mat3";
							case UniformType::Mat4:   return "Mat4";
							case UniformType::Color3: return "Color3";
							case UniformType::Color4: return "Color4";
					}
					return "Unknown";
				};

				ImGui::InputText("Uniform Name", &uniformName);
				if (ImGui::BeginCombo("Type", UniformTypeToString(selectedType).c_str()))
				{
					for (int i = 0; i <= static_cast<int>(UniformType::Color4); i++)
					{
						UniformType type = static_cast<UniformType>(i);
						bool isSelected = (selectedType == type);
						if (ImGui::Selectable(UniformTypeToString(type).c_str(), isSelected))
							selectedType = type;

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				switch (selectedType)
				{
				case UniformType::Int:
					ImGui::InputInt("Value", &intVal);
					if (ImGui::Button("Set Uniform", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					{
						shader->Bind();
						shader->SetInt(uniformName, intVal);
					}
					break;

				case UniformType::Float:
					ImGui::InputFloat("Value", &floatVal);
					if (ImGui::Button("Set Uniform", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					{
						shader->Bind();
						shader->SetFloat(uniformName, floatVal);
					}
					break;

				case UniformType::Float2:
					ImGui::InputFloat2("Value", glm::value_ptr(float2Val));
					if (ImGui::Button("Set Uniform", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					{
						shader->Bind();
						shader->SetFloat2(uniformName, float2Val);
					}
					break;

				case UniformType::Float3:
					ImGui::InputFloat3("Value", glm::value_ptr(float3Val));
					if (ImGui::Button("Set Uniform", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					{
						shader->Bind();
						shader->SetFloat3(uniformName, float3Val);
					}
					break;

				case UniformType::Float4:
					ImGui::InputFloat4("Value", glm::value_ptr(float4Val));
					if (ImGui::Button("Set Uniform", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					{
						shader->Bind();
						shader->SetFloat4(uniformName, float4Val);
					}
					break;

				case UniformType::Mat3:
					for (int i = 0; i < 3; i++)
						ImGui::InputFloat3(("Row " + std::to_string(i)).c_str(), &mat3Val[i][0]);
					if (ImGui::Button("Set Uniform", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					{
						shader->Bind();
						shader->SetMat3(uniformName, mat3Val);
					}
					break;

				case UniformType::Mat4:
					for (int i = 0; i < 4; i++)
						ImGui::InputFloat4(("Row " + std::to_string(i)).c_str(), &mat4Val[i][0]);
					if (ImGui::Button("Set Uniform", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					{
						shader->Bind();
						shader->SetMat4(uniformName, mat4Val);
					}
					break;

				case UniformType::Color3:
					ImGui::ColorEdit3("Color", glm::value_ptr(color3Val));
					if (ImGui::Button("Set Uniform", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					{
						shader->Bind();
						shader->SetFloat3(uniformName, color3Val);
					}
					break;

				case UniformType::Color4:
					ImGui::ColorEdit4("Color", glm::value_ptr(color4Val));
					if (ImGui::Button("Set Uniform", ImVec2(ImGui::GetContentRegionAvail().x, 0.0f)))
					{
						shader->Bind();
						shader->SetFloat4(uniformName, color4Val);
					}
					break;
				}
			}
			
		}

		if (ImGui::CollapsingHeader("Shader substitutions", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::BeginTable("TwoColumnTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
			{
				ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableSetupColumn("Vaule", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();
				for (const auto& [key, value] : s_Data.ShaderLibrary.GetSubstitutionMap())
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::TextUnformatted(key.substr(2).c_str());
					ImGui::TableSetColumnIndex(1);
					ImGui::TextUnformatted(value.c_str());
				}
				ImGui::EndTable();
			}
		}
	
#endif
	}

#pragma endregion 
#pragma region DrawCommands

	void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		for (uint32_t i = 0; i < quad_vertex_count; i++)
		{
			s_Quad2DData.Vertices.emplace_back(
				transform * quad2D_vertex_positions[i],
				color,
				quad2D_vertex_texture_coords[i],
				0.0f
			);
		}
	}

	void Renderer::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture>& texture, const glm::vec4& tint)
	{
		float textureID = static_cast<float>(texture->GetRendererID());
		for (uint32_t i = 0; i < quad_vertex_count; i++)
		{
			s_Quad2DData.Vertices.emplace_back(
				transform * quad2D_vertex_positions[i],
				tint,
				quad2D_vertex_texture_coords[i],
				textureID /// Texture index temporarily holds the texture rendererID
			);
		}
	}

#pragma endregion
#pragma region Shaders

	void Renderer::ReCompileShaders()
	{
		AddSubstitutions();
		s_Data.ShaderLibrary.ReCompileAll();

		// If needed set appropriete uniforms for shaders
	}

	void Renderer::ReCompileShader(const std::string& name)
	{
		AddSubstitutions();
		s_Data.ShaderLibrary.ReCompile(name);
	}

	void Renderer::AddSubstitutions()
	{
		s_Data.ShaderLibrary.AddSubstitution(std::make_pair("SHADER_VERSION", ApplicationConfig::GetRendererData().ShaderVersion));
		s_Data.ShaderLibrary.AddSubstitution(std::make_pair("UNIFORM_CAMERA_DATA_BINDING", std::to_string(s_Data.CameraDataUniformBuffer.GetBinding())));
		s_Data.ShaderLibrary.AddSubstitution(std::make_pair("MAX_TEXTURES_SLOTS", std::to_string(ApplicationConfig::GetRendererData().MaxTextureSlots)));
	}

#pragma endregion
#pragma region Quads2D

	void Renderer::InitQuads2D()
	{
		s_Quad2DData.MaxQuads    = ApplicationConfig::GetRendererData().Renderer2DMaxQuads;
		s_Quad2DData.MaxVertices = s_Quad2DData.MaxQuads * quad_vertex_count;
		s_Quad2DData.MaxIndices  = s_Quad2DData.MaxQuads * quad_index_count;

		s_Quad2DData.VertexArray .Create();
		s_Quad2DData.VertexBuffer.Create(VertexBufferDataUsage::DYNAMIC, s_Quad2DData.MaxVertices * sizeof(Primitives::_2D::QuadVertex));
		s_Quad2DData.VertexBuffer.SetBufferLayout({
			{ ShaderDataType::Float3, "a_Position"      },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float2, "a_TextureCoord"  },
			{ ShaderDataType::Float,  "a_TextureIndex"  }
		});
		s_Quad2DData.VertexArray.SetVertexBuffer(s_Quad2DData.VertexBuffer);

		uint32_t* indices = new uint32_t[s_Quad2DData.MaxIndices];
		uint32_t  offset = 0;
		for (uint32_t i = 0; i < s_Quad2DData.MaxIndices; i += quad_index_count)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += quad_vertex_count;
		}
		s_Quad2DData.IndexBuffer.Create(IndexBufferDataUsage::STATIC, s_Quad2DData.MaxIndices, indices);
		delete[] indices;
		s_Quad2DData.Shader = s_Data.ShaderLibrary.Load("assets/shaders/quad2D.glsl");
		s_Quad2DData.Shader->Bind();

		TextureSpecification whiteTextureSpec;
		whiteTextureSpec.Width  = 1;
		whiteTextureSpec.Height = 1;
		s_Quad2DData.WhiteTexture = std::make_shared<Texture2D>(whiteTextureSpec);
		uint32_t whiteColor = 0xffffffff;
		s_Quad2DData.WhiteTexture->SetData(&whiteColor, sizeof(whiteColor));

		int* samplers = new int[ApplicationConfig::GetRendererData().MaxTextureSlots];
		for (int i = 0; i < ApplicationConfig::GetRendererData().MaxTextureSlots; i++)
			samplers[i] = i;
		s_Quad2DData.Shader->SetIntArray("u_Textures", samplers, ApplicationConfig::GetRendererData().MaxTextureSlots);
		delete[] samplers;

		s_Quad2DData.TextureSlots = new uint32_t[ApplicationConfig::GetRendererData().MaxTextureSlots];
		s_Quad2DData.TextureSlots[0] = s_Quad2DData.WhiteTexture->GetRendererID();

		s_Quad2DData.Vertices.reserve(s_Quad2DData.MaxVertices);

		s_Quad2DData.Shader     ->Unbind();
		s_Quad2DData.VertexArray .Unbind();
		s_Quad2DData.IndexBuffer .Unbind();
		s_Quad2DData.VertexBuffer.Unbind();
	}

	void Renderer::StartQuadsBatch2D()
	{
		s_Quad2DData.IndexCount       = 0;
		s_Quad2DData.TextureSlotIndex = 1;
	}

	void Renderer::NextQuadsBatch2D()
	{
		FlushQuads2D();
		StartQuadsBatch2D();
	}

	void Renderer::RenderQuads2D()
	{
		if (!s_Quad2DData.Vertices.size())
			return;

		s_Quad2DData.Shader     ->Bind();
		s_Quad2DData.VertexArray .Bind();
		s_Quad2DData.VertexBuffer.Bind();
		s_Quad2DData.IndexBuffer .Bind();
		s_Quad2DData.VertexOffset = 0;

		StartQuadsBatch2D();
		for (uint32_t i = 0; i < s_Quad2DData.Vertices.size(); i += quad_vertex_count)
		{
			if (s_Quad2DData.IndexCount == s_Quad2DData.MaxIndices)
				NextQuadsBatch2D();

			if (s_Quad2DData.Vertices[i].TextureIndex == 0.0f)
			{
				s_Quad2DData.Vertices[i + 0].TextureIndex = 0.0f;
				s_Quad2DData.Vertices[i + 1].TextureIndex = 0.0f;
				s_Quad2DData.Vertices[i + 2].TextureIndex = 0.0f;
				s_Quad2DData.Vertices[i + 3].TextureIndex = 0.0f;

				s_Quad2DData.IndexCount += quad_index_count;
			}
			else
			{
				float textureIndex = 0.0f;
				for (uint32_t j = 1; j < s_Quad2DData.TextureSlotIndex; j++)
				{
					if (s_Quad2DData.TextureSlots[j] == s_Quad2DData.Vertices[i].TextureIndex) // TexIndex temporarily holds the texture rendererID
					{
						textureIndex = (float)j;
						break;
					}
				}

				if (textureIndex == 0.0f)
				{
					if (s_Quad2DData.TextureSlotIndex >= ApplicationConfig::GetRendererData().MaxTextureSlots)
						NextQuadsBatch2D();

					textureIndex = static_cast<float>(s_Quad2DData.TextureSlotIndex);
					s_Quad2DData.TextureSlots[s_Quad2DData.TextureSlotIndex] = s_Quad2DData.Vertices[i].TextureIndex;
					s_Quad2DData.TextureSlotIndex++;
				}

				s_Quad2DData.Vertices[i + 0].TextureIndex = textureIndex;
				s_Quad2DData.Vertices[i + 1].TextureIndex = textureIndex;
				s_Quad2DData.Vertices[i + 2].TextureIndex = textureIndex;
				s_Quad2DData.Vertices[i + 3].TextureIndex = textureIndex;

				s_Quad2DData.IndexCount += quad_index_count;
			}
		}

		FlushQuads2D();
	}

	void Renderer::FlushQuads2D()
	{
		if (s_Quad2DData.IndexCount == 0)
			return;

		uint32_t vertexCount = s_Quad2DData.IndexCount / quad_index_count * quad_vertex_count;
		s_Quad2DData.VertexBuffer.SetData(vertexCount * sizeof(Primitives::_2D::QuadVertex), &s_Quad2DData.Vertices[s_Quad2DData.VertexOffset]);
		s_Quad2DData.VertexOffset += vertexCount;

		for (uint32_t i = 0; i < s_Quad2DData.TextureSlotIndex; i++)
			Texture::Bind(s_Quad2DData.TextureSlots[i], i);

		DrawElemnts(s_Quad2DData.IndexCount);

		s_Stats.DrawCalls++;
		s_Stats.Vertices += vertexCount;
	}

#pragma endregion
#pragma region RendererCommands

	void Renderer::DrawElemnts(uint32_t count)
	{
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawArrays(uint32_t count, uint32_t offset)
	{
		glDrawArrays(GL_TRIANGLES, offset, count);
	}

	void Renderer::DrawStripArraysInstanced(uint32_t count, uint32_t instanceCount, uint32_t offset)
	{
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, offset, count, instanceCount);
	}

	void Renderer::EnableBlending()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::DisableBlending()
	{
		glDisable(GL_BLEND);
	}

	void Renderer::EnableFaceCulling()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	void Renderer::EnableFrontFaceCulling()
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}

	void Renderer::DisableFaceCulling()
	{
		glDisable(GL_CULL_FACE);
	}

	void Renderer::EnableDepthTesting()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	void Renderer::EnableLessEqualDepthTesting()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	void Renderer::DisableDepthTesting()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void Renderer::EnableDepthMask()
	{
		glDepthMask(GL_TRUE);
	}

	void Renderer::DisableDepthMask()
	{
		glDepthMask(GL_FALSE);
	}

	void Renderer::EnablePolygonOffset(float factor, float units)
	{
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(factor, units);
	}

	void Renderer::DisablePolygonOffset()
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

#pragma endregion

}
