///
/// @file Renderer.cpp
/// 
/// @author Michal Kuchnicki
/// 

#include "kcpch.h"
#include "Graphics/Renderer.h"

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

		/// tmp
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f, 
			 0.0f,  0.5f, 0.0f
		};

		unsigned int VBO;
		glGenVertexArrays(1, &s_TMPData.VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(s_TMPData.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		s_TMPData.Shader = std::make_shared<Shader>("assets/shaders/tmp.glsl");
		s_TMPData.Shader->Bind();
		s_TMPData.Shader->SetFloat4("u_Color", { 1.0f, 0.0f, 0.0f, 1.0f });
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
		glBindVertexArray(s_TMPData.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

}
