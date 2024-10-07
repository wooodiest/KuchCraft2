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

	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::BeginFrame()
	{

	}

	void Renderer::EndFrame()
	{

	}

}
