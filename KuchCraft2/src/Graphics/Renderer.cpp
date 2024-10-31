///
/// @file Renderer.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "Graphics/Renderer.h"
#include "Graphics/Data/ShaderLibrary.h"

#include "Core/Config.h"
#include "Core/Config.h"

#include <glad/glad.h>

#ifdef  INCLUDE_IMGUI
	#include <imgui.h>
	#include <misc/cpp/imgui_stdlib.h>
#endif

namespace KuchCraft {

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

		/// Add custom shader substitutions
		AddSubstitutions();

		/// tmp
		{
			float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};

			s_TMPData.VertexArray.Create();
			s_TMPData.VertexBuffer.Create(VertexBufferDataUsage::STATIC, sizeof(vertices), vertices);
			s_TMPData.VertexBuffer.SetBufferLayout({
				{ ShaderDataType::Float3, "aPos" }
			});
			s_TMPData.VertexArray.SetVertexBuffer(s_TMPData.VertexBuffer);

			s_TMPData.Shader1 = s_Data.ShaderLibrary.Load("assets/shaders/tmp.glsl");
			s_TMPData.Shader2 = s_Data.ShaderLibrary.Load("assets/shaders/tmp2.glsl");

			s_TMPData.Camera.SetPosition({ 0.0f, 0.0f, 4.0f });
		}
		
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::BeginFrame()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		s_TMPData.Shader1->Bind();
		s_TMPData.Shader1->SetMat4("u_ViewProjection", s_TMPData.Camera.GetViewProjection());
	}

	void Renderer::EndFrame()
	{
		s_TMPData.Shader1->Bind();
		s_TMPData.VertexArray.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void Renderer::OnImGuiRender()
	{
#ifdef  INCLUDE_IMGUI
		
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

		glm::vec3 pos = s_TMPData.Camera.GetPosition();
		glm::vec3 rot = glm::degrees(s_TMPData.Camera.GetRotation());

		if (ImGui::SliderFloat3("Camera position##1", glm::value_ptr(pos), - 10.0f, 10.0f))
			s_TMPData.Camera.SetPosition(pos);

		if (ImGui::SliderFloat3("Camera rotation##2", glm::value_ptr(rot), -180.0f, 180.0f))
			s_TMPData.Camera.SetRotation(glm::radians(rot));
		
#endif
	}

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
	}

}
