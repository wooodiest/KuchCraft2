///
/// @file Shader.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the Shader class.
/// 
/// @details This file defines the Shader class, which is responsible for managing
///          OpenGL shaders within the rendering context. It provides functionality
///          to load, compile, and link vertex and fragment shaders from source code
///          files, enabling developers to create complex graphical effects.
///          It allows for setting uniform variables for shaders and it supports hot-reloading
///          of shaders during runtime, allowing developers to make changes to shader code without 
///          restarting the application.
/// 
///          The shader source code is expected to be structured with specific 
///          preprocessor directives that separate vertex and fragment shader sections.
///          The Shader class handles reading the source file, preprocessing it, and 
///          compiling the shaders, ensuring that they are ready for use in rendering.
/// 

#pragma once

namespace KuchCraft {

	/// Enum representing the type of shader.
	enum class ShaderType
	{
		NONE     = 0,
		VERTEX   = 1,
		FRAGMENT = 2,
	};

	class Shader
	{
	public:
		/// Constructs a Shader object from a file containing the shader source code.
		/// The shader is loaded, preprocessed, compiled, and linked into a shader program.
		/// 
		/// @param filepath The path to the shader source code file. This file is expected
		///                 to contain both vertex and fragment shader code, separated by custom
		///                 preprocessor directives.
		Shader(const std::string& filepath);

		/// Destructor that deletes the shader program from the GPU memory.
		/// Ensures that all resources associated with the shader program are released
		/// when the Shader object is destroyed.
		~Shader();

		/// Binds the shader program, making it the active shader in the OpenGL context.
		/// This must be called before issuing any draw calls that use this shader.
		void Bind() const;

		/// Unbinds the current shader program, effectively deactivating it.
		void Unbind() const;

        /// Recompiles the shader program. This method will reload the shader source file,
        /// preprocess the code, and recompile the shader. It is useful for hot-reloading 
        /// shaders during runtime when you want to update them without restarting the application.
		void ReCompile();

		/// Returns the name of the shader
		/// This name is used for identification purposes and can be helpful for debugging or logging.
		/// @return A reference to the string containing the shader's name.
		inline [[nodiscard]] const std::string& GetName() const { return m_Name; }

	public:
		/// -> Methods for setting various uniform variables in the shader
		
		/// Sets a single integer uniform in the shader program.
		/// @param name - the name of the uniform variable in the shader source.
        /// @param value - the integer value to assign to the uniform variable.
		void SetInt(const std::string& name, int value);

		/// Sets an array of integers as a uniform in the shader program.
		/// @param name - the name of the uniform array in the shader source.
        /// @param values - a pointer to the array of integers to set.
        /// @param count - the number of elements in the array. This must match the array size in the shader.
		void SetIntArray(const std::string& name, int* values, uint32_t count);

		/// Sets a single float uniform in the shader program.
		/// @param name - the name of the uniform variable in the shader source.
        /// @param value - the float value to assign to the uniform variable.
		void SetFloat(const std::string& name, float value);

		/// Sets a vec2 uniform in the shader program.
		/// @param name - the name of the uniform variable in the shader source.
        /// @param value - the vec2 value to assign to the uniform variable.
		void SetFloat2(const std::string& name, const glm::vec2& value);

		/// Sets a vec3 uniform in the shader program.
		/// @param name - the name of the uniform variable in the shader source.
		/// @param value - the vec3 value to assign to the uniform variable.
		void SetFloat3(const std::string& name, const glm::vec3& value);

		/// Sets a vec4 uniform in the shader program.
		/// @param name - the name of the uniform variable in the shader source.
		/// @param value - the vec4 value to assign to the uniform variable.
		void SetFloat4(const std::string& name, const glm::vec4& value);

		/// Sets a mat3 uniform in the shader program.
		/// @param name - the name of the uniform variable in the shader source.
		/// @param value - the mat3 value to assign to the uniform variable.
		void SetMat3(const std::string& name, const glm::mat3& value);

		/// Sets a mat4 uniform in the shader program.
		/// @param name - the name of the uniform variable in the shader source.
		/// @param value - the mat4 value to assign to the uniform variable.
		void SetMat4(const std::string& name, const glm::mat4& value);

	private:
		/// Extracts the name of the shader from its file path.
        /// This method is used during shader initialization to give a shader a meaningful name
		void ExtractName();

		/// This method reads the shader file, preprocesses it, and compiles the shader by 
		/// creating and linking the shader program. It handles all the stages of shader 
		/// creation, including compiling individual shader stagesand linking them into a complete program
		void Compile();

		/// Reads the content of a file from disk.
		/// This utility function is used to load the shader source code into a string.
		/// @param filepath - the path to the file to read.
		/// @return - a string containing the entire contents of the file.
		[[nodiscard]] std::string ReadFile(const std::string filepath);

		/// This method processes the source code of the shader by handling preprocessor directives, 
		/// It reads the content of included files and integrates them into the shader source code,
		/// ensuring that all necessary code is available during compilation.
		/// @param source - a reference to the shader source code as a string, which will be modified
		///               to include the contents of any included files.
		void Preprocess(std::string& source);

		/// This method organizes the shader source code into different types based on defined tokens
		/// that indicate the start of vertex and fragment shader sections. It scans the entire shader 
		/// source for these tokens and extracts the relevant code segments for each shader type, 
		/// returning a map that associates each shader type with its corresponding source code.
		/// @param source - the shader source code as a string, which contains the vertex and 
		///               fragment shader sections marked by specific tokens.
		/// @return a map associating ShaderType enums with their corresponding source code strings.
		[[nodiscard]] std::map<ShaderType, std::string> GroupByType(const std::string& source);

		/// Applies shader substitutions to the provided source code string
		/// This function replaces all occurrences of specific placeholder strings in the shader source code
		/// with their corresponding values from the shader substitution map. The substitutions are retrieved
		/// from the renderer, allowing dynamic modification of shader source code at runtime. This is useful
		/// for injecting configuration options or constants into shaders without modifying the original source code.
		/// @param source - a reference to a string containing the shader source code. The function modifies this string
		///               in place by replacing any found placeholders with their corresponding values from the substitution map.
		void ApplySubstitutions(std::string& source);

	private: 
		/// It holds the OpenGL renderer ID for the compiled shader program.
		/// It is assigned when the shader program is created and linked, allowing the shader
		/// to be referenced in subsequent rendering calls.
		uint32_t m_RendererID = 0;

		/// Stores the file path to the shader source file
		std::string m_Filepath;

		/// Mame of the shader, extracted from the file path.
		/// It is used for logging and debugging purposes, providing a human-readable identifier
		std::string m_Name;

	};

}