///
/// @file ShaderLibrary.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the ShaderLibrary class.
/// 
/// @details This file defines the ShaderLibrary class, which is responsible for managing a collection
///          of shaders used in the application. The ShaderLibrary allows loading, storing, and retrieving 
///          shaders by their name, as well as recompiling shaders during runtime for development and debugging purposes.
///
///          The library also supports the addition of shader substitutions, enabling the replacement of
///          specific keywords or directives within the shader source code, which allows for dynamic 
///          customization of shaders based on predefined rules.
///

#pragma once

#include "Graphics/Data/Shader.h"

namespace KuchCraft {

	class ShaderLibrary
	{
	public:
		ShaderLibrary()  = default;
		~ShaderLibrary() = default;

		/// Loads a shader from a given file path and adds it to the library.
		/// This method creates a new shader, loads the source code from the specified file,
		/// compiles it, and stores it in the library for future use.
		/// @param filePath - the path to the shader source file.
		/// @return - a shared pointer to the loaded shader.
		[[nodiscard]] std::shared_ptr<Shader> Load(const std::filesystem::path& filePath);

	    /// Adds a shader to the library.
		/// Stores a shader in the internal map, associating it with its name.
		/// @param shader - a shared pointer to the shader to be added.
		void Add(const std::shared_ptr<Shader>& shader);

		/// Retrieves a shader from the library by its name.
		/// If the shader exists in the library, it is returned; otherwise, an error is logged.
		/// @param name - the name of the shader to retrieve.
		/// @return - a shared pointer to the shader if it exists, nullptr otherwise.
		[[nodiscard]] std::shared_ptr<Shader> Get(const std::string& name);

		/// Checks if a shader exists in the library.
		/// This method checks if a shader with the given name is present in the shader map.
		/// @param name - the name of the shader to check for.
		/// @return - true if the shader exists, false otherwise.
		[[nodiscard]] bool Exists(const std::string& name);

		/// Recompiles a specific shader by name.
		/// This method reloads the shader's source file and recompiles it.
		/// If the shader does not exist, an error is logged.
		/// @param name - the name of the shader to recompile.
		void ReCompile(const std::string& name);

		/// Recompiles all shaders in the library.
		/// Iterates through all the shaders in the library and recompiles each one.
		void ReCompileAll();

		/// Adds a substitution pair to the shader preprocessing system.
		/// The substitution maps a key (preceded by '##') to a replacement string,
		/// which can be used to inject custom values into the shader code during preprocessing.
		/// @param sub - a pair containing the key and the replacement value.
		void AddSubstitution(const std::pair<std::string, std::string>& sub);

		/// Retrieves the current map of shader substitutions.
		/// This method returns a constant reference to the internal substitution map.
		/// The map contains key-value pairs where keys are marked with '##' and represent
		/// placeholders in the shader code, which are replaced by the corresponding values
		/// during shader preprocessing.
		/// @return - a constant reference to the map of shader substitutions.
		inline [[nodiscard]] const auto& GetSubstitutionMap() { return s_Substitutions; }

	private:
		/// This unordered map holds shared pointers to all shaders loaded into the library,
		/// allowing them to be accessed by name.
		std::unordered_map<std::string, std::shared_ptr<Shader>> s_Shaders;

		/// This unordered map is used to store substitution rules where keys (preceded by '##')
		/// are replaced by their corresponding values in the shader source code.
		std::unordered_map<std::string, std::string> s_Substitutions;

	};

}