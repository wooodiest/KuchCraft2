///
/// @file Renderer.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "Graphics/Renderer.h"
#include "Graphics/Data/ShaderLibrary.h"

#include "Core/Config.h"

#include <glad/glad.h>

namespace KuchCraft {

	void Renderer::Init()
	{
		/// Retrieve the renderer configuration data from the application config.
		auto& config = ApplicationConfig::GetRendererData();
		s_Data.Logs = config.Logs;

		/// Check if logging for OpenGL is enabled.
		if (s_Data.Logs)
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

						case GL_DEBUG_SEVERITY_NOTIFICATION:
							Log::Info("[OpenGL] : {0}", (char*)message);
							break;
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

			s_TMPData.Shader = s_Data.ShaderLibrary.Load("assets/shaders/tmp.glsl");
			s_TMPData.Shader->Bind();
			s_TMPData.Shader->SetFloat4("u_Color", { 1.0f, 0.0f, 0.0f, 1.0f });
		}
		
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::BeginFrame()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::EndFrame()
	{
		s_TMPData.Shader->Bind();
		s_TMPData.VertexArray.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void Renderer::ReCompileShaders()
	{
		AddSubstitutions();
		s_Data.ShaderLibrary.ReCompileAll();
	}

	void Renderer::AddSubstitutions()
	{
		auto tmp = std::make_pair<std::string, std::string>("TMP", "1.0");
		s_Data.ShaderLibrary.AddSubstitution(tmp);
	}

}
